"""将完整标题、目录条目渲染为 SVG 文字，不向网页分发苹方或宋体字体文件。"""
from __future__ import annotations

import hashlib
import html
import json
from pathlib import Path

from fontTools.pens.svgPathPen import SVGPathPen
from fontTools.pens.transformPen import TransformPen
from fontTools.ttLib import TTFont


class PDFLabels:
    def __init__(self, snapshot, assets, font_paths=None):
        self.snapshot, self.assets = snapshot, assets
        self.used = {}
        self.fonts = {}
        if font_paths:
            self.fonts = {role: TTFont(path, fontNumber=0) for role, path in font_paths.items()}
            self.metadata = {role: {"name": font["name"].getDebugName(6),
                                    "sha256": hashlib.sha256(font_paths[role].read_bytes()).hexdigest()}
                             for role, font in self.fonts.items()}
            self.cached = {}
        else:
            saved = json.loads(snapshot.read_text())
            self.metadata, self.cached = saved["fonts"], saved["labels"]
        assets.mkdir(parents=True, exist_ok=True)

    def make_svg(self, text, role, wrap):
        paths, x, line, widest = [], 0, 0, 0
        for char in text:
            face = role if role != "body" else (
                "chinese" if ord(char) >= 0x2E80 else
                "math" if 0x2200 <= ord(char) <= 0x22FF else "latin")
            font = self.fonts[face]
            glyph_name = font.getBestCmap().get(ord(char))
            if glyph_name is None:
                raise ValueError(f"{face} 缺少字符：{char!r}")
            scale = 1000 / font["head"].unitsPerEm
            advance = font["hmtx"][glyph_name][0] * scale
            if wrap and x and x + advance > wrap * 1000:
                widest = max(widest, x)
                x, line = 0, line + 1
            pen = SVGPathPen(font.getGlyphSet(), ntos=lambda n: format(round(n, 2), 'g'))
            transformed = TransformPen(pen, (scale, 0, 0, -scale, x, 900 + line * 1400))
            font.getGlyphSet()[glyph_name].draw(transformed)
            if pen.getCommands():
                paths.append(pen.getCommands())
            x += advance
        width, height = round(max(widest, x), 2), 1150 + line * 1400
        svg = (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {width} {height}">'
               f'<g id="label" fill="currentColor"><path d="{"".join(paths)}"/></g></svg>')
        return {"text": text, "role": role, "wrap": wrap, "width": width, "height": height, "svg": svg}

    def render(self, text, role="regular", wrap=0, css=""):
        key = hashlib.sha256(json.dumps([text, role, wrap], ensure_ascii=False).encode()).hexdigest()[:20]
        record = self.used.get(key) or self.cached.get(key)
        if record is None:
            if not self.fonts:
                raise ValueError(f"矢量文字需更新：{text!r}；运行 generate_html.py --refresh-labels，见字体说明")
            record = self.make_svg(text, role, wrap)
        assert (record["text"], record["role"], record["wrap"]) == (text, role, wrap)
        self.used[key] = record
        w, h = record["width"], record["height"]
        return (f'<span class="pdf-label {css}" data-label="{key}" style="width:{w / 1000:g}em">'
                f'<svg aria-hidden="true" focusable="false" viewBox="0 0 {w} {h}">'
                f'<use href="assets/labels/{{{{LABEL_SPRITE}}}}#{key}"/></svg>'
                f'<span class="pdf-label-text">{html.escape(text)}</span></span>')

    def finish(self):
        groups = [record["svg"].split(">", 1)[1].removesuffix("</svg>").replace('id="label"', f'id="{key}"')
                  for key, record in self.used.items()]
        svg = '<svg xmlns="http://www.w3.org/2000/svg">' + "".join(groups) + '</svg>'
        filename = hashlib.sha256(svg.encode()).hexdigest()[:20] + ".svg"
        (self.assets / filename).write_text(svg)
        if self.fonts:
            self.snapshot.write_text(json.dumps({"fonts": self.metadata, "labels": self.used},
                                                ensure_ascii=False, indent=2) + "\n")
            for font in self.fonts.values():
                font.close()
        return filename

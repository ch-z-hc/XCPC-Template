"""核对 HTML 全部代码、目录、字体和站内资源，不依赖截图判断内容完整性。"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from html.parser import HTMLParser
from pathlib import Path
import sys
from urllib.parse import unquote, urlsplit

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "support/tools"))
from generate_html import COLORS, code_html, entry_id, load_entries
from fontTools.ttLib import TTFont


class BookParser(HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.entries, self.links, self.resources = [], [], []
        self.current = None
        self.in_code = False
        self.copies = 0

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "article":
            self.current = {**attrs, "source": ""}
            self.entries.append(self.current)
        if tag == "code":
            self.in_code = True
        if tag == "span" and self.in_code:
            classes = attrs.get("class", "").split()
            assert set(classes) <= set(COLORS) | {"t-bold"}, classes
        if tag == "a" and "data-entry" in attrs:
            self.links.append(attrs)
        if tag == "button" and attrs.get("class") == "copy-button":
            self.copies += 1
        for key in ("href", "src"):
            if key in attrs:
                self.resources.append(attrs[key])

    def handle_endtag(self, tag):
        if tag == "code":
            self.in_code = False
        if tag == "article":
            self.current = None

    def handle_data(self, text):
        if self.in_code:
            self.current["source"] += text


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--site", type=Path, default=ROOT / "dist")
    args = parser.parse_args()
    site = args.site.resolve()
    document = (site / "index.html").read_text(encoding="utf-8")
    book = BookParser()
    book.feed(document)
    expected = load_entries()
    assert len(book.entries) == len(book.links) == book.copies == len(expected)
    assert len({entry["id"] for entry in book.entries}) == len(expected)
    for entry, link, original in zip(book.entries, book.links, expected):
        assert entry["id"] == link["data-entry"] == entry_id(original)
        assert unquote(urlsplit(link["href"]).fragment) == entry["id"]
        assert entry["data-path"] == original.path
        assert entry["data-title"] == original.title
        assert entry["data-number"] == original.number
        assert entry["source"] == original.source, original.path
        assert entry["data-sha256"] == hashlib.sha256(original.source.encode()).hexdigest()
    sample = '\n\t// 中文 <>&\nconst char *s = "</code><script>alert(1)</script>";\n'
    escaped = code_html(sample, "sample.cpp")
    assert "<script>" not in escaped and "</code>" not in escaped
    probe = BookParser()
    probe.feed(f'<article><pre><code>{escaped}</code></pre></article>')
    assert probe.entries[0]["source"] == sample
    style_sample = '// 中文 f()\nnamespace demo { template < class T > long long f(T x) { return g<T>(x); } }\n'
    styled = code_html(style_sample, "sample.cpp")
    assert '<span class="t-type t-bold">demo</span>' in styled
    assert '<span class="t-type t-bold">std</span>' in code_html('using namespace std;\n', "sample.cpp")
    for name in ("f", "g"):
        assert f'<span class="t-function t-bold">{name}</span>' in styled, name
    for name in ("template", "class", "long"):
        assert f'<span class="t-keyword">{name}</span>' in styled, name
    assert '<span class="t-comment">// 中文 f()\n</span>' in styled
    palette_sample = code_html('struct Point {}; Point p; int n = 42; const char *s = "ok";\n', "sample.cpp")
    for role, text in (("type", "Point"), ("variable", "n"), ("number", "42"), ("string", "ok")):
        assert f'<span class="t-{role}">{text}</span>' in palette_sample, (role, text)
    macro_sample = code_html('#define V vector\nV<int> a;\n', "sample.cpp")
    assert macro_sample.count('<span class="t-number">V</span>') == 2
    css = (site / "assets/book.css").read_text()
    assert "background: #282c34" in css
    for role, color in COLORS.items():
        assert f'.{role} {{ color: {color}; }}' in css if role else f'--code-text: {color};' in css
    assert "t-bold" not in code_html("echo hello\n", "commands.txt")
    viewer = BookParser()
    viewer.feed((site / "pdf.html").read_text())
    for url in book.resources + viewer.resources:
        parsed = urlsplit(url)
        if parsed.scheme or parsed.netloc or not parsed.path:
            continue
        assert not parsed.path.startswith("/"), url
        assert (site / unquote(parsed.path)).exists(), url
    snapshot = json.loads((ROOT / "support/pages/pdf-labels.json").read_text())
    assert {face["name"] for face in snapshot["fonts"].values()} == {
        "PingFangSC-Regular", "PingFangSC-Semibold", "SimSun",
        "LMRoman10-Regular", "LatinModernMath-Regular"}
    label_resources = {urlsplit(url).path for url in book.resources if url.startswith("assets/labels/")}
    assert len(label_resources) == 1
    sprite_path = site / label_resources.pop()
    sprite = sprite_path.read_text()
    assert hashlib.sha256(sprite.encode()).hexdigest().startswith(sprite_path.stem)
    for key, record in snapshot["labels"].items():
        svg = record["svg"]
        assert svg.split(">", 1)[1].removesuffix("</svg>").replace('id="label"', f'id="{key}"') in sprite
        assert "<path " in svg and "<text" not in svg
    assert "local(" not in (site / "assets/book.css").read_text()
    for url in book.resources:
        path = site / urlsplit(url).path
        match = re.search(r"-([0-9a-f]{16})\.(css|js|woff2)$", path.name)
        if match:
            assert hashlib.sha256(path.read_bytes()).hexdigest().startswith(match[1])
    manifest = json.loads((site / "pdf/manifest.json").read_text())
    for name, record in manifest.items():
        data = (site / record["file"]).read_bytes()
        assert hashlib.sha256(data).hexdigest() == record["sha256"]
        assert data == (site / name).read_bytes()
        assert record["file"] in viewer.resources
    for name in ("template.pdf", "oeis-2up.pdf"):
        with (site / name).open("rb") as file:
            assert file.read(5) == b"%PDF-", name
    with TTFont(site / "assets/maple-mono-nf-cn.woff2") as font:
        required = {ord(c) for entry in expected for c in entry.source if not c.isspace()}
        assert not required - set(font.getBestCmap())
    with TTFont(site / "assets/maple-mono-nf-cn-bold.woff2") as font:
        assert font["name"].getDebugName(6) == "MapleMono-NF-CN-Bold"
        assert font["OS/2"].usWeightClass == 700
        assert not required - set(font.getBestCmap())
    assert "SIL OPEN FONT LICENSE Version 1.1" in (site / "assets/OFL.txt").read_text()
    assert "LaTeX Project Public License" in (site / "assets/GUST-FONT-LICENSE.txt").read_text()
    for filename, name in (("lmroman10-regular.woff2", "LMRoman10-Regular"),
                           ("latinmodern-math.woff2", "LatinModernMath-Regular")):
        with TTFont(site / "assets" / filename) as font:
            assert font["name"].getDebugName(6) == name
    print(f"PASS: {len(expected)} HTML entries, exact source text, stable links, font coverage and PDF resources")


if __name__ == "__main__":
    main()

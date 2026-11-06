"""与 PDF 共用目录、源码和词法配色，生成可复制代码的静态阅读页。"""
from __future__ import annotations

import argparse
import hashlib
import html
import shutil
import subprocess
from pathlib import Path
from urllib.parse import quote

from fontTools import subset
from fontTools.ttLib import TTFont

from generate_print import (
    ROOT, CHAPTERS, CODE_TEXT, CODE_BLUE, CODE_GREEN, CODE_COMMENT,
    code_lines, load_entries,
)

PAGES = ROOT / "support/pages"
COLORS = {CODE_TEXT: "", CODE_BLUE: "t-blue", CODE_GREEN: "t-green", CODE_COMMENT: "t-comment"}
COPY_ICON = '<svg viewBox="0 0 24 24" aria-hidden="true"><rect x="8" y="8" width="12" height="12" rx="2"/><path d="M16 8V4H4v12h4"/></svg>'


def entry_id(entry):
    return Path(entry.path).with_suffix("").as_posix().replace("/", "-")


def code_html(source, path):
    lines = []
    for tokens in code_lines(source, path):
        parts = []
        for value, color in tokens:
            text = html.escape(value, quote=False)
            parts.append(f'<span class="{COLORS[color]}">{text}</span>' if COLORS[color] else text)
        lines.append("".join(parts))
    return "\n".join(lines) + ("\n" if source.endswith("\n") else "")


def build_font(source, license_path, target, text):
    with TTFont(source) as font:
        required = {ord(c) for c in text if not c.isspace()}
        missing = required - set(font.getBestCmap())
        if missing:
            raise ValueError(f"代码字体缺字：{''.join(map(chr, sorted(missing)))}")
        options = subset.Options()
        options.hinting = False
        options.layout_features = []
        subsetter = subset.Subsetter(options=options)
        subsetter.populate(text=text)
        subsetter.subset(font)
        font.flavor = "woff2"
        font.save(target / "maple-mono-nf-cn.woff2")
    shutil.copyfile(license_path, target / "OFL.txt")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "dist")
    parser.add_argument("--code-font", type=Path,
                        default=ROOT / "build/pages-font/MapleMono-NF-CN-Regular.ttf")
    parser.add_argument("--font-license", type=Path, default=ROOT / "build/pages-font/LICENSE.txt")
    args = parser.parse_args()
    entries = load_entries()
    toc, articles = [], []
    for chapter_number, (chapter, _) in enumerate(CHAPTERS, 1):
        links = []
        for entry in entries:
            if entry.chapter != chapter:
                continue
            key = entry_id(entry)
            title = html.escape(entry.title)
            links.append(
                f'<a href="#{quote(key, safe="")}" data-entry="{html.escape(key)}">'
                f'<span class="entry-number">{entry.number}</span><span>{title}</span></a>')
        toc.append(
            f'<details class="toc-group"{" open" if chapter_number == 1 else ""}>'
            f'<summary><span>{chapter_number}</span>{html.escape(chapter)}</summary>'
            f'<div class="toc-links">{"".join(links)}</div></details>')
    for index, entry in enumerate(entries):
        key = entry_id(entry)
        title = html.escape(entry.title)
        filename = html.escape(Path(entry.path).name)
        source_hash = hashlib.sha256(entry.source.encode()).hexdigest()
        articles.append(
            f'<article class="entry" id="{html.escape(key)}" '
            f'data-path="{html.escape(entry.path)}" data-title="{title}" '
            f'data-number="{entry.number}" data-sha256="{source_hash}"'
            f'{" hidden" if index else ""}>'
            f'<p class="chapter-label">{entry.number.split(".")[0]} · {html.escape(entry.chapter)}</p>'
            f'<h1 tabindex="-1">{entry.number} {title}</h1>'
            f'<div class="code-card"><div class="card-header">'
            f'<span class="filename"><span class="code-icon" aria-hidden="true">&lt;/&gt;</span>{filename}</span>'
            f'<button class="copy-button" type="button" aria-label="复制代码：{title}">'
            f'{COPY_ICON}<span>复制</span></button></div>'
            f'<pre tabindex="0" aria-label="源码：{title}"><code>{code_html(entry.source, entry.path)}</code></pre>'
            f'</div></article>')
    args.output.mkdir(parents=True, exist_ok=True)
    assets = args.output / "assets"
    assets.mkdir(exist_ok=True)
    build_font(args.code_font, args.font_license, assets,
               "".join(e.source for e in entries) + "".join(e.title for e in entries))
    document = (PAGES / "index.html").read_text()
    revision = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip()
    for marker, value in {"<!-- TOC -->": "\n".join(toc),
                          "<!-- ENTRIES -->": "\n".join(articles),
                          "{{COUNT}}": str(len(entries)), "{{REVISION}}": revision}.items():
        assert document.count(marker) == 1, marker
        document = document.replace(marker, value)
    (args.output / "index.html").write_text(document, encoding="utf-8")
    css = (PAGES / "book.css").read_text()
    for name, color in {"CODE_TEXT": CODE_TEXT, "CODE_BLUE": CODE_BLUE,
                        "CODE_GREEN": CODE_GREEN, "CODE_COMMENT": CODE_COMMENT}.items():
        css = css.replace("{{" + name + "}}", color)
    (assets / "book.css").write_text(css, encoding="utf-8")
    shutil.copyfile(PAGES / "book.js", assets / "book.js")
    shutil.copyfile(PAGES / "pdf.html", args.output / "pdf.html")
    (args.output / ".nojekyll").touch()
    print(f"PASS: {len(entries)} HTML entries generated from PDF sources: {args.output}")


if __name__ == "__main__":
    main()

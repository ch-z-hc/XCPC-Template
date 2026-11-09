"""与 PDF 共用目录和源码，按 One Dark Pro Mix 生成可复制代码的静态阅读页。"""
from __future__ import annotations

import argparse
import hashlib
import html
from itertools import groupby
import re
import shutil
import subprocess
from pathlib import Path
from urllib.parse import quote

from fontTools import subset
from fontTools.ttLib import TTFont
from pdf_labels import PDFLabels
from pygments import lex
from pygments.lexers import BashLexer, CppLexer
from pygments.token import Token
from tree_sitter import Language, Parser
import tree_sitter_cpp

from generate_print import ROOT, CHAPTERS, load_entries

PAGES = ROOT / "support/pages"
# Acer 的 One Dark Pro Mix 3.19.0；仅用于 HTML，不改变打印册配色。
COLORS = {"": "#abb2bf", "t-keyword": "#c678dd", "t-function": "#61afef",
          "t-type": "#e5c07b", "t-variable": "#e06c75", "t-string": "#98c379",
          "t-number": "#d19a66", "t-comment": "#7f848e", "t-escape": "#56b6c2"}
COPY_ICON = '<svg viewBox="0 0 24 24" aria-hidden="true"><rect x="8" y="8" width="12" height="12" rx="2"/><path d="M16 8V4H4v12h4"/></svg>'


def entry_id(entry):
    return Path(entry.path).with_suffix("").as_posix().replace("/", "-")


def name_styles(source, path):
    if not path.endswith(".cpp"):
        return {}, set()
    offsets, byte = {}, 0
    for index, char in enumerate(source):
        offsets[byte] = index
        byte += len(char.encode())
    offsets[byte] = len(source)
    tree = Parser(Language(tree_sitter_cpp.language())).parse(source.encode())
    stack, styles, macros = [tree.root_node], {}, set()
    while stack:
        node = stack.pop()
        stack.extend(node.named_children)
        heavy = False
        if node.type in ("namespace_identifier", "type_identifier"):
            name, color = node, "t-type"
            heavy = node.type == "namespace_identifier"
        elif node.type == "using_declaration" and any(child.type == "namespace" for child in node.children):
            name, color, heavy = node.named_children[-1], "t-type", True
        elif node.type in ("preproc_def", "preproc_function_def"):
            name, color = node.child_by_field_name("name"), "t-number"
            if name:
                macros.add(name.text.decode())
        elif node.type == "function_declarator":
            name, color, heavy = node.child_by_field_name("declarator"), "t-function", True
        elif node.type == "call_expression":
            name, color, heavy = node.child_by_field_name("function"), "t-function", True
        else:
            continue
        while name and name.type in ("qualified_identifier", "template_function",
                                     "template_method", "field_expression"):
            name = name.child_by_field_name("name") or name.child_by_field_name("field")
        if name and name.type in ("identifier", "field_identifier", "namespace_identifier",
                                  "type_identifier", "operator_name", "destructor_name"):
            styles.update((index, (color, heavy))
                          for index in range(offsets[name.start_byte], offsets[name.end_byte]))
    return styles, macros


def code_html(source, path):
    cpp = path.endswith(".cpp")
    styles, macros = name_styles(source, path)
    parts, offset = [], 0
    lexer = CppLexer if cpp else BashLexer
    for token, value in lex(source, lexer(stripnl=False, ensurenl=False)):
        color = ""
        if token in Token.Comment.PreprocFile or token in Token.String:
            color = "t-escape" if token in Token.String.Escape else "t-string"
        elif token in Token.Comment.Preproc or token in Token.Keyword or token in Token.Operator:
            color = "t-keyword"
        elif token in Token.Comment:
            color = "t-comment"
        elif token in Token.Number or token in Token.Name and value in macros:
            color = "t-number"
        elif token in Token.Name:
            color = "t-variable"
        for (role, heavy), run in groupby(enumerate(value, offset),
                                         lambda item: ("t-number", False) if value in macros
                                         else styles.get(item[0], (color, False))):
            text = html.escape("".join(char for _, char in run), quote=False)
            classes = " ".join(filter(None, (role, "t-bold" if heavy else "")))
            parts.append(f'<span class="{classes}">{text}</span>' if classes else text)
        offset += len(value)
    assert offset == len(source)
    return "".join(parts)


def build_font(source, license_path, target, text, filename="maple-mono-nf-cn.woff2",
               license_name="OFL.txt"):
    with TTFont(source) as font:
        required = {ord(c) for c in text if not c.isspace()}
        missing = required - set(font.getBestCmap())
        if missing:
            raise ValueError(f"{source.name} 缺字：{''.join(map(chr, sorted(missing)))}")
        options = subset.Options()
        options.hinting = False
        options.layout_features = []
        subsetter = subset.Subsetter(options=options)
        subsetter.populate(text=text)
        subsetter.subset(font)
        font.flavor = "woff2"
        font.save(target / filename)
    shutil.copyfile(license_path, target / license_name)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "dist")
    parser.add_argument("--code-font", type=Path,
                        default=ROOT / "build/pages-font/MapleMono-NF-CN-Regular.ttf")
    parser.add_argument("--code-bold-font", type=Path,
                        default=ROOT / "build/pages-font/MapleMono-NF-CN-Bold.ttf")
    parser.add_argument("--font-license", type=Path, default=ROOT / "build/pages-font/LICENSE.txt")
    parser.add_argument("--text-font-dir", type=Path, default=ROOT / "build/pages-text-font")
    parser.add_argument("--refresh-labels", action="store_true", help="用 PDF 原字体更新完整文字的 SVG 快照")
    parser.add_argument("--heading-font", type=Path)
    parser.add_argument("--heading-bold-font", type=Path)
    parser.add_argument("--chinese-font", type=Path)
    args = parser.parse_args()
    entries = load_entries()
    assets = args.output / "assets"
    assets.mkdir(parents=True, exist_ok=True)
    font_paths = None
    if args.refresh_labels:
        if not all((args.heading_font, args.heading_bold_font, args.chinese_font)):
            parser.error("--refresh-labels 需指定 --heading-font、--heading-bold-font、--chinese-font")
        font_paths = {"regular": args.heading_font, "bold": args.heading_bold_font,
                      "chinese": args.chinese_font, "latin": args.text_font_dir / "lmroman10-regular.otf",
                      "math": args.text_font_dir / "latinmodern-math.otf"}
    labels = PDFLabels(PAGES / "pdf-labels.json", assets / "labels", font_paths)
    label = labels.render
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
                f'<span class="entry-number">{label(entry.number, "body")}</span>'
                f'<span>{label(entry.title, "body", 13)}</span></a>')
        toc.append(
            f'<details class="toc-group"{" open" if chapter_number == 1 else ""}>'
            f'<summary><span>{label(str(chapter_number), "bold")}</span>{label(chapter, "bold")}</summary>'
            f'<div class="toc-links">{"".join(links)}</div></details>')
    for index, entry in enumerate(entries):
        key = entry_id(entry)
        title = html.escape(entry.title)
        source_hash = hashlib.sha256(entry.source.encode()).hexdigest()
        articles.append(
            f'<article class="entry" id="{html.escape(key)}" '
            f'data-path="{html.escape(entry.path)}" data-title="{title}" '
            f'data-number="{entry.number}" data-sha256="{source_hash}"'
            f'{" hidden" if index else ""}>'
            f'<p class="chapter-label">{label(entry.number.split(".")[0] + " · " + entry.chapter)}</p>'
            f'<h1 tabindex="-1">{label(entry.number + " " + entry.title, "bold", 0, "label-wide")}'
            f'{label(entry.number + " " + entry.title, "bold", 12, "label-narrow")}</h1>'
            f'<div class="code-card"><div class="card-header">'
            f'<span class="filename"><span class="code-icon" aria-hidden="true">&lt;/&gt;</span>'
            f'{label(Path(entry.path).name, "regular", 0, "label-wide")}'
            f'{label(Path(entry.path).name, "regular", 13, "label-narrow")}</span>'
            f'<button class="copy-button" type="button" aria-label="复制代码：{title}">'
            f'{COPY_ICON}<span class="copy-text">{label("复制")}</span></button></div>'
            f'<pre tabindex="0" aria-label="源码：{title}"><code>{code_html(entry.source, entry.path)}</code></pre>'
            f'</div></article>')
    build_font(args.code_font, args.font_license, assets,
               "".join(e.source for e in entries) + "".join(e.title for e in entries)
               + (PAGES / "index.html").read_text() + (PAGES / "book.js").read_text())
    build_font(args.code_bold_font, args.font_license, assets,
               "".join(e.source for e in entries), "maple-mono-nf-cn-bold.woff2")
    text = "".join(e.title for e in entries) + "".join(c for c, _ in CHAPTERS) + "0123456789. "
    license_path = args.text_font_dir / "GUST-FONT-LICENSE.txt"
    build_font(args.text_font_dir / "lmroman10-regular.otf", license_path, assets,
               "".join(c for c in text if ord(c) < 0x2E80 and not 0x2200 <= ord(c) <= 0x22FF),
               "lmroman10-regular.woff2", "GUST-FONT-LICENSE.txt")
    build_font(args.text_font_dir / "latinmodern-math.otf", license_path, assets,
               "".join(c for c in text if 0x2200 <= ord(c) <= 0x22FF) + "∑∞∈",
               "latinmodern-math.woff2", "GUST-FONT-LICENSE.txt")
    document = (PAGES / "index.html").read_text()
    revision = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip()
    for marker, value in {"<!-- TOC -->": "\n".join(toc),
                          "<!-- ENTRIES -->": "\n".join(articles),
                          "{{COUNT}}": label(f"{len(entries)} 个条目"), "{{REVISION}}": revision}.items():
        assert document.count(marker) == 1, marker
        document = document.replace(marker, value)
    document = re.sub(r"\{\{L:(regular|bold|body):([^}]+)}}", lambda m: label(m[2], m[1]), document)
    document = document.replace("{{LABEL_SPRITE}}", labels.finish())
    css = (PAGES / "book.css").read_text()
    for name, color in COLORS.items():
        css = css.replace("{{" + (name or "code-text") + "}}", color)
    for path in assets.glob("*.woff2"):
        if re.search(r"-[0-9a-f]{16}\.woff2$", path.name):
            continue
        digest = hashlib.sha256(path.read_bytes()).hexdigest()[:16]
        versioned = f"{path.stem}-{digest}.woff2"
        shutil.copyfile(path, assets / versioned)
        css = css.replace(path.name, versioned)
        document = document.replace(path.name, versioned)
    (assets / "book.css").write_text(css, encoding="utf-8")
    shutil.copyfile(PAGES / "book.js", assets / "book.js")
    for name in ("book.css", "book.js"):
        data = (assets / name).read_bytes()
        versioned = f"{Path(name).stem}-{hashlib.sha256(data).hexdigest()[:16]}{Path(name).suffix}"
        (assets / versioned).write_bytes(data)
        document = document.replace("assets/" + name, "assets/" + versioned)
    (args.output / "index.html").write_text(document, encoding="utf-8")
    shutil.copyfile(PAGES / "pdf.html", args.output / "pdf.html")
    (args.output / ".nojekyll").touch()
    print(f"PASS: {len(entries)} HTML entries generated from PDF sources: {args.output}")


if __name__ == "__main__":
    main()

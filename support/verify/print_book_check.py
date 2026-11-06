"""验证打印册没有漏文件、漏行、漏字符或越界，并核对目录与 PDF 页数。"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

from pypdf import PdfReader
from pypdf.generic import ContentStream

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "support/tools"))
from generate_print import (  # noqa: E402
    BOTTOM, CARD_PAD, CODE_BLUE, CODE_COMMENT, CODE_GREEN,
    CODE_SIZE, CODE_TEXT, CODE_W, LEADING, LEFT, OUTPUT, PAGE_H, PAGE_W, RIGHT,
    MIN_CODE_SIZE, MAX_CODE_SIZE, TITLE_HEIGHT, TOP, card_top, card_bottom, code_lines, load_entries,
)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--pdf", type=Path, default=OUTPUT)
    path = parser.parse_args().pdf
    layout = json.loads(path.with_suffix(".layout.json").read_text(encoding="utf-8"))
    assert layout["font_size"] == CODE_SIZE and layout["line_height"] == LEADING
    assert layout["font_size_range"] == [MIN_CODE_SIZE, MAX_CODE_SIZE]
    code_baselines = {}
    for row in layout["rows"]:
        code_baselines.setdefault(row["page"], []).append((row["y"], row["font_size"]))
    entries = load_entries()
    paths = {entry.path for entry in entries}
    for name in ("stress_linux", "stress_windows", "gen"):
        assert f"support/tools/stress/{name}.cpp" in paths, name
    for name in ("sol", "bf"):
        assert f"support/tools/stress/{name}.cpp" not in paths, name
    index = (ROOT / "support/PROJECT.md").read_text(encoding="utf-8")
    for entry in entries:
        if entry.path.startswith("template/"):
            assert f"`{entry.path}`" in index, (entry.path, "接口索引漏收")
    reader = PdfReader(path)
    assert "chzhc" not in str(reader.metadata)
    assert len(reader.pages) == layout["pages"]
    assert len(layout["entries"]) == len(entries)
    for number, page in enumerate(reader.pages, 1):
        assert abs(float(page.mediabox.width) - PAGE_W) < .01
        assert abs(float(page.mediabox.height) - PAGE_H) < .01
        header, footer = [], []
        page.extract_text(visitor_text=lambda text, cm, tm, font, size:
                          header.append(text.strip())
                          if text.strip() and tm[5] > TOP + 5 else None)
        if header:
            label = "".join("".join(header).split())
            assert label == "目录" or label in {
                f"{entry.number.split('.')[0]}{entry.chapter}" for entry in entries
            }, (number, header)
        page.extract_text(visitor_text=lambda text, cm, tm, font, size:
                          footer.append((text.strip(), tm[4], tm[5], size))
                          if text.strip() and tm[5] < BOTTOM else None)
        assert len(footer) == 1 and footer[0][0] == str(number), (number, footer)
        assert footer[0][3] == 8 and abs(footer[0][2] - 15 * 72 / 25.4) < .01
        assert abs(footer[0][1] - PAGE_W / 2) < 10
        def check_font(text, cm, tm, font, size):
            if not text.strip():
                return
            is_code = any(abs(y - tm[5]) < .001 and abs(size - row_size) < .001
                          for y, row_size in code_baselines.get(number, []))
            if is_code or any(
                    char in "\ue61d\uebca" for char in text):
                role = "Code"
            elif size in (12, 14, 22):
                role = "HeadingBold"
            elif size in (8, 9):
                role = "Heading"
            elif any(ord(char) >= 0x2E80 and not char.isspace() for char in text):
                role = "Chinese"
            else:
                role = "Latin"
            expected = layout["fonts"][role]
            assert font and expected in str(font.get("/BaseFont", "")), (
                number, text, expected, font)
        page.extract_text(visitor_text=check_font)
    first_body = min(record["page"] for record in layout["entries"])
    contents = [annotation for page in reader.pages[:first_body - 1]
                for annotation in page.get("/Annots", [])]
    assert len(contents) == len(entries)
    for annotation, record in zip(contents, layout["entries"]):
        destination = annotation.get_object()["/Dest"]
        assert reader.get_page_number(destination[0].get_object()) + 1 == record["page"]
    title_offsets = []
    for page in reader.pages[:first_body - 1]:
        spans = []
        text = page.extract_text(visitor_text=lambda text, cm, tm, font, size:
                                 spans.append((text.strip(), tm[4], tm[5]))
                                 if text.strip() and size == 10.5 else None)
        assert "（续）" not in text
        for entry in entries:
            numbers = [(x, y) for text, x, y in spans if text == entry.number]
            for x, y in numbers:
                assert abs(x - (LEFT + 21)) < .01
                title_x = min(sx for _, sx, sy in spans
                              if abs(sy - y) < .01 and x < sx < x + 80)
                title_offsets.append(title_x - x)
    assert len(title_offsets) == len(entries)
    assert max(title_offsets) - min(title_offsets) < .01
    embedded = set()
    for page in reader.pages:
        for reference in page["/Resources"]["/Font"].values():
            font = reference.get_object()
            descriptor = font.get("/FontDescriptor")
            if font.get("/Subtype") == "/TrueType":
                assert descriptor and "/FontFile2" in descriptor.get_object()
                embedded.add(str(font["/BaseFont"]))
    assert embedded
    for role in ("Code", "Chinese", "Latin", "Heading", "HeadingBold"):
        assert any(layout["fonts"][role] in name for name in embedded), role
    colors = set()
    for page in reader.pages:
        for operands, operator in ContentStream(page.get_contents(), reader).operations:
            if operator == b"rg":
                colors.add(tuple(round(float(value) * 255) for value in operands))
    for color in (CODE_TEXT, CODE_BLUE, CODE_GREEN, CODE_COMMENT):
        assert tuple(bytes.fromhex(color[1:])) in colors, color
    # 跨行注释和字符串中的符号仍按词法着色，不重写代码。
    sample = '/* first\nsecond */\nconst char *s = "// text";\n'
    colored = code_lines(sample, "test.cpp")
    assert colored[1] == [("second */", CODE_COMMENT)]
    assert ("// text", CODE_GREEN) in colored[2]
    for entry, record in zip(entries, layout["entries"]):
        assert entry.path == record["path"]
        assert record["sha256"] == hashlib.sha256((ROOT / entry.path).read_bytes()).hexdigest()
        rows = [row for row in layout["rows"] if row["path"] == entry.path]
        assert rows and rows[0]["page"] == record["page"]
        size, leading = record["font_size"], record["line_height"]
        assert MIN_CODE_SIZE <= size <= MAX_CODE_SIZE
        assert abs(leading / size - LEADING / CODE_SIZE) < 1e-8
        assert rows[-1]["page"] == record["end_page"]
        gap = rows[-1]["y"] - card_bottom(size) - BOTTOM
        assert abs(gap - record["last_page_gap"]) < .001
        pages = sorted({row["page"] for row in rows})
        assert pages == list(range(record["page"], record["end_page"] + 1))
        if len(pages) > 1:
            assert sum(row["page"] == pages[0] for row in rows) >= 8
            assert sum(row["page"] == pages[-1] for row in rows) >= 6
        restored = {}
        for row in rows:
            assert row["font_size"] == size and row["line_height"] == leading
            assert row["part"] == 0, (entry.path, row["line"], "源码需要手动换行")
            assert BOTTOM + card_bottom(size) - .001 <= row["y"] <= TOP
            assert abs(row["x"] - LEFT - CARD_PAD) < .01
            assert row["width"] <= CODE_W + .001
            assert row["x"] + row["width"] <= PAGE_W - RIGHT - CARD_PAD + .001
            assert row["part"] == len(restored.setdefault(row["line"], []))
            restored[row["line"]].append(row["text"])
        expected = [line.expandtabs(4) for line in entry.source.splitlines()]
        assert list(restored) == list(range(1, len(expected) + 1))
        assert ["".join(parts) for parts in restored.values()] == expected
    # 检查实际 PDF 的文字层，避免只验证排版清单。
    texts = [page.extract_text() for page in reader.pages]
    assert "目录" in texts[0]
    for text in texts:
        for phrase in ("代码直接来自源码", "阅读顺序", "源码行号", "每张纸", "双面短边翻转",
                       "接口 /", "template/", "support/", "多测入口", "单测题删除",
                       "复制函数时不带示例入口", "chzhc", "比赛起手式", "公共定义"):
            assert phrase not in text, phrase
    for entry, record in zip(entries, layout["entries"]):
        assert entry.number + " " in texts[record["page"] - 1]
        page_rows = {}
        for row in layout["rows"]:
            if row["path"] == entry.path and row["text"].strip():
                page_rows.setdefault(row["page"], []).append(row["text"])
        for number, lines in page_rows.items():
            # 提取器可能重建空白；保留全部非空白字符及顺序核对。
            text = "".join(texts[number - 1].split())
            for line in lines:
                assert "".join(line.split()) in text, (entry.path, number, line)
    print(f"PASS: {len(entries)} entries, {len(reader.pages)} pages, all source lines preserved")


if __name__ == "__main__":
    main()

"""验证打印册没有漏文件、漏行、漏字符或越界，并核对目录与 PDF 页数。"""
from __future__ import annotations

import hashlib
import json
import sys
from pathlib import Path

from pypdf import PdfReader

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "support/tools"))
from generate_print import (  # noqa: E402
    BOTTOM, CODE_W, COL_W, GAP, LEADING, LEFT, OUTPUT, PAGE_H, PAGE_W,
    TITLE_HEIGHT, TOP, load_entries,
)


def main():
    path = OUTPUT
    layout = json.loads(path.with_suffix(".layout.json").read_text(encoding="utf-8"))
    entries = load_entries()
    index = (ROOT / "README.md").read_text(encoding="utf-8")
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
        header = []
        page.extract_text(visitor_text=lambda text, cm, tm, font, size:
                          header.append(text.strip())
                          if text.strip() and tm[5] > TOP + 5 else None)
        assert header == [str(number)], (number, header)
        filenames = []
        page.extract_text(visitor_text=lambda text, cm, tm, font, size:
                          filenames.append(text.strip())
                          if text.strip() and size == 7.5 else None)
        assert not filenames, (number, filenames)
        def check_font(text, cm, tm, font, size):
            if not any(ord(char) < 128 and not char.isspace() for char in text):
                return
            expected = "Times-Bold" if size in (12, 13, 18, 24) else "Consolas"
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
                                 if text.strip() and size == 9 else None)
        assert "（续）" not in text
        for entry in entries:
            numbers = [(x, y) for text, x, y in spans if text == entry.number]
            for x, y in numbers:
                assert min(abs(x - column) for column in
                           (LEFT + 7, LEFT + (COL_W + GAP) / 2 + 7)) < .01
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
    for entry, record in zip(entries, layout["entries"]):
        assert entry.path == record["path"]
        assert record["sha256"] == hashlib.sha256((ROOT / entry.path).read_bytes()).hexdigest()
        rows = [row for row in layout["rows"] if row["path"] == entry.path]
        assert rows and rows[0]["page"] == record["page"]
        if TITLE_HEIGHT + len(rows) * LEADING <= TOP - BOTTOM:
            assert len({row["page"] for row in rows}) == 1, entry.path
        elif entry.number.split(".")[1] != "1":
            assert abs(rows[0]["y"] - (TOP - TITLE_HEIGHT)) < .01, entry.path
        restored = {}
        for row in rows:
            assert row["part"] == 0, (entry.path, row["line"], "源码需要手动换行")
            assert BOTTOM <= row["y"] <= TOP
            assert abs(row["x"] - LEFT) < .01
            assert row["width"] <= CODE_W + .001
            assert row["x"] + row["width"] <= PAGE_W
            assert row["part"] == len(restored.setdefault(row["line"], []))
            restored[row["line"]].append(row["text"])
        expected = [line.expandtabs(4) for line in entry.source.splitlines()]
        assert list(restored) == list(range(1, len(expected) + 1))
        assert ["".join(parts) for parts in restored.values()] == expected
    # 检查实际 PDF 的文字层，避免只验证排版清单。
    texts = [page.extract_text() for page in reader.pages]
    assert "XCPC 算法模板" in texts[0]
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

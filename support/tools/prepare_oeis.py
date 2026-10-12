"""准备固定版本的 OEIS 原版附册和保留原文的 A4 横向 2 合 1 打印版。"""
from __future__ import annotations

import hashlib
import shutil
import tempfile
from pathlib import Path
from urllib.request import urlopen

from pypdf import PdfReader, PdfWriter, Transformation
from pypdf.generic import DecodedStreamObject
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm


ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / "output/pdf/oeis.pdf"
TWO_UP = ROOT / "output/pdf/oeis-2up.pdf"
SOURCE = (
    "https://github.com/Dusker233/OEIS-PrintMirror/releases/download/"
    "Daily_Update_2025-07-11/oeis.pdf"
)
SHA256 = "3ff9d9490fbe45f980054ced0962305ac616922b9b87412aa2d64ab89fbd515e"
PAGES = 975
SEQUENCES = 31120  # 对固定快照正文的条目标记统计，并按编号去重核对。


def verify(path: Path) -> None:
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    if digest != SHA256:
        raise ValueError(f"SHA-256 不符，不覆盖已有文件：{path}")
    reader = PdfReader(path)
    if len(reader.pages) != PAGES:
        raise ValueError(f"PDF 页数不符：{len(reader.pages)}")
    for number, page in enumerate(reader.pages, 1):
        if any(abs(float(value) - expected) > .1 for value, expected in
               zip((page.mediabox.width, page.mediabox.height), A4)):
            raise ValueError(f"第 {number} 页不是 A4 竖版")


def add_dashed_frames(page, count: int) -> None:
    width, height = map(float, (page.mediabox.width, page.mediabox.height))
    inset = 2 * mm
    commands = "q\n0.55 G\n0.4 w\n[3 3] 0 d\n"
    for slot in range(count):
        commands += (
            f"{slot * width / 2 + inset:.5f} {inset:.5f} "
            f"{width / 2 - 2 * inset:.5f} {height - 2 * inset:.5f} re S\n"
        )
    commands += "Q\n"
    stream = DecodedStreamObject()
    stream.set_data(b"q\n" + page.get_contents().get_data() + b"\nQ\n" + commands.encode("ascii"))
    page.replace_contents(stream.flate_encode())


def generate_two_up() -> None:
    reader = PdfReader(OUTPUT)
    writer = PdfWriter()
    width, height = map(float, (reader.pages[0].mediabox.width,
                                reader.pages[0].mediabox.height))
    scale = min(height / 2 / width, width / height)
    for first in range(0, len(reader.pages), 2):
        sheet = writer.add_blank_page(height, width)
        for slot, number in enumerate(range(first, min(first + 2, len(reader.pages)))):
            x = slot * height / 2 + (height / 2 - width * scale) / 2
            y = (width - height * scale) / 2
            sheet.merge_transformed_page(
                reader.pages[number],
                Transformation().scale(scale).translate(x, y),
            )
        add_dashed_frames(sheet, min(2, len(reader.pages) - first))
        if len(writer.pages) % 50 == 0:
            print(f"BUILD: {len(writer.pages)}/{(PAGES + 1) // 2} pages", flush=True)
    writer.add_metadata({"/Title": "OEIS - A4 landscape 2-up"})
    writer.compress_identical_objects()
    with tempfile.TemporaryDirectory(dir=TWO_UP.parent) as directory:
        generated = Path(directory) / TWO_UP.name
        writer.write(generated)
        check = PdfReader(generated)
        assert len(check.pages) == (PAGES + 1) // 2
        for index, page in enumerate(check.pages):
            assert abs(float(page.mediabox.width) - height) < .1
            assert abs(float(page.mediabox.height) - width) < .1
            expected = "".join(
                "".join(reader.pages[number].extract_text().split())
                for number in range(index * 2, min(index * 2 + 2, PAGES))
            )
            assert "".join(page.extract_text().split()) == expected, index + 1
            if (index + 1) % 50 == 0:
                print(f"VERIFY: {index + 1}/{len(check.pages)} pages", flush=True)
        generated.replace(TWO_UP)
    shutil.copyfile(TWO_UP, ROOT / "oeis-2up.pdf")
    print(f"PASS: {PAGES} source pages preserved in {len(check.pages)} A4 landscape pages")
    print(TWO_UP)


def main() -> None:
    if OUTPUT.exists():
        verify(OUTPUT)
    else:
        OUTPUT.parent.mkdir(parents=True, exist_ok=True)
        # 下载或校验失败时删除临时文件，完整通过后才发布输出。
        with tempfile.TemporaryDirectory(dir=OUTPUT.parent) as directory:
            downloaded = Path(directory) / "oeis.pdf"
            with urlopen(SOURCE, timeout=60) as response, downloaded.open("wb") as stream:
                shutil.copyfileobj(response, stream)
            verify(downloaded)
            downloaded.replace(OUTPUT)
    print(f"PASS: {SEQUENCES} sequences, {PAGES} pages, SHA-256 and A4 verified")
    print(OUTPUT)
    generate_two_up()


if __name__ == "__main__":
    main()

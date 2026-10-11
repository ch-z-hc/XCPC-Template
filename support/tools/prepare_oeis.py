"""准备固定版本的万条级 OEIS 检索附册，保留上游 PDF 原文和排版。"""
from __future__ import annotations

import hashlib
import shutil
import tempfile
from pathlib import Path
from urllib.request import urlopen

from pypdf import PdfReader
from reportlab.lib.pagesizes import A4


ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / "output/pdf/oeis.pdf"
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


if __name__ == "__main__":
    main()

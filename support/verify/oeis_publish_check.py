"""用三页样本验证 OEIS 拼版后会创建并更新根目录的正式打印文件。"""
import sys
import tempfile
from pathlib import Path
from unittest.mock import patch

from reportlab.pdfgen import canvas

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import prepare_oeis


with tempfile.TemporaryDirectory() as directory:
    root = Path(directory)
    source = root / "source.pdf"
    generated = root / "output/oeis-2up.pdf"
    generated.parent.mkdir()
    pdf = canvas.Canvas(str(source))
    for number in range(3):
        pdf.drawString(50, 700, f"source page {number + 1}")
        pdf.showPage()
    pdf.save()
    with patch.multiple(prepare_oeis, ROOT=root, OUTPUT=source, TWO_UP=generated, PAGES=3):
        prepare_oeis.generate_two_up()
        published = root / "oeis-2up.pdf"
        assert published.read_bytes() == generated.read_bytes()
        published.write_bytes(b"outdated print copy")
        prepare_oeis.generate_two_up()
        assert published.read_bytes() == generated.read_bytes()
print("PASS: OEIS creates and refreshes the root print copy")

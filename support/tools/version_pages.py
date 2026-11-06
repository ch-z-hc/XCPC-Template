"""为 PDF 阅读入口生成内容哈希路径，避免浏览器继续打开缓存中的旧版。"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--site", type=Path, default=ROOT / "dist")
    site = parser.parse_args().site
    target = site / "pdf"
    target.mkdir(parents=True, exist_ok=True)
    viewer = (ROOT / "support/pages/pdf.html").read_text()
    manifest = {}
    for name in ("template.pdf", "oeis-2up.pdf"):
        source = site / name
        digest = hashlib.sha256(source.read_bytes()).hexdigest()
        filename = f"{source.stem}-{digest[:16]}.pdf"
        shutil.copyfile(source, target / filename)
        url = "pdf/" + filename
        viewer = viewer.replace(f'href="{name}"', f'href="{url}"')
        viewer = viewer.replace(f'src="{name}#', f'src="{url}#')
        manifest[name] = {"file": url, "sha256": digest}
    (site / "pdf.html").write_text(viewer)
    (target / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print("PASS: versioned PDF viewer resources")


if __name__ == "__main__":
    main()

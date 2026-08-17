from __future__ import annotations

import re
import unicodedata
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
README = ROOT / "README.md"
OUTPUT_DIR = ROOT / "docs" / "algorithms"
CATEGORIES = {
    "ds": "数据结构",
    "geometry": "几何",
    "graph": "图论",
    "math": "数学",
    "others": "其他",
    "string": "字符串",
}

INTERFACE_ROW = re.compile(r"^\|\s*`([^`]+\.cpp)`\s*\|\s*(.*?)\s*\|\s*$")
COMPLEXITY_MARKERS = ("复杂度", "O(")
INDEX_MARKERS = ("下标", "编号", "0-based", "1-based")
PITFALL_MARKERS = ("要求", "必须", "不能", "注意", "概率", "递归", "非负", "质数", "负环")


def read_interfaces() -> dict[str, str]:
    interfaces: dict[str, str] = {}
    for line in README.read_text(encoding="utf-8").splitlines():
        match = INTERFACE_ROW.match(line)
        if match:
            interfaces[match.group(1)] = match.group(2)
    return interfaces


def slugify(name: str) -> str:
    ascii_name = unicodedata.normalize("NFKD", name).encode("ascii", "ignore").decode()
    slug = re.sub(r"[^a-z0-9]+", "-", ascii_name.lower()).strip("-")
    return slug or "template"


def first_comment_block(source: str) -> list[str]:
    blocks: list[list[str]] = []
    current: list[str] = []
    for line in source.splitlines()[:25]:
        stripped = line.strip()
        if stripped.startswith("//"):
            comment = stripped[2:].strip()
            if comment and not comment.lower().startswith("usage:"):
                current.append(comment)
        elif current:
            blocks.append(current)
            current = []
    if current:
        blocks.append(current)
    for block in blocks:
        if not all(line.startswith("#") for line in block):
            return block
    return []


def lines_with_markers(lines: list[str], markers: tuple[str, ...]) -> list[str]:
    return [line for line in lines if any(marker in line for marker in markers)]


def paragraph(lines: list[str], fallback: str) -> str:
    return "  \n".join(lines) if lines else fallback


def write_if_changed(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and path.read_text(encoding="utf-8") == content:
        return
    path.write_text(content, encoding="utf-8", newline="\n")


def template_page(source_path: Path, source: str, public_interface: str) -> str:
    relative_path = source_path.relative_to(ROOT).as_posix()
    comments = first_comment_block(source)
    complexity = lines_with_markers(comments, COMPLEXITY_MARKERS)
    indexing = lines_with_markers(comments, INDEX_MARKERS)
    pitfalls = lines_with_markers(comments, PITFALL_MARKERS)
    title = source_path.stem
    return f"""# `{title}`

> 本页由 `tools/generate_docs.py` 自动生成，请修改源文件而不是本页。

- 文件路径：`{relative_path}`
- 公开接口：{public_interface}

## 适用条件

{paragraph(comments, "源文件尚未提供独立适用条件说明，请先阅读实现和项目约定。")}

## 复杂度

{paragraph(complexity, "源文件尚未单独标注复杂度，请结合实现确认。")}

## 下标约定

{paragraph(indexing, "源文件尚未单独标注下标约定，请在调用前确认容器大小和编号范围。")}

## 调用方式

复制下方代码到比赛代码中，并通过上述公开接口调用；模板本身不提供 `int main()`。

## 代码

```cpp
{source.rstrip()}
```

## 易错点

{paragraph(pitfalls, "使用前确认输入满足适用条件，并按公开接口初始化所有状态。")}
"""


def category_page(title: str, pages: list[tuple[str, str, str]]) -> str:
    links = "\n".join(
        f"- [`{name}`]({slug}.md) — `{source_path}`" for name, slug, source_path in pages
    )
    return f"""# {title}

本分类共 {len(pages)} 个模板。

{links}
"""


def index_page(categories: list[tuple[str, str, list[tuple[str, str, str]]]]) -> str:
    sections = []
    total = 0
    for directory, title, pages in categories:
        total += len(pages)
        sections.append(f"## [{title}]({directory}/index.md)\n\n共 {len(pages)} 个模板。")
    joined_sections = "\n\n".join(sections)
    return f"""# 模板索引

本页及各模板页面由源代码自动生成，当前共收录 {total} 个模板。

{joined_sections}
"""


def generate_docs() -> None:
    interfaces = read_interfaces()
    expected: set[Path] = set()
    generated_categories: list[tuple[str, str, list[tuple[str, str, str]]]] = []

    for directory, title in CATEGORIES.items():
        category_dir = ROOT / directory
        pages: list[tuple[str, str, str]] = []
        used_slugs: set[str] = set()
        for source_path in sorted(category_dir.glob("*.cpp"), key=lambda path: path.name.lower()):
            slug = slugify(source_path.stem)
            if slug in used_slugs:
                raise ValueError(f"生成页面文件名冲突：{directory}/{slug}.md")
            used_slugs.add(slug)

            relative_path = source_path.relative_to(ROOT).as_posix()
            public_interface = interfaces.get(
                relative_path,
                "`README.md` 尚未登记；请从下方代码确认公开结构体或函数。",
            )
            source = source_path.read_text(encoding="utf-8")
            page_path = OUTPUT_DIR / directory / f"{slug}.md"
            write_if_changed(page_path, template_page(source_path, source, public_interface))
            expected.add(page_path.resolve())
            pages.append((source_path.stem, slug, relative_path))

        category_index = OUTPUT_DIR / directory / "index.md"
        write_if_changed(category_index, category_page(title, pages))
        expected.add(category_index.resolve())
        generated_categories.append((directory, title, pages))

    main_index = OUTPUT_DIR / "index.md"
    write_if_changed(main_index, index_page(generated_categories))
    expected.add(main_index.resolve())

    if OUTPUT_DIR.exists():
        output_root = OUTPUT_DIR.resolve()
        for path in OUTPUT_DIR.rglob("*.md"):
            resolved = path.resolve()
            if resolved.is_relative_to(output_root) and resolved not in expected:
                path.unlink()


def on_config(config: object) -> object:
    generate_docs()
    return config


if __name__ == "__main__":
    generate_docs()

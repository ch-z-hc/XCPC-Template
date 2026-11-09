---
name: mkdocs-formula-generation
description: MkDocs 公式应在生成器的说明文字阶段转换，不污染可复制的算法源码
metadata:
  type: project
---

模板开头注释继续使用适合 C++ 源码阅读的普通写法；`tools/generate_docs.py` 仅在生成页面说明文字时添加 `$...$`、`\mathcal{O}` 和其他 LaTeX 记号。代码块保留源码原文。

**Why:** 模板需要直接复制到比赛代码中，不应在 C++ 注释内混入文档站专用的 MathJax 定界符。
**How to apply:** 新增公式格式时扩展生成器转换规则，并用 `mkdocs build --strict` 检查生成 HTML 中的 `arithmatex` 输出。

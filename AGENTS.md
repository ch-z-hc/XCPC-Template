# XCPC Template 项目协作规则

## 项目目标

这是一个 C++17 XCPC 算法模板库，同时生成便于比赛查阅的 PDF 打印册。

代码目录保存可直接复制的算法片段；Microsoft To Do 负责记录模板整理、验证和复习进度。

## 模板代码规则

- C++ 标准库统一使用 `#include <bits/stdc++.h>`，不再另列 `<cassert>` 等已覆盖的头文件；扩展库头文件和验证程序的源码引用按需保留。
- 算法模板文件默认不包含 `int main()`。
- `template/` 下的 `.cpp` 文件使用中文算法名；重命名时同步索引、打印目录和验证路径，代码接口保持不变。
- 算法模板不得绑定题目输入输出，不保留题目专属的 `solve()`。
- `int main()` 只允许出现在 `support/verify/` 验证程序或明确的工具程序中。
- 计算几何以 Git `8f2e476` 的原版为基础，保留 `P`、`db`、`EPS` 和已有函数写法，使用 `V`、移除 `debug` 宏，并保留已确认的错误修复。按用户要求拆成算法片段，共用 `点与向量.cpp` 中的基础定义；原 `solve()` / `main()` 移至 `support/verify/geometry_cut_submission.cpp`，不统一重写几何接口。
- 不使用自定义头文件组织模板；模板应能直接复制到比赛代码中。
- 每个模板必须提供清晰的公开结构体或函数接口。
- 接口命名使用小写下划线或 PascalCase，避免含义不明确的缩写。
- 命名以场上好写、好改为先，采用 SA、AC、MCMF、ST、Edge 等常见短名，不堆算法前缀。辅助类型放在所属结构体内；直接改名并同步测试与索引，不保留累赘的兼容别名。
- 代码风格以 `support/skills/chzhc-xcpc-style/SKILL.md` 为准：允许 DSU、LCA 等常见算法缩写，以及 fa、siz、dep 等竞赛变量名。
- 保留 `V < int >`、`++ i`、`ans ++`、`! op[x]` 等个人写法；简单语句适度压行，复杂逻辑展开。
- DFS 使用递归写法，不改成显式栈模拟或先收集遍历序再逆序处理；深链在 Linux 验证时提高栈限制。算法本身需要的 BFS、单调栈、Tarjan 分量栈等保留。
- 线段树直接维护 sum、lazy 或具体节点字段，不采用 Info / Tag 泛型骨架。
- 文件头说明适用条件、复杂度、下标和必要的数值限制；文件尾用最小示例展示调用。
- 注释只写使用约定和不直观的原因，不复述代码，不添加与算法无关的“无模数”等声明。边界、失败返回值、状态重置、复制依赖和来源许可须保留。
- 计算几何保留原有注释，不参与注释风格统一。
- 默认图和树使用 1-based 顶点编号；不同约定必须在文件中明确说明。
- 避免全局可变状态；如果必须使用状态，应提供 `init`、`clear` 或可重复 `build` 的接口。
- 不要为了统一风格重写已经正确且与当前任务无关的算法。

## 当前主要接口约定

- 图论：复杂状态使用结构体，简单算法可直接使用函数；调用形式为 `add_edge`、`build`、`query` 或对应的明确名称。
- 数据结构：使用结构体封装状态，支持初始化、更新和查询。
- 数学：函数显式接收模数，或在文件中明确固定模数。
- 字符串：字符串构建和查询分离，避免依赖全局字符数组。
- 几何：保留原接口与码风；先复制基础定义，再复制需要的片段，依赖写在文件头，不使用自定义头文件。几何模板不再保留示例入口。

## 验证要求

- 新增或修改模板后，至少执行：

  ```text
  g++ -std=c++17 -Wall -Wextra -pedantic -fsyntax-only <file>.cpp
  ```

- 修改公共接口或多个模板时，运行：

  ```text
  g++ -std=c++17 -O2 -Wall -Wextra -pedantic support/verify/snippets_smoke.cpp -o support/verify/snippets_smoke.exe
  support/verify/snippets_smoke.exe
  ```

- 正式全量验证在 Linux 下运行 `bash support/verify/run_linux.sh`，栈设为 256 MiB，执行语法检查、随机测试和回归测试，并启用 ASan / UBSan。
- 验证程序可以包含 `main()`，但不得把验证入口复制回算法模板。
- 算法正确性不能只依赖编译；重要模板应增加样例、随机测试或对拍。
- `.exe`、`.o` 和其他构建产物不得提交到 Git。

## Git 工作流

- 每个有明确意义的阶段单独提交。
- 提交前检查 `git diff --check` 和 `git status`。
- 提交信息使用简洁的 Conventional Commits 风格，例如：

  ```text
  feat: add reusable template
  refactor: expose graph interfaces
  fix: reset reusable state
  docs: update template index
  ```

- 不使用破坏性命令覆盖用户未确认的修改。
- 当前目录包含 Nutstore 同步目录的 reparse point；批量移动、重命名或删除前必须先确认目标范围。

## 打印册规则

- 使用 `support/tools/generate_print.py` 从 `.cpp` 原文生成 PDF，不维护第二份代码。
- 输出放在 `output/pdf/`，采用 A4 竖版单栏，保留个人码风，不为排版改写算法。
- 不显示代码行号，不保留行号栏；代码与标题左对齐。
- PDF 尽量减少留白：短代码优先整页容纳；长代码仅在剩余至少半页时接排，否则标题和代码一起换页。缩小字号后能整页容纳的代码也优先整体换页；单份代码保持相同字号和行距，在 7.5–8.5 pt 内按行宽、可用空间和页数自动调整，尽量让末页靠近页底，不为凑页改写或删减源码。修改分页后运行 `print_pagination_check.py`、`print_book_check.py` 并渲染检查。
- 超出版心的源码行在合适的语义或语法位置手动换行；注释续行补 `//`，代码保留缩进，不依赖 PDF 自动续排。几何也允许这种仅换行的调整。
- 版面只保留目录、算法标题、必要说明、代码和简洁页眉页码；阅读顺序、生成方式和打印设置写在 `support/PROJECT.md`，不印入模板册。
- 目录标题和顺序在生成脚本的 `CHAPTERS` 中维护；接口索引保存在 `support/PROJECT.md`，适用条件、复杂度和易错点保留在源码注释中。
- 新增模板时同步维护接口索引和打印目录，运行 `support/verify/print_book_check.py` 检查完整性，并渲染 PDF 检查裁切、乱码和分页。
- 不引入 MkDocs；HTML 阅读页按下节维护，对拍说明保存在 `support/tools/stress/README.md`。

## HTML 阅读页

- `support/tools/generate_html.py` 直接复用 PDF 的 `load_entries`、`code_lines` 与配色；页面外观和交互在 `support/pages/`，输出到忽略的 `dist/`。不维护第二份模板或章节目录。
- 复制内容须与对应源码逐字符一致，保留空白和末尾换行；代码不加行号，不做展示层的中英文空格改写。
- 保持浅色代码卡片、目录搜索、原生 PDF / OEIS 入口，手机代码只在卡片内横向滚动。修改后运行 `html_book_check.py` 和真实浏览器交互检查，检查桌面及手机。
- HTML 字形按 PDF 分工：标题用苹方 Semibold，静态界面和文件名用苹方 Regular，目录正文用宋体 / Latin Modern Roman，非代码数学符号用 Latin Modern Math。上述完整文字生成 SVG 轮廓，保留真实文本供搜索与辅助技术使用；快照为 `support/pages/pdf-labels.json`，标签变化时按字体说明更新。代码保持原生文本，使用随页发布的 Maple Mono NF CN WOFF2 子集，输入框也使用该网络字体。不依赖本机字体，不分发苹方或宋体字体文件。
- CSS、JavaScript、网络字体和 PDF 阅读入口使用内容哈希路径；PDF 下载完成后运行 `version_pages.py`。验收须包含 `html_ui_check.py --isolated-fonts`，在没有安装 PDF 字体的浏览器环境核对字形与复制。
- `Publish book` 工作流在相关源码 push 后更新 HTML，也响应 Release 更新；PDF 仍从最新正式 Release 下载，不因源码 push 自动重新生成。

## To Do 对应关系

Microsoft To Do 中的一个 XCPC 模板任务对应一个 `.cpp` 模板文件。

完成标准：

1. 代码已整理为无 `main()` 的片段；
2. 对外接口和调用方式已明确；
3. 复杂度、适用条件和易错点已记录；
4. 至少完成一次编译和运行验证；
5. 文件已加入代码目录、接口索引和打印目录。

## 语言和文字格式

- 面向用户的文档、注释和说明使用简洁中文。
- 中文与英文、数字之间保留空格，例如：`C++17 模板`、`1-based 下标`。
- 代码、函数名、文件名和 Git 提交信息遵循代码风格，不强行添加中文空格。

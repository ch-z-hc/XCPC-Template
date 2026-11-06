# 项目说明

## 在线阅读与下载

- [HTML 在线阅读](https://ch-z-hc.github.io/XCPC-Template/)：目录搜索、单篇阅读与一键复制代码。
- [PDF 在线阅读](https://ch-z-hc.github.io/XCPC-Template/pdf.html)：算法模板册和 OEIS 2 合 1 打印版。
- [最新 Release](https://github.com/ch-z-hc/XCPC-Template/releases/latest)：下载 `template.pdf`、`oeis-2up.pdf`。

HTML 直接从 `main` 分支源码生成，与 PDF 共用章节顺序、词法配色和代码内容。页面保留 HTML / PDF / OEIS 三个入口；PDF 作为 Release 附件发布，不提交到 Git，使用浏览器原生阅读器。相关源码 push、发布或编辑 Release 时触发 `Publish book`，也可手动运行。HTML 使用最新源码，PDF 使用最新正式 Release 附件；源码 push 不会重新生成 PDF，两者版本可能不同。

更新时先生成、校验 PDF，并提交和推送对应源码，再创建新标签的草稿 Release，上传根目录的 `template.pdf`、`oeis-2up.pdf` 后发布。OEIS 原版仅作为本地生成缓存，不发布。

### HTML 本地构建与检查

```text
python -m pip install -r requirements-pages.txt
python support/tools/generate_html.py --code-font /path/to/MapleMono-NF-CN-Regular.ttf --font-license /path/to/LICENSE.txt
gh release download --repo ch-z-hc/XCPC-Template --dir dist --pattern template.pdf --pattern oeis-2up.pdf
python support/tools/version_pages.py --site dist
python support/verify/html_book_check.py --site dist
python -m http.server 8765 --bind 127.0.0.1 --directory dist
```

生成器默认从 `build/pages-font/` 读取 Maple Mono 和许可；CI 从 Maple Mono v7.9 官方发行包取得这两个文件，并验证固定 SHA-256。另需在 `build/pages-text-font/` 放置 `lmroman10-regular.otf`（2.007）、`latinmodern-math.otf`（1.959）和 `GUST-FONT-LICENSE.txt`，可用 `--text-font-dir` 指定目录；CI 的 `Prepare PDF text fonts` 步骤记录下载地址和校验值。输出包含源码生成的 HTML、样式、脚本、WOFF2 子集和 OFL / GFL 许可，不需要 Node.js 构建或运行服务。`html_book_check.py` 核对全部篇目的代码文本、目录锚点、字体覆盖和本地 PDF 资源。

标题、目录和代码分别沿用 PDF 的字体，具体分工见 [字体说明](fonts/README.md)。标题、目录与文件名使用原字体生成的完整文字 SVG，并保留真实文本；代码使用随页发布的 Maple Mono，读者无需安装字体。修改文字标签后须按字体说明更新 `support/pages/pdf-labels.json`。CSS、脚本、网络字体与 PDF 阅读路径带内容哈希，更新后不会继续使用同名旧资源；根目录 PDF 下载地址仍保留。

浏览器检查另需 `python -m pip install playwright==1.58.0` 和 `python -m playwright install chromium`；启动上面的预览服务后运行 `python support/verify/html_ui_check.py --url http://127.0.0.1:8765/`。可通过 `--browser` 使用已有 Chromium；Linux 下加 `--isolated-fonts`，用只包含 DejaVu 的独立 Fontconfig 验证未安装 PDF 字体时的渲染。检查覆盖真实剪贴板、备用复制与失败反馈、搜索、前后篇和历史导航、320 / 390 / 768 px 布局、手机目录和禁用 JavaScript 时的阅读。截图保存在 `build/html-preview/`。

HTML 为每个文件保留稳定锚点；`/` 或 `Ctrl/Cmd + K` 聚焦搜索。代码可手动选中或点击复制，复制保留原始空格、缩进和末尾换行。原首页的 `#template` / `#oeis` 链接会转到相应 PDF 阅读器。

## 目录结构

- `template/`：可直接复制的 C++17 算法模板，按数据结构、图论、数学等分类；
- `support/`：个人码风、验证程序和开发工具；
- `output/pdf/`：自动生成的打印册；
- 根目录：最终打印用的 `template.pdf`、`oeis-2up.pdf`，以及打印依赖和项目说明等配置。

## 约定

- 算法片段放在分类目录中，默认不包含 `int main()`。
- `template/` 下的 `.cpp` 使用中文算法名；结构体和函数名仍保留便于抄写的原接口。
- `int main()` 只保留在比赛入口、验证程序或明确的工具程序中。
- 文件头注明适用条件、复杂度、下标和必要的数值限制；文件尾保留最小调用示例。
- DFS 使用递归，不使用显式栈模拟或遍历序倒推；算法本身需要的 BFS 和栈不受影响。
- 对拍程序属于验证工具，可以保留自己的 `main()`，但不要混入算法片段。

## 个人码风

注释按三处组织：文件头写使用约定，函数旁写局部语义或不直观的原因，文件尾用 `用法：` 引出示例。复杂度、溢出限制、失败返回值、重置方式和复制依赖保留；删去重复解释、显而易见的步骤和与算法无关的声明。中文与英文、数字之间留空格，路径、标识符和代码表达式内部保持原样。计算几何保留原有注释，不参与上述统一整理。

码风规范保存在 `support/skills/chzhc-xcpc-style/SKILL.md`，覆盖命名、空格、适度压行和线段树 A 写法。只约束竞赛代码，不影响其他项目；计算几何以 Git `8f2e476` 的 `P` / `db` / `EPS` 原版为基础，将 `vector` 写为 `V`、移除 `debug` 宏，并按本次审查修复排序、宏参数与凸包方向问题，不参与统一码风重写。

常量写 `const`，不写 `constexpr`；少量结构体共用常量使用 `static const`，片段末尾的配套定义也要复制。普通查询函数不加尾部 `const`，不再支持通过 `const` 对象调用；原版几何保留原写法。大数组、字符串等输入仍用 `const &`，避免整份复制并支持临时参数；局部引用直接用 `auto &`。

## To Do 对应关系

Microsoft To Do 中的每个模板任务，对应一个可直接复制的 `.cpp` 片段。
完成一个模板的标准是：代码整理、接口说明、验证题和一次编译/运行验证都完成。

全部 145 个文件的原题 / 公开验证题见 [题源索引](verify/SOURCES.md)；实际验证范围和使用限制见 [审查报告](verify/AUDIT.md)。题目链接不等于整题通过记录；李超树已修复整数线段数学同值的编号判定，浮点调用仍受舍入误差影响。

## 调用方式

这些文件不依赖自定义头文件，均为无 `main()` 的算法片段。

`support/tools/contest_main.cpp` 保存你的比赛起手式，`support/verify/` 和对拍工具也可以有 `main()`。

计算几何按点与向量、直线与线段、多边形、凸包、圆等拆开，原函数接口不变。先复制 `点与向量.cpp`，再按文件头注明的依赖复制所需部分；基础定义只需一份。原凸多边形切割示例移至 `support/verify/geometry_cut_submission.cpp`；拆分前的两份完整文件备份在 `build/geometry-before-split-20261010/`。

新增的三维凸包独立使用 `P3`，无需复制二维基础定义。`极大周期子串.cpp` 先复制后缀数组，`上下界最小费用流.cpp` 先复制常规费用流；依赖均写在各文件头。

`等距拉格朗日插值.cpp` 先复制组合数，`两类斯特林数.cpp` 先复制 NTT，`线性递推快速求项.cpp` 先复制 FPS；`最小面积包围矩形.cpp` 只依赖点向量定义，输入需先取得严格逆时针凸包。

## 打印模板册

从 `.cpp` 原文生成 A4 竖版单栏 PDF，保留代码空格和缩进。视觉风格参考 [Calculatelove 的竖版笔记](https://calculatelove.github.io/cp-library-v4-LaTeX/)：左右 20 mm 页边距、单栏目录、苹方标题、浅灰圆角代码卡片、蓝灰／绿色词法高亮、无横线页眉和居中页码。不显示代码行号，不额外插入接口说明或使用提示。

代码基准为 8 pt、行距 10.5 pt。短代码优先完整放在同一页，必要时略微缩小；放不完整且剩余不足半页时，标题和代码一起换页；长代码在剩余至少半页时才接排，并在 7.5–8.5 pt 内按实际行宽与页数自动选择字号，行距同比例调整。先减少页数，再在这些页内尽量放大，使末页靠近页底；如果另起一页不增加该份代码所需的新增页数，并能让末页收在页底附近，则优先这样放置。同一份代码跨页保持同一字号和行距。若字号上限或行宽限制使末页仍有空白，后续代码可接排，不把字强行放大或拉开行距。函数边界由 Tree-sitter C++ 识别：能放入单页的函数保持完整，紧邻注释和模板声明尽量随函数移动。单个函数在 7.5 pt 下仍超过一页时，才允许内部分页，并保留能整页容纳的语句块；解析失败会停止构建。分页前运行 `python support/verify/function_pagination_check.py`，整册检查也会验证实际换页位置。正文不删空行、不自动折行、不改变源码。

分页采用先测量、再决定整体放置或拆分的方式，对应 [ReportLab 的 `KeepTogether` / `CondPageBreak`](https://docs.reportlab.com/reportlab/userguide/ch9_other_useful_flowables/) 和 [tcolorbox 的最小首段约束](https://github.com/T-F-S/tcolorbox/blob/master/doc/latex/tcolorbox/tcolorbox.doc.breakable.tex)。当前生成器直接绘制 Canvas，因此在绘制前执行约束；“半页”是本册为避免页尾短开头选择的参数，并非上述工具的默认值。

```text
python -m pip install -r requirements-print.txt
python support/tools/generate_print.py
python support/verify/print_pagination_check.py
python support/verify/print_book_check.py
```

输出为 `output/pdf/template.pdf`，默认生成成功后同步更新根目录的 `template.pdf`；旁边的 `.layout.json` 用于检查逐行完整性，不需要打印。更新源码后重新生成即可，不手工维护第二份代码。通过 `--output` 导出其他文件时不覆盖根目录的正式打印版。

册首收录 `support/tools/linux_commands.txt` 中的 Linux 竞赛常用命令，包括编译、调试、输入输出重定向、栈大小、测时、限时运行和答案比较。

字体分别为：正文中文宋体、正文西文 Latin Modern Roman、数学 Latin Modern Math、标题苹方、代码与中文注释 Maple Mono NF CN。先按 [字体准备说明](fonts/README.md) 放置字体文件；缺少字体时停止生成，不使用替代字体。使用到的字体嵌入 PDF；源码注释中的公式仍为代码原文。新增模板时也需更新生成脚本的 `CHAPTERS`，漏收文件会直接报错。

打印选择 A4、竖向、100% 原尺寸；双面选择长边翻转。

排版主要参考 [Calculatelove/cp-library-v4-LaTeX](https://github.com/Calculatelove/cp-library-v4-LaTeX) 的竖版字体、目录和代码卡片；保持本库的算法内容、章节编号和无行号规则，不引入另一份代码或 LaTeX 构建链。

## OEIS 检索附册

OEIS 独立于算法模板册，不保留常用数列详解。使用 [Dusker233/OEIS-PrintMirror 的 2025-07-11 快照](https://github.com/Dusker233/OEIS-PrintMirror/releases/tag/Daily_Update_2025-07-11)，实测正文收录 31,120 条不同数列，含简介共 975 页。直接保留原版 A4 竖向双栏排版、数列和公式，不重新抄录或改写。

```text
python -m pip install -r requirements-print.txt
python support/tools/prepare_oeis.py
```

输出为原版 `output/pdf/oeis.pdf` 和最终打印版 `output/pdf/oeis-2up.pdf`，完整校验后同步更新根目录的 `oeis-2up.pdf`。首次运行下载固定版本，校验 SHA-256、页数和 A4 页面尺寸后才保存；已有原版时离线校验并生成 2 合 1 版，不访问网络。打印版将连续两页左右并排，每个原页面外侧添加细灰色虚线框，保留原文和矢量文字，并逐页核对文字完整性。PDF 属于生成资料，不提交到 Git。

正文按去掉前导 0、1 后的数值字典序排列，下划线标出检索前缀的第一项。先打表得到若干项，再按前缀定位，用定义、公式和后续项确认；不能只凭前几项相同认定规律。

这是有公式、非负、单调不下降数列的筛选镜像，并非 OEIS 全库；上游还过滤重复项过多或数字过长的序列，最多保留前 25 项。数据范围截至 A385846，快照日期不是本项目的生成日期。镜像仓库已于 2025-08-07 归档，不会自动更新。

原版由 Kaiqi Wang（Sun Yat-sen University）制作，见 [africamonkey/OEIS-Spider](https://github.com/africamonkey/OEIS-Spider)；后续镜像由 Dusker233 维护。数列数据和公式来自 [OEIS](https://oeis.org/)，按 [OEIS 的 CC BY-SA 4.0 许可](https://oeis.org/LICENSE) 使用，不改变其他代码的许可。

最终打印文件固定放在项目根目录：`template.pdf` 和 `oeis-2up.pdf`。之后分别运行上面的两个生成脚本，会同步覆盖对应的根目录文件；桌面旧副本不再同步更新。

附册打印选择根目录的 `oeis-2up.pdf`：共 488 页，A4、横向、100% 原尺寸、每面 1 页，双面选择短边翻转，共需 244 张纸。文件已经完成 2 合 1 拼版，不要在打印设置里再次多页缩印；最后一页右半留空。原版 `output/pdf/oeis.pdf` 仍保留，不作为此次最终打印文件。

## 对拍工具

`support/tools/stress/` 提供 `gen.cpp`、`sol.cpp`、`bf.cpp`、`stress.cpp`。打印册分别收录 Linux 对拍、Windows 对拍和随机生成器；`sol.cpp`、`bf.cpp` 两份占位示例不印入册。按平台将 `stress_linux.cpp` 或 `stress_windows.cpp` 复制为 `stress.cpp`，也可直接使用自动区分平台的通用版。复制到专用题目目录，Linux 下运行：

```text
g++ -std=c++17 -O2 gen.cpp -o gen
g++ -std=c++17 -O2 sol.cpp -o sol
g++ -std=c++17 -O2 bf.cpp -o bf
g++ -std=c++17 -O2 stress.cpp -o stress
./stress
```

对拍只依赖 C++17 和系统重定向，不需要 Python、`diff` 或 `fc`。程序以轮数为种子，依次运行生成器、待测程序和暴力，自己逐 token 比较，忽略空白和换行差异。每 100 轮输出一次 `AC`；答案不同输出 `WA seed=...`，运行或文件错误输出 `RE` 后停止，保留 `in.txt`、`out.txt`、`ans.txt`。再次运行会覆盖文件；没有内置限时，浮点与多解题需要修改比较逻辑。

Windows 使用 MinGW g++，输出文件名加 `.exe`，PowerShell 运行 `.\stress.exe`。编译、种子复现和限制见 [对拍说明](tools/stress/README.md)。原有 6 组题型示例和 Python / Bash 工具保留，不是这套纯 C++ 对拍的依赖。

## 回归验证

`support/verify/` 包含集成烟测和随机测试。编译使用 `g++ -std=c++17 -O2 -Wall -Wextra -pedantic`，运行生成的可执行文件；构建产物放在 `build/`。

正式验证在 Linux 下运行：

```text
bash support/verify/run_linux.sh
```

脚本将栈设为 256 MiB，先检查所有模板和验证程序的语法，再运行无输入测试，启用 ASan / UBSan 和标准库断言。`deep_stack.cpp` 验证 20 万节点深链、边双大环和 20 万条边的欧拉路递归，需要上述栈设置。

- `snippets_smoke.cpp`：公开接口烟测；
- `common_templates_random.cpp`、`template_regressions.cpp`：常用模板随机测试和已知边界回归；
- `dsu_on_tree_random.cpp`、`geometry_random.cpp`、`li_chao_random.cpp`、`suffix_array_random.cpp`：专项随机测试；
- `graph_random.cpp`：LCA、SCC、双连通分量、欧拉路、匹配、差分约束和可撤销并查集；
- `flow_random.cpp`：三种网络流与小规模穷举对拍，覆盖自环、重边和负环；
- `remaining_templates_random.cpp`：递推、变换、组合数、筛法、其他数据结构和字符串。
- `new_ds_random.cpp`、`new_tree_random.cpp`：带权并查集、主席树、Treap、01 Trie、树链剖分、虚树、点分治和 2-SAT；
- `new_flow_random.cpp`：上下界流、KM 和 Hopcroft-Karp；
- `new_math_random.cpp`、`new_poly_random.cpp`：数论、消元、FPS、FFT 和任意模数卷积；
- `new_string_random.cpp`、`new_offline_random.cpp`、`new_geometry_random.cpp`：字符串、离线查询和几何补充。

`mcmf_submission_standalone.cpp` 是需要题目输入的独立验证入口，不是无输入随机测试。

新增模板另有四组 `extra_*_random.cpp`，覆盖随机暴力对拍和数值边界；公开题验证使用 Library Checker 官方数据。题目对应、验证范围和复跑命令见 [验证说明](verify/README.md)。

`more_ds_random.cpp`、`more_graph_random.cpp` 覆盖吉司机线段树、可持久化数组、点分树和斯坦纳树的随机暴力对拍、重初始化与边界回归。

前批新增的 14 个片段由 `batch_math_random.cpp`、`batch_string_random.cpp` 和各专项 `*_random.cpp` 验证；包含匹配穷举、流量穷举、字符串穷举、几何独立真值和历史版本分叉。算法参考与改写来源见 [参考资料](verify/REFERENCES.md)。

常用补充由 `completion_math_random.cpp`、`completion_structures_random.cpp` 和 `min_rectangle_random.cpp` 验证，覆盖等距插值、线段树搜索、LIS、整除变换、函数图、斯特林数、二维哈希、包围矩形、区间集合、SG 和 Bostan–Mori；递归深链另加入 `deep_stack.cpp`。

## 接口索引

可能失败的接口使用 `bool` 加输出参数；只在返回 `true` 时使用输出。
逆元、模平方根和斯坦纳树的合法答案均非负，因此用 `-1` 表示失败。
模板不使用 `optional`、异常、具名类型转换或类型萃取；李超树整数和浮点插入分别调用。
比赛模板不使用 `tie` 元组比较、`move`；保留 lambda。比赛入口默认单测，多测时打开 `cin >> t`。
比赛模板和入口不使用 `int128`，整数默认使用 `int` / `long long`，直接用 `cout` 输出。
所有整数中间量也必须满足源码头部的范围，不能只看最终答案；验证程序可用宽整数作独立真值，不印入册。
快速幂与逆元要求 `mod <= INT_MAX`，杜教筛要求 `n <= 4e9`；李超树、斜率优化和费用流须检查中间乘积。
CRT 与 Pollard–Rho 保留大模数范围，使用倍增模乘；Pollard–Rho 每次模乘为 `O(log n)`，较大半素数需关注时限。
本轮逐项修改与复验见 [竞赛接口审计](verify/AUDIT_CONTEST_INTERFACES_20261012.md)。
打印前的使用陷阱与处理记录见 [最终易用性审查](verify/AUDIT_PRINT_USABILITY_20261013.md)。

| 文件 | 对外接口 |
| --- | --- |
| `template/math/等距拉格朗日插值.cpp` | `LagrangeEq::build`、`query`；连续整数采样的线性时间求值，依赖 `组合数.cpp` |
| `template/others/最长上升子序列与方案.cpp` | `lis(a, strict)`；1-based 数组，返回严格递增 / 非递减子序列的下标方案 |
| `template/math/约数与倍数变换与卷积.cpp` | `divisor_zeta`、`multiple_zeta`、`gcd_convolution`、`lcm_convolution`；支持合数模数 |
| `template/graph/函数图分解与跳跃.cpp` | `FunctionalGraph::build`、`jump`、`distance`；每点恰有一条出边，保存环与入环信息 |
| `template/math/两类斯特林数.cpp` | `stirling_first`、`stirling_second`、`stirling_second_row`；无符号第一类、第二类与 NTT 求固定一行 |
| `template/string/二维哈希.cpp` | `Hash2D::build`、`query`；字节矩阵双模哈希，0-based 闭矩形 |
| `template/geometry/最小面积包围矩形.cpp` | `min_rectangle(hull)`；严格逆时针凸包，返回面积与四角，依赖 `点与向量.cpp` |
| `template/ds/区间集合与最小未出现数.cpp` | `IntervalSet::insert`、`erase`、`contains`、`mex`、`clear`；维护整数半开区间的并 |
| `template/others/博弈状态函数.cpp` | `grundy(g, sg)`；写入 SG 值，有向环返回 `false` |
| `template/math/线性递推快速求项.cpp` | `bostan_mori`、`linear_kth`；有理生成函数系数与大阶递推，依赖 `多项式基本运算.cpp` |
| `template/ds/可持久化二进制字典树.cpp` | `PersistentXorTrie::init`、`insert`、`max_xor(left, right, x, ans)`；空差集返回 `false` |
| `template/ds/二维最近邻.cpp` | `KDTree::build`、`nearest(x, y, ans, exclude)`、`query`；最近邻为空返回 `false` |
| `template/graph/第几短游走.cpp` | `KShortestWalk::init`、`add_edge`、`query(s, t, k)`；非负边权，按边序列区分游走 |
| `template/graph/上下界最小费用流.cpp` | `LRCostFlow::init`、`add_edge`、`add_supply`、`solve`；上下界与点的净供给，支持负费用与负环 |
| `template/graph/支配树.cpp` | `Dominator::init`、`add_edge`、`build(root)`；`idom[root] = root`，不可达为 0 |
| `template/graph/一般图最大权匹配.cpp` | `WeightedBlossom::init`、`add_edge`、`solve`；一般图最大总权匹配，`match` 保存方案 |
| `template/graph/最大团与最大独立集.cpp` | `MaxClique::init`、`add_edge`、`max_clique`、`max_independent_set`；返回精确最优顶点集合 |
| `template/math/积性函数求和.cpp` | `Min25::init`、`sum`、`count_primes`；素数处至多二次多项式，传入素数幂回调 |
| `template/math/单纯形与线性规划.cpp` | `Simplex::solve(A, b, c)`；最大化 `c*x`，约束 `A*x <= b`、`x >= 0` |
| `template/string/林登分解.cpp` | `lyndon_factorization(s)`；返回含 0 和 n 的分界点，0-based 半开区间 |
| `template/string/极大周期子串.cpp` | `enumerate_runs(s)`；返回 `{最小周期, l, r}`，依赖 `后缀数组.cpp` |
| `template/geometry/圆面积并与周长并.cpp` | `circle_union(cs)`；返回圆并的面积与周长，依赖 `点与向量.cpp`、`直线与线段.cpp`、`圆.cpp` |
| `template/geometry/三维凸包.cpp` | `P3`、`Hull3D::build`、`area`、`volume`；`dim` 为维度，`faces` 为外向三角面 |
| `template/others/舞蹈链精确覆盖.cpp` | `DLX::init`、`add_row`、`solve(rows)`；写入精确覆盖行编号，无解返回 `false` |
| `template/ds/吉司机线段树.cpp` | `Beats::init`、`add`、`chmin`、`chmax`、`query`、`query_min`、`query_max`；区间加、截断、和与最值 |
| `template/ds/可持久化数组.cpp` | `PArray::init`、`modify(root, pos, value)`、`query(root, pos)`；`root` 为初始根，修改返回新根 |
| `template/graph/点分树.cpp` | `CentroidTree::init`、`add_edge`、`build`、`modify(u, delta)`、`query(u, radius)`；动态点权的距离范围和 |
| `template/graph/最小斯坦纳树.cpp` | `Steiner::init`、`add_edge`、`solve(terminals)`；最小连接代价，不可连通返回 `-1` |
| `template/math/子集卷积.cpp` | `subset_convolution(a, b, mod)`；0-based 位掩码，支持合数模数 |
| `template/math/多项式多点求值与快速插值.cpp` | `Multipoint::build`、`evaluate`、`interpolate`；依赖 `多项式基本运算.cpp`，固定模数 `998244353` |
| `template/graph/重构树.cpp` | `KruskalTree::add_edge`、`build(descending)`、`lca`、`query(u, v, ans)`；不连通返回 `false` |
| `template/graph/圆方树.cpp` | `block_cut_tree(n, blocks)`；输入点双顶点列表，返回广义圆方森林 |
| `template/graph/边双缩点树.cpp` | `bridge_tree(cnt, bel, edges)`；返回保留原桥编号的缩点森林 |
| `template/graph/长链剖分与祖先查询.cpp` | `KthAncestor::add_edge`、`build(root)`、`query(u, k)`；长链剖分，查询 `O(1)` |
| `template/graph/最小割树.cpp` | `GomoryHu::add_edge`、`build`、`query`；依赖 `最大流.cpp`，无向非负容量 |
| `template/ds/李超线段树.cpp` | `LiChaoTree::add_line`、`add_seg`；浮点用 `add_line_real`、`add_seg_real`；`query_id`、浮点 `query`；仅整数直线用 `query_int(x, ans)` |
| `template/ds/左偏树与可并堆.cpp` | `LeftistHeap::init`、`push`、`merge`、`top`、`pop`；合并与删除返回新根 |
| `template/others/单调栈与左右最近更小与更大.cpp` | `monotonic_bounds(a, greater = false)`；左右最近严格更小 / 更大元素 |
| `template/others/单调凸包与斜率优化.cpp` | `CHT::add`、`query`、`clear`；斜率不增、横坐标不减、求最小值 |
| `template/math/快速幂与逆元.cpp` | `pow_mod`、`linear_inverse`、`batch_inverse`；`mod <= INT_MAX`，逆元要求质数模数 |
| `template/math/矩阵求逆.cpp` | `matrix_inv(a, mod, b)`；写入逆矩阵，不可逆返回 `false` |
| `template/math/原根.cpp` | `primitive_root`；质数模数，依赖 `素性测试与整数分解.cpp` |
| `template/ds/并查集.cpp` | `DSU::init`、`find`、`merge`、`same`、`size`，`cnt` 为连通块数 |
| `template/ds/树状数组.cpp` | `Fenwick::init`、`add`、`query`、`kth`；`kth` 要求原数组非负 |
| `template/ds/可撤销并查集.cpp` | `RollbackDSU::merge`、`snapshot`、`rollback` |
| `template/ds/线段树与树上二分.cpp` | `SegTree::init`、`modify`、`query`、`first_ge`、`last_ge`、`kth`；`kth` 要求所有元素非负 |
| `template/geometry/点与向量.cpp` | 原版 `P`、`db`、`EPS`、`sign`、`cmp`、`rad`、`polar_cmpX`、`polar_cmpY` |
| `template/geometry/直线与线段.cpp` | `Line`、`isLL`、`isSS`、`isLS`、`onSeg`、`proj`、`reflect`、`nearest`、`disSS` 等 |
| `template/geometry/多边形.cpp` | `area`、`contain`、`convexCut` |
| `template/geometry/凸包与直径与闵可夫斯基和.cpp` | `convexHull`、`convexHullNonStrict`、`checkConvexHull`（简单多边形的逆时针凸性）、`convexDiameter`（顺逆时针均可）、`MincowskySum` |
| `template/graph/二分图匹配.cpp` | `Kuhn::add_edge`、`solve` |
| `template/graph/桥.cpp` | `Bridge::add_edge`、`find` |
| `template/graph/割点.cpp` | `CutVertex::add_edge`、`find` |
| `template/graph/差分约束.cpp` | `Diff::add`、`solve`；全 0 初始化，负环返回 false；使用 ll，松弛中间量不得溢出 |
| `template/graph/堆优化最短路.cpp` | `dijkstra` |
| `template/graph/树上启发式合并.cpp` | `DSUOnTree::add_edge`、`build(root)`；按题意补全 `add`、`del`、`query` |
| `template/graph/有向图欧拉路.cpp` | `euler_d` |
| `template/graph/无向图欧拉路.cpp` | `euler_u` |
| `template/graph/最小生成树.cpp` | `Edge`、`MST`、`kruskal`；结果为 `sum`、`ids`、`ok` |
| `template/graph/倍增最近公共祖先.cpp` | `LCA::add_edge`、`build`、`lca` |
| `template/graph/最大流.cpp` | `Dinic::add_edge`、`flow` |
| `template/graph/势能最短路费用流.cpp` | `MCMF::add_edge`、`flow`；无负环常规题 |
| `template/graph/容量缩放费用流.cpp` | `ScalingMCMF::add_edge`、`flow`；UOJ 487 / Capacity Scaling |
| `template/graph/强连通分量.cpp` | `SCC::add_edge`、`build`，`bel` 保存分量编号 |
| `template/graph/点双连通分量.cpp` | `VBCC::add_edge`、`build`、`bcc`（每项的 `v` 为顶点，`e` 为边编号） |
| `template/graph/边双连通分量.cpp` | `EBCC::add_edge`、`build`、`bcc`（每项含 `v`、`e`）、`bel`、`is_bridge` |
| `template/math/自适应辛普森积分.cpp` | `simpson` |
| `template/math/组合数.cpp` | `Comb::init`、`choose` |
| `template/math/快速沃尔什变换.cpp` | `fwt_or`、`fwt_and`、`fwt_xor` 及逆变换 |
| `template/math/快速数论变换与多项式乘法.cpp` | `ntt`、`multiply` |
| `template/math/线性筛素数.cpp` | `primes` |
| `template/math/线性筛积性函数.cpp` | `Sieve::build`，`prime`、`lp`、`phi`、`mu` |
| `template/math/整除分块.cpp` | `div_blocks` |
| `template/math/异或线性基.cpp` | `Basis::insert`、`contains`、`max_xor`、`kth(k, ans)`；0-based，包含 0，越界返回 `false` |
| `template/others/笛卡尔树.cpp` | `Cartesian::build` |
| `template/others/莫队.cpp` | `Mo::Query`、`Mo::solve(n, qs)`；按题意补全 `add`、`del`、`answer` |
| `template/others/滑动窗口最值.cpp` | `window_min`、`window_max` |
| `template/others/稀疏表.cpp` | `ST::build`、`query` |
| `template/others/常用定义.cpp` | `chkmin`、`chkmax`、`add`、`sub` 及常用宏、类型别名 |
| `template/others/三分搜索.cpp` | `ternary_min`、`ternary_max` |
| `template/string/多模式匹配自动机.cpp` | `AC::insert`、`build`、`query` |
| `template/string/字符串匹配.cpp` | `get_pi`、`kmp` |
| `template/string/最长回文子串.cpp` | `Manacher::build`、`query`、`longest` |
| `template/string/后缀自动机.cpp` | `SAM::build`、`count`、`query` |
| `template/string/字典树.cpp` | `Trie::insert`、`prefix`、`count` |
| `template/others/离散化.cpp` | `discretize(a, key)`；返回 1-based 编号，`key` 输出排序去重后的值，恢复为 `key[id[i] - 1]` |
| `template/string/后缀数组.cpp` | `SA::build`、`lcp` |
| `template/graph/欧拉序最近公共祖先.cpp` | `EulerLCA::add_edge`、`build`、`lca` |
| `template/graph/换根动态规划与距离和.cpp` | `reroot` |
| `template/graph/树的直径.cpp` | `tree_diameter` |
| `template/graph/拓扑排序.cpp` | `topo(g, order)`；写入拓扑序，有环返回 `false` |
| `template/others/树哈希.cpp` | `tree_hash(g, root = 1)` |
| `template/math/矩阵快速幂.cpp` | `matrix_mul`、`matrix_pow` |
| `template/math/拉格朗日插值.cpp` | `lagrange` |
| `template/math/最短线性递推.cpp` | `bm`、`kth` |
| `template/ds/带权与异或并查集.cpp` | `WDSU`、`XDSU`：`init`、`find`、`merge`、`query(u, v, ans)`；不连通返回 `false` |
| `template/ds/主席树.cpp` | `PST::build`、`kth`、`count` |
| `template/ds/无旋平衡树.cpp` | `Treap::insert`、`erase`、`rank`、`kth(k, ans)`、`pre(x, ans)`、`suf(x, ans)`；`SeqTreap::insert`、`erase`、`reverse`、`query`、`at` |
| `template/ds/二进制字典树.cpp` | `XorTrie::init`、`insert`、`erase`、`max_xor(x, ans)`；空集返回 `false` |
| `template/graph/树链剖分.cpp` | `HLD::add_edge`、`build`、`lca`、`path`；子树区间为 `[in[u], out[u]]` |
| `template/graph/二元可满足性.cpp` | `SAT::add`、`force`、`solve`，`ans` 保存布尔解 |
| `template/graph/虚树.cpp` | `VTree::add_edge`、`build`、`query`；`nodes` 为虚树节点，`t` 为带距离的虚树边 |
| `template/graph/点分治.cpp` | `Centroid::add_edge`、`query(k)`，统计距离不超过 `k` 的无序点对 |
| `template/graph/上下界网络流.cpp` | `LRFlow::add_edge`、`solve`；无参为可行循环流，`solve(s, t, mode)` 求可行 / 最大 / 最小流，`f` 保存各边流量 |
| `template/graph/二分图最大权匹配.cpp` | `KM::add_edge`、`solve`；最大权左侧完全匹配，`ml` / `mr` 为匹配 |
| `template/graph/分层增广二分图匹配.cpp` | `HK::add_edge`、`solve`、`cover`；`cover` 在最大匹配后返回最小点覆盖 |
| `template/math/扩展欧几里得与中国剩余定理.cpp` | `exgcd`、`inv`（无逆元返回 `-1`）、`crt(r, m, ans, mod)`（无解或 lcm 超出 ll 返回 `false`） |
| `template/math/离散对数.cpp` | `bsgs`、`exbsgs`，返回最小非负指数或 `-1` |
| `template/math/卢卡斯定理.cpp` | `Lucas::init`、`choose` |
| `template/math/素性测试与整数分解.cpp` | `is_prime`、`factor`，支持 64 位无符号整数 |
| `template/math/高斯消元.cpp` | `gauss`、`gauss_mod`、`gauss_xor`；返回 `0` 无解 / `1` 唯一解 / `2` 多解 |
| `template/math/多项式基本运算.cpp` | `FPS::mul`、`inv`、`ln`、`exp`、`pow`、`divmod`；`sqrt(a, n, ans)` 无解返回 `false`，模数 `998244353` |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | `fft`、`fft_mul`、`conv_mod`，后者为任意正 `int` 模数的精确卷积 |
| `template/string/扩展字符串匹配.cpp` | `get_z`、`exkmp`，字符串位置为 0-based |
| `template/string/字符串哈希.cpp` | `Hash::build`、`query`、`lcp`，双模哈希 |
| `template/string/回文自动机.cpp` | `PAM::init`、`insert`、`build`、`distinct`、`count` |
| `template/string/最小表示法.cpp` | `min_rotation`，返回最小循环表示的起始位置 |
| `template/others/分治三维偏序.cpp` | `dominance`，统计每点被其他点三维支配的数量 |
| `template/others/整体二分与动态区间第几小.cpp` | `KthOffline::init`、`modify`、`query`、`solve`，离线求带修改区间第 k 小 |
| `template/geometry/圆.cpp` | `Circle`、`isCL`、`isCC`、`tangent_points`、`common_tangents`、`circle_area`、`circumcircle(a, b, c, ans)`、`diameter_circle` |
| `template/geometry/半平面交.cpp` | `half_plane` |
| `template/geometry/最小圆覆盖.cpp` | `min_circle` |
| `template/geometry/平面最近点对.cpp` | `ClosestPair::build`，返回原下标，`d2` 为最短距离平方 |
| `template/geometry/凸多边形包含查询.cpp` | `ConvexQuery::build`、`query`，返回 0 外部 / 1 边界 / 2 内部 |
| `template/geometry/矩形面积并与周长并.cpp` | `RectUnion::solve`，返回矩形并的面积与周长 |
| `template/ds/仿射线段树.cpp` | `AffineSegTree::init`、`modify`、`query`；修改为乘后加 |
| `template/ds/二维树状数组.cpp` | `Fenwick2D::build`、`add`、`query`，离线收集更新坐标，查询闭矩形 |
| `template/ds/动态开点线段树合并与分裂.cpp` | `MergeSegTree::init`、`add`、`query`、`kth`、`merge`、`split`；合并消耗源根 |
| `template/ds/动态树.cpp` | `LCT::init`、`same`、`link`、`cut`、`modify`、`query`；维护点权路径和 |
| `template/graph/离线动态连通性.cpp` | `DynConn::init`、`add`、`erase`、`query`、`solve` |
| `template/graph/零一最短路.cpp` | `bfs01`，边权只能为 0 或 1 |
| `template/graph/全源最短路.cpp` | `floyd`，原地更新距离矩阵，返回是否无负环 |
| `template/graph/重赋权全源最短路.cpp` | `Johnson::init`、`add_edge`、`solve(dis)`；写入距离矩阵，存在负环返回 `false` |
| `template/graph/全局最小割.cpp` | `MinCut::init`、`add_edge`、`solve`，返回割权与一侧顶点 |
| `template/graph/最小树形图.cpp` | `DMST::init`、`add_edge`、`solve(root, ans)`；写入最小权，不可达返回 `false` |
| `template/graph/一般图匹配.cpp` | `Blossom::init`、`add_edge`、`solve`，`match` 保存匹配 |
| `template/math/类欧几里得.cpp` | `floor_sum`，返回 `ll`，支持有符号参数，所有中间量须放入 `ll` |
| `template/math/二次剩余.cpp` | `sqrt_mod`；质数模数，不存在平方根返回 `-1` |
| `template/math/杜教筛.cpp` | `DuJiao::init`、`phi`、`mu`；前缀和返回 `ll`，`n <= 4e9` |
| `template/math/扩展卢卡斯定理.cpp` | `ExLucas::init`、`choose`，任意正 `int` 模数，预处理规模受质数幂大小限制 |
| `template/math/行列式与矩阵树定理.cpp` | `det`、`matrix_tree`，质数模数；有向版本为从根向外的树 |
| `template/others/带修改莫队.cpp` | `MoModify::init`、`modify`、`query`、`solve()`；补全 `add`、`del`、`answer`，按值增删 |
| `template/others/树上莫队.cpp` | `MoTree::init`、`add_edge`、`build`、`query`、`solve()`；补全 `add`、`del`、`answer`，按点增删 |
| `template/others/分治动态规划与单调队列多重背包.cpp` | `dc_dp`、`bounded_knapsack`，分别为单层分治转移与一种物品的多重背包转移 |
| `template/string/位集优化最长公共子序列.cpp` | `bitset_lcs`，只求 LCS 长度，支持任意字节 |

## 约定

- 图和树模板通常使用 1-based 顶点编号；具体例外写在文件注释中。
- `template/math/快速数论变换与多项式乘法.cpp` 使用模数 `998244353`。
- `template/math/拉格朗日插值.cpp` 要求横坐标在模数意义下互异且模数为质数。
- `template/graph/树的直径.cpp` 的边权默认为非负数。
- `template/graph/换根动态规划与距离和.cpp` 是“所有点到所有点距离和”的换根 DP 实例；其他换根转移需按题目修改。
- 莫队与 DSU on Tree 不使用多回调接口；使用前在结构体中加入题目状态，并补全声明的 `add`、`del`、`answer/query` 函数定义。这些函数没有默认空实现，未补全时不能链接运行。
- `template/graph/树上启发式合并.cpp` 的 `add` 与 `del` 必须互为逆操作；`query(u)` 记录子树答案，`build(root)` 结束后统计状态恢复为空。
- 三种莫队每次 `solve` 前须清空题目统计状态；带修改莫队按值增删，普通莫队按下标增删，树上莫队按顶点增删。
- `template/others/树哈希.cpp` 是随机化哈希，不能当作严格同构判定。
- 李超树的整数参数使用有理数精确比较，交叉乘积等全部中间量须放入 `ll`；所有横坐标在 `[0, 40000]` 且 `|k|`、`|b|`、`|y| <= 10^9` 是充分范围。浮点参数仍为浮点比较，不要混用后仍要求精确判等。
- 李超树按整个密集区间分配约 `4 * (R - L + 1)` 个 `int`，不是动态开点或坐标压缩；坐标范围必须能存下。`query` 是浮点近似值；整数 DP 只用 `add_line`，并在 `query_int(x, ans)` 返回 `true` 后读取 `ll` 答案。
- 莫队和树上启发式合并的结构体内给出不同值 / 颜色数的填写示例；先用示例成员替换声明，再按题意修改状态、增删和答案。样例中的值域必须先离散化；带修改莫队还要包含所有将来修改的值。
- `CHT` 仅用于斜率不增、查询横坐标不减的最小值问题；不满足单调性时使用李超树等结构。

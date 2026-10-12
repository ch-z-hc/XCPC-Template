# XCPC Template

## 目录结构

- `template/`：可直接复制的 C++17 算法模板，按数据结构、图论、数学等分类；
- `support/`：个人码风、验证程序和开发工具；
- `output/pdf/`：自动生成的打印册；
- 根目录：最终打印用的 `template.pdf`、`oeis-2up.pdf`，以及打印依赖和项目说明等配置。

## 约定

- 算法片段放在分类目录中，默认不包含 `int main()`。
- `int main()` 只保留在比赛入口、验证程序或明确的工具程序中。
- 每个片段应在开头注明适用条件、复杂度、下标约定和调用方式。
- DFS 使用递归，不使用显式栈模拟或遍历序倒推；算法本身需要的 BFS 和栈不受影响。
- 对拍程序属于验证工具，可以保留自己的 `main()`，但不要混入算法片段。

## 个人码风

码风规范保存在 `support/skills/chzhc-xcpc-style/SKILL.md`，覆盖命名、空格、适度压行和线段树 A 写法。只约束竞赛代码，不影响其他项目；计算几何以 Git `8f2e476` 的 `P` / `db` / `EPS` 原版为基础，将 `vector` 写为 `V`、移除 `debug` 宏，并按本次审查修复排序、宏参数与凸包方向问题，不参与统一码风重写。

## To Do 对应关系

Microsoft To Do 中的每个模板任务，对应一个可直接复制的 `.cpp` 片段。
完成一个模板的标准是：代码整理、接口说明、验证题和一次编译/运行验证都完成。

全部 145 个文件的原题 / 公开验证题见 [题源索引](support/verify/SOURCES.md)；实际验证范围和使用限制见 [审查报告](support/verify/AUDIT.md)。题目链接不等于整题通过记录；李超树已修复整数线段数学同值的编号判定，浮点调用仍受舍入误差影响。

## 调用方式

这些文件不依赖自定义头文件，均为无 `main()` 的算法片段。

`support/tools/contest_main.cpp` 保存你的比赛起手式，`support/verify/` 和对拍工具也可以有 `main()`。

计算几何按点与向量、直线与线段、多边形、凸包、圆等拆开，原函数接口不变。先复制 `Point.cpp`，再按文件头注明的依赖复制所需部分；基础定义只需一份。原凸多边形切割示例移至 `support/verify/geometry_cut_submission.cpp`；拆分前的两份完整文件备份在 `build/geometry-before-split-20261010/`。

新增的三维凸包独立使用 `P3`，无需复制二维基础定义。`Runs.cpp` 先复制后缀数组，`LowerBoundMinCostFlow.cpp` 先复制常规费用流；依赖均写在各文件头。

`EquidistantLagrange.cpp` 先复制组合数，`Stirling.cpp` 先复制 NTT，`BostanMori.cpp` 先复制 FPS；`MinRectangle.cpp` 只依赖点向量定义，输入需先取得严格逆时针凸包。

## 打印模板册

从 `.cpp` 原文生成 A4 竖版单栏 PDF，保留代码空格和缩进。正文代码为 9.5 pt，代码以外的中文统一用华文新魏，标题英文用 Times Bold，包含目录、PDF 书签和页码，不显示代码行号，不额外插入接口说明或使用提示。

```text
python -m pip install -r requirements-print.txt
python support/tools/generate_print.py
python support/verify/print_book_check.py
```

输出为 `output/pdf/template.pdf`，默认生成成功后同步更新根目录的 `template.pdf`；旁边的 `.layout.json` 用于检查逐行完整性，不需要打印。更新源码后重新生成即可，不手工维护第二份代码。通过 `--output` 导出其他文件时不覆盖根目录的正式打印版。

册首收录 `support/tools/linux_commands.txt` 中的 Linux 竞赛常用命令，包括编译、调试、输入输出重定向、栈大小、测时、限时运行和答案比较。

默认使用 Windows 的 Consolas 和宋体，字体嵌入 PDF。其他系统通过 `--code-font` 和 `--chinese-font` 指定支持相应字符的 TrueType 字体（中文 TTC 默认第 0 个字体）。新增模板时也需更新生成脚本的 `CHAPTERS`，漏收文件会直接报错。

打印选择 A4、竖向、100% 原尺寸；双面选择长边翻转。

排版参考：[justiceHui 2024 Astana](https://github.com/justiceHui/icpc-teamnote/blob/master/pdf/240919-icpc-world-finals-astana.pdf)、[tatyam 2026](https://github.com/tatyam-prime/ICPC_notebook/blob/main/notebook.pdf)、[brant 2026](https://github.com/brantwcw/codebook_generator/blob/main/codebook.pdf)、[KACTL](https://github.com/kth-competitive-programming/kactl)。参考标题层次、紧凑页眉和连续分页，不照搬为页数限制设计的小字号多栏。

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

`support/tools/stress/stress.cpp` 是 C++ 对拍程序，已收录到打印册的 `template` 章节。将它复制到题目目录，同目录准备 `gen.cpp`、`brute.cpp`、`a.cpp`，Linux 下运行：

```text
g++ -std=c++17 -O2 gen.cpp -o gen
g++ -std=c++17 -O2 brute.cpp -o brute
g++ -std=c++17 -O2 a.cpp -o a
g++ -std=c++17 -O2 stress.cpp -o stress
./stress
```

程序逐轮生成数据、运行暴力和待测代码，直接用 `system("diff -wB brute.out a.out")` 比较答案。答案不同或程序异常退出时停止，种子显示在终端，输入和两份输出留在当前目录；再次运行会覆盖这些文件。编译、开栈和限时按需使用册首的 Linux 命令，不封装进对拍程序。

`support/tools/stress/` 另有最大子段和、区间操作、排列逆序对、树上路径、图最短路和字符串匹配的生成器、暴力和参考程序。原 Python / Bash 工具保留；Python 版支持 ASan / UBSan、浮点比较和独立失败目录。具体用法见 [Linux 对拍说明](support/tools/stress/README.md)。

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

新增模板另有四组 `extra_*_random.cpp`，覆盖随机暴力对拍和数值边界；公开题验证使用 Library Checker 官方数据。题目对应、验证范围和复跑命令见 [验证说明](support/verify/README.md)。

`more_ds_random.cpp`、`more_graph_random.cpp` 覆盖吉司机线段树、可持久化数组、点分树和斯坦纳树的随机暴力对拍、重初始化与边界回归。

前批新增的 14 个片段由 `batch_math_random.cpp`、`batch_string_random.cpp` 和各专项 `*_random.cpp` 验证；包含匹配穷举、流量穷举、字符串穷举、几何独立真值和历史版本分叉。算法参考与改写来源见 [参考资料](support/verify/REFERENCES.md)。

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
本轮逐项修改与复验见 [竞赛接口审计](support/verify/AUDIT_CONTEST_INTERFACES_20261012.md)。
打印前的使用陷阱与处理记录见 [最终易用性审查](support/verify/AUDIT_PRINT_USABILITY_20261013.md)。

| 文件 | 对外接口 |
| --- | --- |
| `template/math/EquidistantLagrange.cpp` | `LagrangeEq::build`、`query`；连续整数采样的线性时间求值，依赖 `Combination.cpp` |
| `template/others/LIS.cpp` | `lis(a, strict)`；1-based 数组，返回严格递增 / 非递减子序列的下标方案 |
| `template/math/DivisorTransform.cpp` | `divisor_zeta`、`multiple_zeta`、`gcd_convolution`、`lcm_convolution`；支持合数模数 |
| `template/graph/FunctionalGraph.cpp` | `FunctionalGraph::build`、`jump`、`distance`；每点恰有一条出边，保存环与入环信息 |
| `template/math/Stirling.cpp` | `stirling_first`、`stirling_second`、`stirling_second_row`；无符号第一类、第二类与 NTT 求固定一行 |
| `template/string/Hash2D.cpp` | `Hash2D::build`、`query`；字节矩阵双模哈希，0-based 闭矩形 |
| `template/geometry/MinRectangle.cpp` | `min_rectangle(hull)`；严格逆时针凸包，返回面积与四角，依赖 `Point.cpp` |
| `template/ds/IntervalSet.cpp` | `IntervalSet::insert`、`erase`、`contains`、`mex`、`clear`；维护整数半开区间的并 |
| `template/others/SG.cpp` | `grundy(g, sg)`；写入 SG 值，有向环返回 `false` |
| `template/math/BostanMori.cpp` | `bostan_mori`、`linear_kth`；有理生成函数系数与大阶递推，依赖 `FPS.cpp` |
| `template/ds/PersistentXorTrie.cpp` | `PersistentXorTrie::init`、`insert`、`max_xor(left, right, x, ans)`；空差集返回 `false` |
| `template/ds/KDTree.cpp` | `KDTree::build`、`nearest(x, y, ans, exclude)`、`query`；最近邻为空返回 `false` |
| `template/graph/KShortestWalk.cpp` | `KShortestWalk::init`、`add_edge`、`query(s, t, k)`；非负边权，按边序列区分游走 |
| `template/graph/LowerBoundMinCostFlow.cpp` | `LRCostFlow::init`、`add_edge`、`add_supply`、`solve`；上下界与点的净供给，支持负费用与负环 |
| `template/graph/Dominator.cpp` | `Dominator::init`、`add_edge`、`build(root)`；`idom[root] = root`，不可达为 0 |
| `template/graph/WeightedMatching.cpp` | `WeightedBlossom::init`、`add_edge`、`solve`；一般图最大总权匹配，`match` 保存方案 |
| `template/graph/MaxClique.cpp` | `MaxClique::init`、`add_edge`、`max_clique`、`max_independent_set`；返回精确最优顶点集合 |
| `template/math/Min25.cpp` | `Min25::init`、`sum`、`count_primes`；素数处至多二次多项式，传入素数幂回调 |
| `template/math/Simplex.cpp` | `Simplex::solve(A, b, c)`；最大化 `c*x`，约束 `A*x <= b`、`x >= 0` |
| `template/string/Lyndon.cpp` | `lyndon_factorization(s)`；返回含 0 和 n 的分界点，0-based 半开区间 |
| `template/string/Runs.cpp` | `enumerate_runs(s)`；返回 `{最小周期, l, r}`，依赖 `SuffixArray.cpp` |
| `template/geometry/CircleUnion.cpp` | `circle_union(cs)`；返回圆并的面积与周长，依赖 `Point.cpp`、`Line.cpp`、`Circle.cpp` |
| `template/geometry/ConvexHull3D.cpp` | `P3`、`Hull3D::build`、`area`、`volume`；`dim` 为维度，`faces` 为外向三角面 |
| `template/others/DLX.cpp` | `DLX::init`、`add_row`、`solve(rows)`；写入精确覆盖行编号，无解返回 `false` |
| `template/ds/SegmentTreeBeats.cpp` | `Beats::init`、`add`、`chmin`、`chmax`、`query`、`query_min`、`query_max`；区间加、截断、和与最值 |
| `template/ds/PersistentArray.cpp` | `PArray::init`、`modify(root, pos, value)`、`query(root, pos)`；`root` 为初始根，修改返回新根 |
| `template/graph/CentroidTree.cpp` | `CentroidTree::init`、`add_edge`、`build`、`modify(u, delta)`、`query(u, radius)`；动态点权的距离范围和 |
| `template/graph/SteinerTree.cpp` | `Steiner::init`、`add_edge`、`solve(terminals)`；最小连接代价，不可连通返回 `-1` |
| `template/math/SubsetConvolution.cpp` | `subset_convolution(a, b, mod)`；0-based 位掩码，支持合数模数 |
| `template/math/Multipoint.cpp` | `Multipoint::build`、`evaluate`、`interpolate`；依赖 `FPS.cpp`，固定模数 `998244353` |
| `template/graph/KruskalTree.cpp` | `KruskalTree::add_edge`、`build(descending)`、`lca`、`query(u, v, ans)`；不连通返回 `false` |
| `template/graph/BlockCutTree.cpp` | `block_cut_tree(n, blocks)`；输入点双顶点列表，返回广义圆方森林 |
| `template/graph/BridgeTree.cpp` | `bridge_tree(cnt, bel, edges)`；返回保留原桥编号的缩点森林 |
| `template/graph/KthAncestor.cpp` | `KthAncestor::add_edge`、`build(root)`、`query(u, k)`；长链剖分，查询 `O(1)` |
| `template/graph/GomoryHu.cpp` | `GomoryHu::add_edge`、`build`、`query`；依赖 `Dinic.cpp`，无向非负容量 |
| `template/ds/LiChaoTree.cpp` | `LiChaoTree::add_line`、`add_seg`；浮点用 `add_line_real`、`add_seg_real`；`query_id`、浮点 `query`；仅整数直线用 `query_int(x, ans)` |
| `template/ds/LeftistHeap.cpp` | `LeftistHeap::init`、`push`、`merge`、`top`、`pop`；合并与删除返回新根 |
| `template/others/MonotonicStack.cpp` | `monotonic_bounds(a, greater = false)`；左右最近严格更小 / 更大元素 |
| `template/others/ConvexHullTrick.cpp` | `CHT::add`、`query`、`clear`；斜率不增、横坐标不减、求最小值 |
| `template/math/ModArithmetic.cpp` | `pow_mod`、`linear_inverse`、`batch_inverse`；`mod <= INT_MAX`，逆元要求质数模数 |
| `template/math/MatrixInverse.cpp` | `matrix_inv(a, mod, b)`；写入逆矩阵，不可逆返回 `false` |
| `template/math/PrimitiveRoot.cpp` | `primitive_root`；质数模数，依赖 `PollardRho.cpp` |
| `template/ds/DSU.cpp` | `DSU::init`、`find`、`merge`、`same`、`size`，`cnt` 为连通块数 |
| `template/ds/Fenwick.cpp` | `Fenwick::init`、`add`、`query`、`kth`；`kth` 要求原数组非负 |
| `template/ds/RollbackDSU.cpp` | `RollbackDSU::merge`、`snapshot`、`rollback` |
| `template/ds/SegTree.cpp` | `SegTree::init`、`modify`、`query`、`first_ge`、`last_ge`、`kth`；`kth` 要求所有元素非负 |
| `template/geometry/Point.cpp` | 原版 `P`、`db`、`EPS`、`sign`、`cmp`、`rad`、`polar_cmpX`、`polar_cmpY` |
| `template/geometry/Line.cpp` | `Line`、`isLL`、`isSS`、`isLS`、`onSeg`、`proj`、`reflect`、`nearest`、`disSS` 等 |
| `template/geometry/Polygon.cpp` | `area`、`contain`、`convexCut` |
| `template/geometry/ConvexHull.cpp` | `convexHull`、`convexHullNonStrict`、`checkConvexHull`（简单多边形的逆时针凸性）、`convexDiameter`（顺逆时针均可）、`MincowskySum` |
| `template/graph/BipartiteMatch.cpp` | `Kuhn::add_edge`、`solve` |
| `template/graph/Bridge.cpp` | `Bridge::add_edge`、`find` |
| `template/graph/CutVertex.cpp` | `CutVertex::add_edge`、`find` |
| `template/graph/DiffConstraints.cpp` | `Diff::add`、`solve`；全 0 初始化，负环返回 false；使用 ll，松弛中间量不得溢出 |
| `template/graph/Dijkstra.cpp` | `dijkstra` |
| `template/graph/DSUOnTree.cpp` | `DSUOnTree::add_edge`、`build(root)`；按题意补全 `add`、`del`、`query` |
| `template/graph/DirectedEulerTrail.cpp` | `euler_d` |
| `template/graph/UndirectedEulerTrail.cpp` | `euler_u` |
| `template/graph/Kruskal.cpp` | `Edge`、`MST`、`kruskal`；结果为 `sum`、`ids`、`ok` |
| `template/graph/LCA.cpp` | `LCA::add_edge`、`build`、`lca` |
| `template/graph/Dinic.cpp` | `Dinic::add_edge`、`flow` |
| `template/graph/MinCostFlow.cpp` | `MCMF::add_edge`、`flow`；无负环常规题 |
| `template/graph/ScalingMinCostFlow.cpp` | `ScalingMCMF::add_edge`、`flow`；UOJ 487 / Capacity Scaling |
| `template/graph/SCC.cpp` | `SCC::add_edge`、`build`，`bel` 保存分量编号 |
| `template/graph/VBCC.cpp` | `VBCC::add_edge`、`build`、`bcc`（每项的 `v` 为顶点，`e` 为边编号） |
| `template/graph/EBCC.cpp` | `EBCC::add_edge`、`build`、`bcc`（每项含 `v`、`e`）、`bel`、`is_bridge` |
| `template/math/AdaptiveSimpson.cpp` | `simpson` |
| `template/math/Combination.cpp` | `Comb::init`、`choose` |
| `template/math/FWT.cpp` | `fwt_or`、`fwt_and`、`fwt_xor` 及逆变换 |
| `template/math/NTT.cpp` | `ntt`、`multiply` |
| `template/math/PrimeSieve.cpp` | `primes` |
| `template/math/LinearSieve.cpp` | `Sieve::build`，`prime`、`lp`、`phi`、`mu` |
| `template/math/QuotientBlocks.cpp` | `div_blocks` |
| `template/math/XorBasis.cpp` | `Basis::insert`、`contains`、`max_xor`、`kth(k, ans)`；0-based，包含 0，越界返回 `false` |
| `template/others/CartesianTree.cpp` | `Cartesian::build` |
| `template/others/Mo.cpp` | `Mo::Query`、`Mo::solve(n, qs)`；按题意补全 `add`、`del`、`answer` |
| `template/others/SlidingWindow.cpp` | `window_min`、`window_max` |
| `template/others/SparseTable.cpp` | `ST::build`、`query` |
| `template/others/Common.cpp` | `chkmin`、`chkmax`、`add`、`sub` 及常用宏、类型别名 |
| `template/others/TernarySearch.cpp` | `ternary_min`、`ternary_max` |
| `template/string/ACAutomaton.cpp` | `AC::insert`、`build`、`query` |
| `template/string/KMP.cpp` | `get_pi`、`kmp` |
| `template/string/Manacher.cpp` | `Manacher::build`、`query`、`longest` |
| `template/string/SAM.cpp` | `SAM::build`、`count`、`query` |
| `template/string/Trie.cpp` | `Trie::insert`、`prefix`、`count` |
| `template/others/Discretize.cpp` | `discretize(a, key)`；返回 1-based 编号，`key` 输出排序去重后的值，恢复为 `key[id[i] - 1]` |
| `template/string/SuffixArray.cpp` | `SA::build`、`lcp` |
| `template/graph/EulerLCA.cpp` | `EulerLCA::add_edge`、`build`、`lca` |
| `template/graph/Reroot.cpp` | `reroot` |
| `template/graph/TreeDiameter.cpp` | `tree_diameter` |
| `template/graph/TopologicalSort.cpp` | `topo(g, order)`；写入拓扑序，有环返回 `false` |
| `template/others/TreeHash.cpp` | `tree_hash(g, root = 1)` |
| `template/math/Matrix.cpp` | `matrix_mul`、`matrix_pow` |
| `template/math/Lagrange.cpp` | `lagrange` |
| `template/math/BerlekampMassey.cpp` | `bm`、`kth` |
| `template/ds/WeightedDSU.cpp` | `WDSU`、`XDSU`：`init`、`find`、`merge`、`query(u, v, ans)`；不连通返回 `false` |
| `template/ds/PersistentSegTree.cpp` | `PST::build`、`kth`、`count` |
| `template/ds/FHQTreap.cpp` | `Treap::insert`、`erase`、`rank`、`kth(k, ans)`、`pre(x, ans)`、`suf(x, ans)`；`SeqTreap::insert`、`erase`、`reverse`、`query`、`at` |
| `template/ds/XorTrie.cpp` | `XorTrie::init`、`insert`、`erase`、`max_xor(x, ans)`；空集返回 `false` |
| `template/graph/HLD.cpp` | `HLD::add_edge`、`build`、`lca`、`path`；子树区间为 `[in[u], out[u]]` |
| `template/graph/TwoSAT.cpp` | `SAT::add`、`force`、`solve`，`ans` 保存布尔解 |
| `template/graph/VirtualTree.cpp` | `VTree::add_edge`、`build`、`query`；`nodes` 为虚树节点，`t` 为带距离的虚树边 |
| `template/graph/Centroid.cpp` | `Centroid::add_edge`、`query(k)`，统计距离不超过 `k` 的无序点对 |
| `template/graph/LowerBoundFlow.cpp` | `LRFlow::add_edge`、`solve`；无参为可行循环流，`solve(s, t, mode)` 求可行 / 最大 / 最小流，`f` 保存各边流量 |
| `template/graph/KM.cpp` | `KM::add_edge`、`solve`；最大权左侧完全匹配，`ml` / `mr` 为匹配 |
| `template/graph/HopcroftKarp.cpp` | `HK::add_edge`、`solve`、`cover`；`cover` 在最大匹配后返回最小点覆盖 |
| `template/math/CRT.cpp` | `exgcd`、`inv`（无逆元返回 `-1`）、`crt(r, m, ans, mod)`（无解或 lcm 超出 ll 返回 `false`） |
| `template/math/BSGS.cpp` | `bsgs`、`exbsgs`，返回最小非负指数或 `-1` |
| `template/math/Lucas.cpp` | `Lucas::init`、`choose` |
| `template/math/PollardRho.cpp` | `is_prime`、`factor`，支持 64 位无符号整数 |
| `template/math/Gauss.cpp` | `gauss`、`gauss_mod`、`gauss_xor`；返回 `0` 无解 / `1` 唯一解 / `2` 多解 |
| `template/math/FPS.cpp` | `FPS::mul`、`inv`、`ln`、`exp`、`pow`、`divmod`；`sqrt(a, n, ans)` 无解返回 `false`，模数 `998244353` |
| `template/math/FFT.cpp` | `fft`、`fft_mul`、`conv_mod`，后者为任意正 `int` 模数的精确卷积 |
| `template/string/Z.cpp` | `get_z`、`exkmp`，字符串位置为 0-based |
| `template/string/Hash.cpp` | `Hash::build`、`query`、`lcp`，双模哈希 |
| `template/string/PAM.cpp` | `PAM::init`、`insert`、`build`、`distinct`、`count` |
| `template/string/MinRotation.cpp` | `min_rotation`，返回最小循环表示的起始位置 |
| `template/others/CDQ.cpp` | `dominance`，统计每点被其他点三维支配的数量 |
| `template/others/OverallBinary.cpp` | `KthOffline::init`、`modify`、`query`、`solve`，离线求带修改区间第 k 小 |
| `template/geometry/Circle.cpp` | `Circle`、`isCL`、`isCC`、`tangent_points`、`common_tangents`、`circle_area`、`circumcircle(a, b, c, ans)`、`diameter_circle` |
| `template/geometry/HalfPlane.cpp` | `half_plane` |
| `template/geometry/MinCircle.cpp` | `min_circle` |
| `template/geometry/ClosestPair.cpp` | `ClosestPair::build`，返回原下标，`d2` 为最短距离平方 |
| `template/geometry/ConvexQuery.cpp` | `ConvexQuery::build`、`query`，返回 0 外部 / 1 边界 / 2 内部 |
| `template/geometry/RectangleUnion.cpp` | `RectUnion::solve`，返回矩形并的面积与周长 |
| `template/ds/AffineSegTree.cpp` | `AffineSegTree::init`、`modify`、`query`；修改为乘后加 |
| `template/ds/Fenwick2D.cpp` | `Fenwick2D::build`、`add`、`query`，离线收集更新坐标，查询闭矩形 |
| `template/ds/MergeSegTree.cpp` | `MergeSegTree::init`、`add`、`query`、`kth`、`merge`、`split`；合并消耗源根 |
| `template/ds/LCT.cpp` | `LCT::init`、`same`、`link`、`cut`、`modify`、`query`；维护点权路径和 |
| `template/graph/DynamicConnectivity.cpp` | `DynConn::init`、`add`、`erase`、`query`、`solve` |
| `template/graph/ZeroOneBFS.cpp` | `bfs01`，边权只能为 0 或 1 |
| `template/graph/Floyd.cpp` | `floyd`，原地更新距离矩阵，返回是否无负环 |
| `template/graph/Johnson.cpp` | `Johnson::init`、`add_edge`、`solve(dis)`；写入距离矩阵，存在负环返回 `false` |
| `template/graph/GlobalMinCut.cpp` | `MinCut::init`、`add_edge`、`solve`，返回割权与一侧顶点 |
| `template/graph/DirectedMST.cpp` | `DMST::init`、`add_edge`、`solve(root, ans)`；写入最小权，不可达返回 `false` |
| `template/graph/GeneralMatching.cpp` | `Blossom::init`、`add_edge`、`solve`，`match` 保存匹配 |
| `template/math/FloorSum.cpp` | `floor_sum`，返回 `ll`，支持有符号参数，所有中间量须放入 `ll` |
| `template/math/ModSqrt.cpp` | `sqrt_mod`；质数模数，不存在平方根返回 `-1` |
| `template/math/DuJiao.cpp` | `DuJiao::init`、`phi`、`mu`；前缀和返回 `ll`，`n <= 4e9` |
| `template/math/ExLucas.cpp` | `ExLucas::init`、`choose`，任意正 `int` 模数，预处理规模受质数幂大小限制 |
| `template/math/Determinant.cpp` | `det`、`matrix_tree`，质数模数；有向版本为从根向外的树 |
| `template/others/MoModify.cpp` | `MoModify::init`、`modify`、`query`、`solve()`；补全 `add`、`del`、`answer`，按值增删 |
| `template/others/MoTree.cpp` | `MoTree::init`、`add_edge`、`build`、`query`、`solve()`；补全 `add`、`del`、`answer`，按点增删 |
| `template/others/DPOpt.cpp` | `dc_dp`、`bounded_knapsack`，分别为单层分治转移与一种物品的多重背包转移 |
| `template/string/BitsetLCS.cpp` | `bitset_lcs`，只求 LCS 长度，支持任意字节 |

## 约定

- 图和树模板通常使用 1-based 顶点编号；具体例外写在文件注释中。
- `template/math/NTT.cpp` 使用模数 `998244353`。
- `template/math/Lagrange.cpp` 要求横坐标在模数意义下互异且模数为质数。
- `template/graph/TreeDiameter.cpp` 的边权默认为非负数。
- `template/graph/Reroot.cpp` 是“所有点到所有点距离和”的换根 DP 实例；其他换根转移需按题目修改。
- 莫队与 DSU on Tree 不使用多回调接口；使用前在结构体中加入题目状态，并补全声明的 `add`、`del`、`answer/query` 函数定义。这些函数没有默认空实现，未补全时不能链接运行。
- `template/graph/DSUOnTree.cpp` 的 `add` 与 `del` 必须互为逆操作；`query(u)` 记录子树答案，`build(root)` 结束后统计状态恢复为空。
- 三种莫队每次 `solve` 前须清空题目统计状态；带修改莫队按值增删，普通莫队按下标增删，树上莫队按顶点增删。
- `template/others/TreeHash.cpp` 是随机化哈希，不能当作严格同构判定。
- 李超树的整数参数使用有理数精确比较，交叉乘积等全部中间量须放入 `ll`；所有横坐标在 `[0, 40000]` 且 `|k|`、`|b|`、`|y| <= 10^9` 是充分范围。浮点参数仍为浮点比较，不要混用后仍要求精确判等。
- 李超树按整个密集区间分配约 `4 * (R - L + 1)` 个 `int`，不是动态开点或坐标压缩；坐标范围必须能存下。`query` 是浮点近似值；整数 DP 只用 `add_line`，并在 `query_int(x, ans)` 返回 `true` 后读取 `ll` 答案。
- 莫队和树上启发式合并的结构体内给出不同值 / 颜色数的填写示例；先用示例成员替换声明，再按题意修改状态、增删和答案。样例中的值域必须先离散化；带修改莫队还要包含所有将来修改的值。
- `CHT` 仅用于斜率不增、查询横坐标不减的最小值问题；不满足单调性时使用李超树等结构。

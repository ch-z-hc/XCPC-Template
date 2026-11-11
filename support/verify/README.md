# 模板验证

2026-10-13 最新调整：比赛模板和入口全部取消宽整数；精确整数真值只保留在验证程序内，不印入册。快速幂与逆元要求 `mod <= INT_MAX`，杜教筛要求 `n <= 4e9`；李超树、斜率优化、类欧几里得和费用流的全部中间量须放入 `ll`。CRT 与 Pollard–Rho 使用倍增模乘保留大模数范围，后者每次模乘增加 `O(log n)` 代价。最新记录见 [打印前审查](AUDIT_PRINT_USABILITY_20261013.md) 第七节；下文保留历次验证快照。

最新接口调整见 [竞赛接口审计](AUDIT_CONTEST_INTERFACES_20261012.md)：18 个模板移除 `optional`，改用 `bool` 加输出参数或安全的 `-1`；李超树用明确的整数 / 浮点接口，不再使用类型萃取。所有调用处、随机真值比较与官方题适配同步更新。失败时输出参数无保证，只能在返回 `true` 时使用。

本轮复验已完成：145 个模板语法检查、45 个标准验证程序、5 个新种子专项、20 题共 409 组本地官方数据全部通过；完整 Linux 流程含独立整数真值和对拍工具检查退出 0。打印册 147 项、232 页通过内容 / 版面检查与改动页目视检查。逐项接口审计和当前源码快照见上述报告；证据在 `build/contest-interfaces-20261012/`。

上一阶段竞赛写法调整：差分约束改为纯 `ll` Bellman–Ford，负环仍返回 `false`，不再做宽整数平移或抛异常；合法输入必须保证判负环前的全部松弛中间量也不溢出。独立真值可在验证程序内使用宽整数。模板中的具名类型转换已换成普通类型转换；当前 CRT 溢出返回 `false`，FWT 的长度前提用 `assert` 检查。

上一阶段复验：145 个模板语法检查、45 个标准验证程序全部通过 ASan / UBSan，完整 Linux 流程及独立整数 / 对拍工具检查退出 0；salt `2026101211` 的图论、数论专项重播分别通过。打印册 147 项、231 页通过逐行内容与版面检查。证据保存在 `build/contest-style-20261012/`，源码 SHA-256 见其中的 `source-index.json`。这些结果对应移除 `optional` 前的版本；新接口另行复验，不把旧结果冒充本轮结果。

第二轮重新全量复核见 [2026-10-12 第二轮审计](AUDIT_ROUND2_20261012.md)，仍对全部 145 个模板逐项记录。新发现并修复闵可夫斯基和的空集合语义、Simpson 在极大 / 极小有限尺度上的中间量溢出；补明三分的严格单调前提和欧拉路的非空顶点集前提。修复前后证据及本轮复跑结果单独保留，不覆盖第一轮记录。

第二轮最终结果：45 个标准验证程序、salt `2026101210` 的 42 个随机入口、20 题共 409 组官方生成数据全部通过；全量流程的独立整数 / 对拍工具检查完整退出 0。另修正 Jarvis 真值程序在全重复点上的缺陷，保留原失败日志；正式打印册已同步并通过内容及版面检查。

全量逐文件随机与特殊数据审计见 [2026-10-12 逐模板审计](AUDIT_PER_TEMPLATE_20261012.md)，覆盖全部 145 个模板，逐项记录真值来源、特殊数据及使用前提。新增 `audit_gaps_random.cpp`、`audit_state_random.cpp` 由全量脚本自动收录；`audit_reseed.py` 可在不修改原文件的情况下用另一组种子重播全部随机测试，并保存逐入口日志与 SHA-256。

自适应 Simpson 的左右误差抵消提前判停问题已修复：在原判据通过后补查下一层两侧绝对误差之和，接口不变。原失败反例、原失败种子重播及 1000 组变换回归通过；保留原 `1e-10` 精度断言。它仍是采样型积分，`eps` 不是任意连续函数的严格误差保证，深度耗尽仍返回近似值。修复前失败证据与修复后复验结果分别记录在上述审计中。

修复后，标准 Linux 流程的 45 个无输入验证程序及 salt `2026101207` 的 42 个随机入口全部通过 ASan / UBSan，全部模板均有随机和特殊数据审计。完整日志路径、种子与源码 SHA-256 见逐模板审计，不使用中断日志充当完整通过记录。

逐文件题源见 [SOURCES.md](SOURCES.md)，本轮审查与使用限制见 [AUDIT.md](AUDIT.md)。题目链接不等于通过记录；李超树整数线段的数学同值判定已修复，浮点调用仍不保证精确判等。

## Linux 全量检查

```sh
bash support/verify/run_linux.sh
```

栈设为 256 MiB。先对模板和验证程序执行 C++17 语法检查，再运行 46 个无输入验证程序，启用 ASan、UBSan 和标准库断言；任一失败立即退出。构建产物放在 `build/linux-verify/`。几何和 runs 等片段的复制依赖由语法检查中的 `-include` 补齐，模板本身不依赖自定义头文件。

`print_usability.cpp` 检查 HLD 的 DFS 序建树、李超树整数精确取值和端点小区间、宽整数输出的零／负数／最小值，以及异或高斯、Min_25 的调用示例。`print_usage_check.py` 直接从四个框架内的注释提取维护成员，替换声明后编译运行，覆盖重跑和颜色统计；另验证默认单测入口保留首个输入值，以及超大密集坐标区间先由断言拒绝，不触发有符号溢出。这两项均已加入全量流程，Python 示例检查仅在 Linux 下运行。

最后运行 `stress_tools_check.py`，检查 6 组对拍示例、C++ 对拍程序，以及 Python / Bash 脚本的故障保存流程；工具自身使用普通优化编译，`run.py --sanitize` 可额外检查对拍中的程序。Linux 下可用 `python3 support/verify/stress_tools_check.py --cpp-only` 仅检查 C++ 版。

## 常用补充验证

`nested_hull_random.cpp` 验证嵌套凸多边形切线与合法弦区间：将测试坐标放大为整数后使用宽整数枚举全部内点、弦和三角形作真值，覆盖二进制小数坐标、QOJ 14714 的 3 组官方样例、10000 组随机数据、切线贴边、循环换起点、重复 build、大坐标整系数变换、极细多边形及双方各 40001 点的凸链。`inside_triangle_submission.cpp` 为独立输入输出示例，计数部分使用单调指针与前缀和，总复杂度 O(n + m)。本地验证不代表在线提交 AC。

本批新增 10 个文件，并给原线段树补搜索接口，共 11 项能力。

| 入口 | 真值与边界 |
| --- | --- |
| `completion_math_random.cpp` | 等距插值、约数 / 倍数变换与两种卷积、斯特林数、Bostan–Mori 各 2000 轮；分别对照直接多项式求值、整除与数对枚举、计数恒等式和逐项递推。斯特林数另枚举至 8 个元素的排列环数与集合划分。 |
| `completion_structures_random.cpp` | 线段树搜索、区间集合、函数图、LIS、SG、二维哈希各 2000 轮；分别对照数组扫描、布尔覆盖、逐步走图、二次 DP、双子游戏胜负 DP 和直接二维多项式哈希。 |
| `min_rectangle_random.cpp` | 两组种子共 6000 组点集，每组测试原凸包、循环换起点、旋转缩放平移；逐边方向枚举最小矩形，另检查四角、垂直关系与包含性。含空集、单点、线段和 10 万点规则凸多边形。 |
| `deep_stack.cpp` | 函数图与 SG 追加 20 万点深链，保留递归；另测深链闭环拒绝。 |

边界覆盖：空输入 / 重构、重复值与负值、合数及 1 模数、`LLONG_MIN` / `MAX` 坐标、`ULLONG_MAX` 跳跃与递推。插值另测 10 万采样点，斯特林数另测 4096 项 NTT，递推另测 2048 阶。

复制依赖：等距插值先复制 `组合数.cpp`，斯特林数先复制 `快速数论变换与多项式乘法.cpp`，Bostan–Mori 先复制 `多项式基本运算.cpp`，包围矩形先复制 `点与向量.cpp`。包围矩形输入必须先取得严格逆时针凸包；二维哈希不保证无碰撞；线段树 `kth` 要求所有权值非负。

本批只记录本地随机与回归验证，不新增在线 AC 或官方全量数据通过记录。

## 既有随机与回归验证

本轮新增的四组测试：

| 验证程序 | 内容 |
| --- | --- |
| `extra_ds_random.cpp` | 树状数组第 k 小、仿射线段树、二维树状数组、动态开点合并分裂、离线动态连通性、LCT 各 2200 组；矩形面积与周长并 2500 组 |
| `extra_graph_random.cpp` | 最小树形图、一般图匹配、Johnson、Floyd、全局最小割、0-1 BFS、HK 点覆盖，各 2000 轮 |
| `extra_math_random.cpp` | 有符号类欧几里得、二次剩余、杜教筛、扩展 Lucas、行列式 / 矩阵树、FPS 幂 / 开根 / 除法、线性基第 k 小，各至少 2500 轮或查询 |
| `extra_offline_random.cpp` | 两种莫队、分治 DP、多重背包、位集 LCS、最近点对、凸多边形查询，各 2000 轮 |

真值分别采用数组逐项更新、图遍历、子集 / 排列枚举、朴素多项式运算、普通 DP 和点对枚举，不使用被测模板生成答案。另测重初始化、重复求解、空与单点、负权与负环、重边与自环、数值边界和 64 位块边界。

`deep_stack.cpp` 使用 20 万点长链，包含递归树上莫队、LCT、Kruskal 重构树、圆方树与桥树；第 k 级祖先另用 50 万点长链并更换根。验证入口与算法片段分开；`*_submission.cpp` 需要题目输入，不作为无输入测试直接运行。

`remaining_templates_random.cpp` 另覆盖离散化的空输入、重复值和整数极值，以及树哈希与有根树括号编码的一致性、换编号不变性，各 2000 轮。树哈希测试不能证明不存在碰撞。

`template_regressions.cpp` 覆盖差分约束在 `ll` 合法中间量范围内的大负权、负环、断开分量、空系统和重复调用；`graph_random.cpp` 另用独立宽整数 Floyd 对拍 500 组大权值图，生成范围保证 `n * m * max(abs(w))` 可装入 `ll`。不再测试需要宽整数中间量或异常返回的旧调用域。`geometry_random.cpp` 覆盖多边形面积的大坐标平移、顺逆时针、退化点集与凹多边形，期望值按实际输入坐标的边长计算。

逐文件复查追加：`template_regressions.cpp` 用 `INT_MAX` 值域覆盖动态线段树的增删、查询、第 k 小、合并、分裂和重初始化；整除分块覆盖 0、1、2 和 `10^12`，逐段核对连续性、商值与最大右端点。`geometry_random.cpp` 加入顺时针正方形超时反例，并对 500 组随机凸包反转方向、循环换起点，与枚举点对的直径对拍；凸性判断另测共线边界点和顺时针拒绝。

整除分块返回完整区间数组，`LLONG_MAX` 需要数十亿段，未完整运行该规模；末段在 `r == n` 时直接退出，避免 `r + 1` 溢出。`checkConvexHull` 仍为 O(n)，调用方须保证输入是简单多边形；它不负责判断自交。

`fixtures/lichao_p4097_tie.cpp` 保留原失败反例，修复后应退出成功；同一反例已加入常规 `li_chao_random.cpp`。整数参数用有理数精确比较，浮点参数保留原浮点比较。

`priority_templates_random.cpp` 覆盖新增的 6 类模板：单调栈、CHT 与 P3195 的 DP 转移、左偏树各 3000 轮；矩阵求逆对照排列展开行列式并验证左右乘积，各 2000 轮；快速幂对照逐次乘法，逆元验证模乘恒等式；原根枚举小素数的所有非零剩余类，并检查 64 位大素数。

本轮继续补充的测试：

| 验证程序 | 真值与边界 |
| --- | --- |
| `advanced_poly_random.cpp` | 子集卷积枚举所有子掩码，多点求值对照 Horner，插值对照朴素拉格朗日系数，各 2000 轮；另测 4096 点、重复点、零多项式、空数组、合数模数与整数极值。 |
| `advanced_tree_random.cpp` | Kruskal 重构树对照 Floyd 的 minimax / maximin；圆方树对照删点后的连通性；桥树核对原桥编号、森林性质与连通性；祖先查询逐步跳父亲；最小割树枚举所有割，各 2000 轮。含重复 build、更换根、负权、不连通、重边、自环和 LLONG_MAX 容量。 |
| `more_ds_random.cpp` | 吉司机线段树对照数组逐项更新，可持久化数组对照完整历史快照，各 2000 轮；覆盖全等、两值、交替截断与加法、旧版本分叉、重初始化和 64 位边界。 |
| `more_graph_random.cpp` | 点分树对照独立 DFS 求出的树上距离与数组点权和；斯坦纳树枚举全部边子集，各 2000 轮；另测 20 万点链、零权边、负点权、重复 build、重复终端、不连通、重边、自环和大权值。 |

全量脚本还运行 `check_lichao_integer.py`，使用 Python 任意精度整数作真值，覆盖 P4097 在线解码 200 组、大整数端点 200 组。C++ 适配程序同样启用 ASan / UBSan。

## 本次 14 项补充的独立测试

| 验证程序 | 真值与边界 |
| --- | --- |
| `batch_math_random.cpp` | Min_25 用逐数试除生成积性函数真值，1600 组，含二次素数值、负系数、合数模数与 INT_MAX；另与百万规模 φ 筛表对拍。单纯形用枚举顶点与独立高斯消元对拍 1200 组有界 LP，另测无解、无界、零尺寸和 Beale 退化例。 |
| `batch_string_random.cpp` | 穷举长度不超过 9 的三元字符串，runs 对照逐子串最小周期枚举；Lyndon 因子核对后缀严格比较与因子不增性。另测 1500 组任意字节串和 10 万长度全等串。 |
| `persistent_xor_trie_random.cpp` | 300 组、每组 180 次历史版本分叉，对照完整多重集快照；另枚举 99 个元素的全部闭区间，覆盖最高位、全 1 和重复值。 |
| `k_shortest_walk_random.cpp` | 1200 组小图，对照每点最多出堆 k 次的独立枚举；覆盖所有源汇、空游走、零权环、重边、不可达、近 inf 权值及 1 万层双重边 DAG。 |
| `lower_bound_cost_random.cpp` | 25000 组小图，穷举各边全部合法流量；覆盖不同供需、强制下界、孤立负环、负自环、重边、不可行、重复 solve 和 ll 大值。 |
| `dominator_random.cpp` | 1500 组小图，通过逐点删除后的可达性确定支配集合，对所有根求直接支配点；覆盖不可达、自环、重边、更换根和深链。 |
| `weighted_matching_random.cpp` | 20000 组 n <= 12 的图对子集匹配穷举；枚举全部 59049 个 5 点三态边图；另含历史 Blossom 反例、n = 500 图、非正权、重边、奇环和 ll 边界。 |
| `max_clique_random.cpp` | 穷举全部 1024 个 5 点图，另 1200 组不超过 16 点的图对子集枚举；最大团与一般图最大独立集分别核对，另测 100 点完全图和空图。 |
| `kdtree_random.cpp` | 500 个随机点集、各 200 次最近邻与矩形查询，对照直接枚举；覆盖平手、排除点、重复点、空集、重建、坐标极值、10 万共线点。 |
| `dlx_random.cpp` | 2500 组矩阵对照所有行子集；逐字段快照核对成功和失败后的链完全恢复，连续求解、追加行、空行、0 列与 Knuth 示例。 |
| `circle_union_random.cpp` | 解析值覆盖重复、包含、相切、两圆相交；两组种子共 200 组独立竖切片积分和圆周采样，另测洞、零半径、小圆、近重合、打乱顺序与平移。平移后周长用局部坐标采样检验实际输入，避免相切舍入导致不变性断言误报；数值采样不是严格精度证明。 |
| `hull3d_random.cpp` | 1500 个整数点集，每组 3 种顺序；独立枚举整数支撑平面并做面内凸包求面积体积，核对支撑方向和闭合网格；另测立方体、四面体、重复点、共面和共线。 |

本次新片段的算法参考、代码改写来源和许可见 [REFERENCES.md](REFERENCES.md)。上述测试不等于新增公开题的官方全量数据或在线 AC。

## Library Checker 官方数据

题目与生成器来自 [library-checker-problems](https://github.com/yosupo06/library-checker-problems)，本轮固定在提交 `1814c4e5205517e368bb57a8d1127eb961cfeaae`。本地运行完整官方生成数据；生成器与验证脚本均核对 `hash.json`。这不是在线提交，也不表示已经取得在线 AC。

| 题目 | 对应模板 | 数据组数 |
| --- | --- | ---: |
| [Range Chmin Chmax Add Range Sum](https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum) | `Beats` | 33 |
| [Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum) | `AffineSeg` | 19 |
| [Point Add Rectangle Sum](https://judge.yosupo.jp/problem/point_add_rectangle_sum) | `Fenwick2D` | 18 |
| [Dynamic Tree Vertex Add Path Sum](https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum) | `LCT` | 15 |
| [General Matching](https://judge.yosupo.jp/problem/general_matching) | `Blossom` | 12 |
| [Directed MST](https://judge.yosupo.jp/problem/directedmst) | `DMST`，只核对最优权值 | 12 |
| [Counting Spanning Tree (Directed)](https://judge.yosupo.jp/problem/counting_spanning_tree_directed) | `matrix_tree`，从根向外 | 22 |
| [Counting Spanning Tree (Undirected)](https://judge.yosupo.jp/problem/counting_spanning_tree_undirected) | `matrix_tree`，无向 | 22 |
| [Sqrt Mod](https://judge.yosupo.jp/problem/sqrt_mod) | `sqrt_mod` | 13 |
| [Sum of Floor of Linear](https://judge.yosupo.jp/problem/sum_of_floor_of_linear) | `floor_sum` | 11 |
| [Matrix Determinant](https://judge.yosupo.jp/problem/matrix_det) | `det` | 25 |
| [Division of Polynomials](https://judge.yosupo.jp/problem/division_of_polynomials) | `FPS::divmod` | 35 |
| [Pow of Formal Power Series](https://judge.yosupo.jp/problem/pow_of_formal_power_series) | `FPS::pow` | 37 |
| [Sqrt of Formal Power Series](https://judge.yosupo.jp/problem/sqrt_of_formal_power_series) | `FPS::sqrt` | 35 |
| [Closest Pair](https://judge.yosupo.jp/problem/closest_pair) | `ClosestPair` | 29 |
| [Inverse Matrix](https://judge.yosupo.jp/problem/inverse_matrix) | `matrix_inv` | 26 |
| [Primitive Root](https://judge.yosupo.jp/problem/primitive_root) | `primitive_root` | 14 |
| [Subset Convolution](https://judge.yosupo.jp/problem/subset_convolution) | `subset_conv` | 11 |
| [Multipoint Evaluation](https://judge.yosupo.jp/problem/multipoint_evaluation) | `Multipoint::eval` | 11 |
| [Polynomial Interpolation](https://judge.yosupo.jp/problem/polynomial_interpolation) | `Multipoint::interp` | 9 |

合计 20 题、409 组数据。除 `DMST` 仅提供最小权值、不恢复父亲数组，因此比较官方答案首项外，其余均使用题目官方 checker，允许不同的合法匹配、点对、平方根和原根方案。每组限时 120 秒，仅用于本地正确性验证，不据此宣称满足题目的在线时限。

Linux 下复跑：

```sh
git -c core.autocrlf=false clone --depth 1 https://github.com/yosupo06/library-checker-problems.git build/library-checker-problems
git -C build/library-checker-problems fetch --depth 1 origin 1814c4e5205517e368bb57a8d1127eb961cfeaae
git -C build/library-checker-problems switch --detach 1814c4e5205517e368bb57a8d1127eb961cfeaae
python3 support/verify/run_public.py --generate
```

已有对应版本及生成数据时，直接运行 `python3 support/verify/run_public.py`。输入适配在 `oj/library_checker.cpp`，只转换下标、区间和题目操作，不另写一份算法。结果及各组名称保存在 `build/public-verify/results.json`。

未接公开题的新增能力仍有随机暴力对拍，不把随机测试写成公开题通过记录。扩展 Lucas 的预处理需要完整的质数幂阶乘表，不能直接拿大质数模数题的全规模数据测试。

另接了洛谷 P4213（杜教筛）、P4720（扩展 Lucas）、P5490（矩形面积并）、P1856（矩形周长并），共 5 组题面样例。官方页面抓取受限，样例从每题两份公开题面转载交叉确认，实际来源保存在脚本中；它们不是洛谷官方全量数据。

```sh
python3 support/verify/run_luogu_samples.py
```

脚本内嵌样例，复跑不依赖网络；结果放在 `build/luogu-samples/results.json`。

## 二维凸包查询与动态更新

`hull_query_random.cpp` 对 3 × 3 网格的全部 512 个子集枚举查询方向与包含点；随机点集与插入 / 删除序列对照原点枚举最大点积。包含真值枚举所有支撑直线，使用独立宽整数计算；覆盖空集、重复点、共线、负方向、零方向、1e9 坐标、叉积为 1 的细三角形、跨块凸包、删除后内部点恢复、重复删除、重初始化和大凸链。可传入整数参数切换随机种子。

三维凸包继续使用 `hull3d_random.cpp` 验证：整数支撑平面真值、面积体积、外向面与闭合边，并覆盖退化维度。

## 几何拆分

原 `geo.cpp` 拆为 `点与向量.cpp`、`直线与线段.cpp`、`多边形.cpp`、`凸包与直径与闵可夫斯基和.cpp`；原圆及补充算法拆为 `圆.cpp`、`半平面交.cpp`、`最小圆覆盖.cpp`。基础定义共用原版 `P` / `db` / `EPS`，函数接口不变，原示例入口移到 `geometry_cut_submission.cpp`。

拆分前备份在 `build/geometry-before-split-20261010/`。已核对原版 30 个顶层函数（含示例入口）和补充版 9 个顶层函数的实现没有改动；原版 `P` 保留，补充版重复的基础定义不再保留。两组原几何随机测试继续运行。

## 打印检查

```sh
python support/tools/generate_print.py
python support/verify/print_book_check.py
```

检查目录完整性、接口索引、源码哈希、逐行逐字符一致、无自动续行、页码与目录跳转、字体和版心边界。再渲染目录、更新的正文和分页处，检查实际可读性。

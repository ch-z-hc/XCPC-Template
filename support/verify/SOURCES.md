# 原题与验证题索引

核对日期：2026-10-12。覆盖 `template/` 下全部 145 个 `.cpp` 文件。

这里的链接是原题题面或公开模板题，不表示代码抄自该题，也不把通用算法强行归到某一道“最初原题”。洛谷收录的历史比赛题保留题目来源；Library Checker 使用其官方题面与数据仓库。

“直接”仍需写输入输出、转换下标与区间；“应用”还需题目建模或统计逻辑；“部分”表示题目与模板只有部分能力相同，不能直接声称能通过整题。`Common.cpp` 是辅助定义，没有独立原题。

验证链接不等于通过记录。本轮实际测试和限制见 [AUDIT.md](AUDIT.md)，每个文件对应的本地测试入口及源码哈希由 `check_sources.py` 记录。

## 数据结构（19 个）

| 文件 | 原题 / 验证题 | 对应关系与注意事项 |
| --- | --- | --- |
| `template/ds/IntervalSet.cpp` | [Nyaan Segment Set](https://nyaannyaan.github.io/library/data-structure/segment-set.hpp) | 算法参考；维护整数半开区间的并、删除和 mex，不是逐段赋值 ODT。 |
| `template/ds/AffineSegTree.cpp` | [Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)、[P3373 线段树 2](https://www.luogu.com.cn/problem/P3373) | 直接；闭区间与半开区间需转换，模数显式传入。 |
| `template/ds/PersistentXorTrie.cpp` | [P4735 最大异或和](https://www.luogu.com.cn/problem/P4735) | 应用；保存前缀异或值的历史根，将询问转为两个前缀版本之差；不是任意两棵无关树的差。 |
| `template/ds/KDTree.cpp` | [CGAL Spatial Searching](https://doc.cgal.org/latest/Spatial_searching/index.html) | 算法参考；静态二维最近邻与矩形计数，随机数据对照直接枚举，不声称在线通过。 |
| `template/ds/DSU.cpp` | [Unionfind](https://judge.yosupo.jp/problem/unionfind) | 直接；题目 0-based，模板 1-based。 |
| `template/ds/Fenwick.cpp` | [Point Add Range Sum](https://judge.yosupo.jp/problem/point_add_range_sum)、[P3374 树状数组 1](https://www.luogu.com.cn/problem/P3374) | 直接；`kth` 另外要求所有元素非负。 |
| `template/ds/Fenwick2D.cpp` | [Point Add Rectangle Sum](https://judge.yosupo.jp/problem/point_add_rectangle_sum) | 直接；须预先收集所有更新坐标，题面半开矩形转闭矩形。 |
| `template/ds/FHQTreap.cpp` | [P3369 普通平衡树](https://www.luogu.com.cn/problem/P3369)、[Range Reverse Range Sum](https://judge.yosupo.jp/problem/range_reverse_range_sum) | 分别对应 `Treap` 与 `SeqTreap`；删除只删一份，区间下标需转换。 |
| `template/ds/LCT.cpp` | [Dynamic Tree Vertex Add Path Sum](https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum) | 直接；题目点权增量需转为 `modify` 的新点权。 |
| `template/ds/LiChaoTree.cpp` | [P4097 李超线段树 / HEOI2013 Segment](https://www.luogu.com.cn/problem/P4097) | 整数端点直接对应；已修复数学同值编号判定，另需在线解码。浮点调用仍不能保证精确判等。 |
| `template/ds/LeftistHeap.cpp` | [P3377 左偏树 / 可并堆](https://www.luogu.com.cn/problem/P3377) | 应用；合并和删除返回新根；题目按任意编号查所在堆，需另配并查集与删除标记。 |
| `template/ds/MergeSegTree.cpp` | [P5494 线段树分裂](https://www.luogu.com.cn/problem/P5494) | 直接；合并、分裂会消耗原树，不能共享非空节点。 |
| `template/ds/PersistentSegTree.cpp` | [Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)、[P3834 静态区间第 k 小](https://www.luogu.com.cn/problem/P3834) | 直接；模板区间与 k 均为 1-based。不是任意单点改值版本。 |
| `template/ds/PersistentArray.cpp` | [P3919 可持久化线段树 1 / 可持久化数组](https://www.luogu.com.cn/problem/P3919) | 直接；已读取官方题面。调用者保存各版本根，查询产生的新版本复用被查询根；与静态区间第 k 小的主席树分开。 |
| `template/ds/RollbackDSU.cpp` | [Persistent Unionfind](https://judge.yosupo.jp/problem/persistent_unionfind) | 应用；需离线建立版本树，递归进入时合并、退出时回滚，并非在线持久化 DSU。 |
| `template/ds/SegTree.cpp` | [P3372 线段树 1](https://www.luogu.com.cn/problem/P3372)、[ACL Segtree](https://github.com/atcoder/ac-library/blob/master/document_en/segtree.md) | 前者对应区间加、区间和；阈值搜索与第 k 个另做扫描对拍，`kth` 要求元素非负；所有中间结果须不溢出 `ll`。 |
| `template/ds/SegmentTreeBeats.cpp` | [Range Chmin Chmax Add Range Sum](https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum) | 直接；题目为 0-based 半开区间，模板为 1-based 闭区间；另提供最小值与最大值查询。 |
| `template/ds/WeightedDSU.cpp` | [AOJ DSL_1_B Weighted Union Find Trees](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_B) | 对应差值版 `WDSU`，方向为 `value[v] - value[u]`；异或版另由随机约束对拍覆盖。不是模意义下的势能 DSU。 |
| `template/ds/XorTrie.cpp` | [P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551) | 应用；先求根到各点的路径异或值，再插入、查询。模板返回最大异或值，不能直接套求最小值的 `set_xor_min`。 |

## 图论（47 个）

| 文件 | 原题 / 验证题 | 对应关系与注意事项 |
| --- | --- | --- |
| `template/graph/FunctionalGraph.cpp` | [Nyaan Functional Graph](https://nyaannyaan.github.io/library/graph/functional-graph.hpp) | 算法参考；每点出度为 1，提供环分解、跳跃和最少到达步数；随机真值直接沿出边走。 |
| `template/graph/BipartiteMatch.cpp` | [Bipartite Matching](https://judge.yosupo.jp/problem/bipartitematching)、[P3386 二分图最大匹配](https://www.luogu.com.cn/problem/P3386) | 直接；Kuhn 适合中小规模，大规模优先 `HopcroftKarp`。 |
| `template/graph/KShortestWalk.cpp` | [K Shortest Walk](https://judge.yosupo.jp/problem/k_shortest_walk) | 直接能力；按边序列计数，允许重复顶点与零权环，源汇相同时包含空游走；非简单路径。 |
| `template/graph/LowerBoundMinCostFlow.cpp` | [Minimum Cost b-flow](https://judge.yosupo.jp/problem/min_cost_b_flow) | 部分；计算最小费用和各边流量，不输出题目要求的对偶势；先复制 MinCostFlow.cpp。 |
| `template/graph/Dominator.cpp` | [Dominator Tree](https://judge.yosupo.jp/problem/dominatortree) | 直接；题目下标需转换，根和不可达点的输出约定需转换。 |
| `template/graph/WeightedMatching.cpp` | [General Weighted Matching](https://judge.yosupo.jp/problem/general_weighted_matching) | 直接；最大总权而非最大基数优先，自环和非正权忽略，重边保留最大权。 |
| `template/graph/MaxClique.cpp` | [Maximum Independent Set](https://judge.yosupo.jp/problem/maximum_independent_set) | 直接；另提供最大团，指数时间，实际规模取决于图结构。 |
| `template/graph/Bridge.cpp` | [AOJ GRL_3_B Bridges](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_B) | 直接；输出格式需要按题目排序端点。 |
| `template/graph/Centroid.cpp` | [P4178 Tree](https://www.luogu.com.cn/problem/P4178) | 直接；统计距离不超过 k 的无序不同点对，要求非负边权。不是直接查询距离恰好为 k 的接口。 |
| `template/graph/CentroidTree.cpp` | [P6329 点分树 / 震波](https://www.luogu.com.cn/problem/P6329) | 直接能力；已读取官方题面。build 后用 modify 加入初始点权；赋值转为差量，另补题目要求的在线异或解码。模板支持非负边权及负点权。 |
| `template/graph/CutVertex.cpp` | [P3388 割点](https://www.luogu.com.cn/problem/P3388) | 直接；图不保证连通。 |
| `template/graph/DiffConstraints.cpp` | [P5960 差分约束](https://www.luogu.com.cn/problem/P5960) | 直接；`add(u, v, w)` 表示 `x[u] - x[v] <= w`。 |
| `template/graph/Dijkstra.cpp` | [AOJ GRL_1_A Single Source Shortest Path](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A)、[Shortest Path](https://judge.yosupo.jp/problem/shortest_path) | 前者直接；后者还要求恢复路径，模板仅提供距离，需另存前驱。边权必须非负。 |
| `template/graph/Dinic.cpp` | [P3376 网络最大流](https://www.luogu.com.cn/problem/P3376) | 直接；源汇不同，容量与总流量遵守源码数值限制。 |
| `template/graph/DirectedEulerTrail.cpp` | [Eulerian Trail (Directed)](https://judge.yosupo.jp/problem/eulerian_trail_directed) | 部分；模板返回顶点序列，题面还要边编号，可按相邻顶点从重边桶取边。不是字典序最小欧拉路。 |
| `template/graph/DirectedMST.cpp` | [P4716 最小树形图](https://www.luogu.com.cn/problem/P4716)、[Directed MST](https://judge.yosupo.jp/problem/directedmst) | 前者直接；后者部分，模板只返回最优权值，不恢复父亲。现有官方数据验证只核对权值。 |
| `template/graph/DSUOnTree.cpp` | [CF 600E Lomsat gelral](https://codeforces.com/problemset/problem/600/E) | 应用；需补全加点、删点、记录答案函数，自行维护众数频率及颜色和。 |
| `template/graph/DynamicConnectivity.cpp` | [P2147 SDOI2008 洞穴勘测](https://www.luogu.com.cn/problem/P2147)、[LOJ 121 动态图连通性](https://loj.ac/p/121) | 前者可离线适配，森林是一般图的子集；已核对其题面。后者原站返回错误，没有冒充已读取题面。 |
| `template/graph/EBCC.cpp` | [Two-Edge-Connected Components](https://judge.yosupo.jp/problem/two_edge_connected_components) | 直接；顶点、分量下标与输出格式需转换。 |
| `template/graph/BlockCutTree.cpp` | [P4320 道路相遇](https://www.luogu.com.cn/problem/P4320) | 应用；先用 VBCC 求点双，再构造广义圆方树；题目还需 LCA 和路径上的圆点计数，包含端点。 |
| `template/graph/BridgeTree.cpp` | [P2860 USACO06JAN Redundant Paths](https://www.luogu.com.cn/problem/P2860) | 应用；先用 EBCC 缩点，再统计桥树叶子；题目原图连通，需补叶子配对计数。 |
| `template/graph/KruskalTree.cpp` | [P2245 星际导航](https://www.luogu.com.cn/problem/P2245) | 直接；默认递增建树求路径最大边权的最小值，query 返回 false 时输出 impossible。模板另支持递减建树。 |
| `template/graph/KthAncestor.cpp` | [P5903 树上 k 级祖先](https://www.luogu.com.cn/problem/P5903) | 直接祖先查询；题目给父亲数组，需转为无向边，另补随机询问生成与校验和。越过根返回 0。 |
| `template/graph/GomoryHu.cpp` | [P3329 ZJOI2011 最小割](https://www.luogu.com.cn/problem/P3329)、[KACTL GomoryHu](https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/GomoryHu.h) | 应用；复用 Dinic 构造任意两点最小割树，题目还需统计最小割不超过给定值的无序点对数。 |
| `template/graph/EulerLCA.cpp` | [Lowest Common Ancestor](https://judge.yosupo.jp/problem/lca) | 直接；欧拉序 + ST，注意根及下标。 |
| `template/graph/Floyd.cpp` | [AOJ GRL_1_C All Pairs Shortest Path](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_C) | 直接；重边取最小，初始化对角与 `inf`，负环时不使用距离。 |
| `template/graph/GeneralMatching.cpp` | [Matching on General Graph](https://judge.yosupo.jp/problem/general_matching) | 直接；无权一般图最大匹配，不是带权带花树。 |
| `template/graph/GlobalMinCut.cpp` | [P5632 Stoer-Wagner](https://www.luogu.com.cn/problem/P5632) | 直接；无向非负边权，全局最小割，不是有向图最小割。 |
| `template/graph/HLD.cpp` | [Vertex Add Path Sum](https://judge.yosupo.jp/problem/vertex_add_path_sum)、[P3384 树链剖分](https://www.luogu.com.cn/problem/P3384) | 应用；只提供剖分、路径区间和子树区间，需配合 Fenwick / 线段树。 |
| `template/graph/HopcroftKarp.cpp` | [Bipartite Matching](https://judge.yosupo.jp/problem/bipartitematching) | 直接；最小点覆盖要在最大匹配之后调用 `cover`。 |
| `template/graph/Johnson.cpp` | [P5905 全源最短路（Johnson）](https://www.luogu.com.cn/problem/P5905) | 直接能力；题目最后还需按不可达规则计算加权校验和。 |
| `template/graph/KM.cpp` | [Assignment Problem](https://judge.yosupo.jp/problem/assignment) | 直接；题目求最小费用、模板求最大权，输入边权取负后使用。 |
| `template/graph/Kruskal.cpp` | [Minimum Spanning Tree](https://judge.yosupo.jp/problem/minimum_spanning_tree)、[P3366 最小生成树](https://www.luogu.com.cn/problem/P3366) | 直接；不连通时 `ok` 为 false，不能把森林权值当生成树答案。 |
| `template/graph/LCA.cpp` | [Lowest Common Ancestor](https://judge.yosupo.jp/problem/lca) | 直接；倍增，树必须连通。 |
| `template/graph/LowerBoundFlow.cpp` | [P5192 Shoot the Bullet](https://www.luogu.com.cn/problem/P5192)、[LOJ 115 无源汇上下界可行流](https://loj.ac/p/115)、[LOJ 116 有源汇上下界最大流](https://loj.ac/p/116) | 前者为建模应用，已核对题面；后两题直接对应循环流与有源汇流，原站抓取受限，不列入已跑公开数据题。 |
| `template/graph/MinCostFlow.cpp` | [P3381 最小费用最大流](https://www.luogu.com.cn/problem/P3381) | 直接；要求无负环，距离、势能与费用中间量不溢出 `__int128`，最终流量和费用可用 `ll` 表示。不能验证 UOJ 487 的负环能力。 |
| `template/graph/Reroot.cpp` | [CSES Tree Distances II](https://cses.fi/problemset/task/1133) | 直接；本模板具体求每个点到所有点的距离和，不是任意状态的通用换根框架。 |
| `template/graph/ScalingMinCostFlow.cpp` | [UOJ 487 最小费用最大流](https://uoj.ac/problem/487) | 直接；包含不与源汇连通的负环费用。 |
| `template/graph/SCC.cpp` | [Strongly Connected Components](https://judge.yosupo.jp/problem/scc) | 直接；Tarjan 出栈编号是逆拓扑顺序，题目要求的拓扑顺序需反转。 |
| `template/graph/SteinerTree.cpp` | [P6192 最小斯坦纳树](https://www.luogu.com.cn/problem/P6192) | 直接；已读取官方题面。无向非负图、少量关键点，返回最小边权和，不恢复边集；关键点去重，不连通返回 -1。 |
| `template/graph/TopologicalSort.cpp` | [AOJ GRL_4_B Topological Sort](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_B) | 直接；返回顺序，空结果的含义按源码区分。 |
| `template/graph/TreeDiameter.cpp` | [CSES Tree Diameter](https://cses.fi/problemset/task/1131)、[Tree Diameter](https://judge.yosupo.jp/problem/tree_diameter) | 前者直接；后者还需完整路径恢复。模板返回端点与长度，两次 DFS 要求非负边权。 |
| `template/graph/TwoSAT.cpp` | [Two SAT](https://judge.yosupo.jp/problem/two_sat)、[P4782 2-SAT](https://www.luogu.com.cn/problem/P4782) | 直接；布尔字面量的正负语义、编号需转换。 |
| `template/graph/UndirectedEulerTrail.cpp` | [Eulerian Trail (Undirected)](https://judge.yosupo.jp/problem/eulerian_trail_undirected) | 部分；返回顶点序列，题面边编号需从无向重边桶恢复。 |
| `template/graph/VBCC.cpp` | [Biconnected Components](https://judge.yosupo.jp/problem/biconnected_components) | 直接；割点可同时属于多个分量，不能当作每点唯一分量编号。 |
| `template/graph/VirtualTree.cpp` | [P2495 虚树 / SDOI2011 消耗战](https://www.luogu.com.cn/problem/P2495) | 应用；模板只构造虚树及距离。题目需原树路径最小边权与虚树 DP，距离不能代替最小边权。 |
| `template/graph/ZeroOneBFS.cpp` | [AtCoder ABC077 D Small Multiple](https://atcoder.jp/contests/abc077/tasks/arc084_b) | 应用；构造余数图，边权只能为 0 / 1，起点代价另算。 |

## 数学（34 个）

| 文件 | 原题 / 验证题 | 对应关系与注意事项 |
| --- | --- | --- |
| `template/math/EquidistantLagrange.cpp` | [Nyaan Equidistant Interpolation](https://nyaannyaan.github.io/library/fps/lagrange-interpolation-point.hpp) | 算法参考；连续整数采样，质数模数且采样数小于模数，与任意点插值分开。 |
| `template/math/DivisorTransform.cpp` | [GCD Convolution](https://judge.yosupo.jp/problem/gcd_convolution)、[LCM Convolution](https://judge.yosupo.jp/problem/lcm_convolution) | 直接；输入 index 0 为 0；LCM 大于数组上界的项不返回，另测合数模数。 |
| `template/math/Stirling.cpp` | [Stirling Number of the Second Kind](https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind)、[Nyaan Famous Series](https://nyaannyaan.github.io/library/fps/fps-famous-series.hpp) | 前者对应固定一行第二类卷积版；第一类为无符号版本，另用排列环数枚举验证。 |
| `template/math/BostanMori.cpp` | [K-th Term of Linearly Recurrent Sequence](https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence)、[Nyaan Bostan–Mori](https://nyaannyaan.github.io/library/fps/nth-term.hpp) | 前者直接；递推系数顺序遵循文件头，模数固定 998244353；有理生成函数接口另对拍。 |
| `template/math/AdaptiveSimpson.cpp` | [P4525 自适应辛普森法 1](https://www.luogu.com.cn/problem/P4525) | 应用；传入被积函数。局部采样误差不是对任意连续函数的严格误差保证。 |
| `template/math/Min25.cpp` | [Counting Primes](https://judge.yosupo.jp/problem/counting_primes)、[Sum of Multiplicative Function (Large)](https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large) | 前者对应 count_primes，后者传入 f(p^e) 回调；素数处支持二次多项式，规模须遵守 n <= 10^12 和内存限制，不声称全规模时限通过。 |
| `template/math/Simplex.cpp` | [KACTL Simplex](https://github.com/kth-competitive-programming/kactl/blob/main/content/numerical/Simplex.h)、[P3980 志愿者招募](https://www.luogu.com.cn/problem/P3980) | 前者为算法参考，后者需建模；实数线性规划，不是整数规划，数值稳定性受条件数与缩放影响。 |
| `template/math/BerlekampMassey.cpp` | [Find Linear Recurrence](https://judge.yosupo.jp/problem/find_linear_recurrence)、[K-th Term of Linearly Recurrent Sequence](https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence) | 直接；质数模数、0-based，递推系数与题面定义需对齐。 |
| `template/math/BSGS.cpp` | [Discrete Logarithm](https://judge.yosupo.jp/problem/discrete_logarithm_mod) | 直接；用 `exbsgs` 处理非互质情况，支持的模数不超过 `INT_MAX`。 |
| `template/math/Combination.cpp` | [P1313 NOIP2011 计算系数](https://www.luogu.com.cn/problem/P1313)、[Binomial Coefficient (Prime Mod)](https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod) | 前者用组合数配合两次快速幂；后者仅部分范围。模板要求 `n < mod` 且能存下 `O(n)` 阶乘表，不能直接过大 n 数据。 |
| `template/math/CRT.cpp` | [P4777 扩展中国剩余定理](https://www.luogu.com.cn/problem/P4777) | 直接；支持非互质模数，合并后的 lcm 须能放入 `ll`。 |
| `template/math/Determinant.cpp` | [Matrix Determinant](https://judge.yosupo.jp/problem/matrix_det)、[Counting Spanning Tree (Directed)](https://judge.yosupo.jp/problem/counting_spanning_tree_directed)、[Counting Spanning Tree (Undirected)](https://judge.yosupo.jp/problem/counting_spanning_tree_undirected) | 直接；有向矩阵树接口统计从根向外的树，模数要求为质数。 |
| `template/math/DuJiao.cpp` | [P4213 杜教筛](https://www.luogu.com.cn/problem/P4213) | 直接；`phi` 返回 `__int128`，输出时不能无条件截为 `ll`。 |
| `template/math/ExLucas.cpp` | [P4720 扩展卢卡斯定理](https://www.luogu.com.cn/problem/P4720) | 对应公式；要完整预处理各质数幂的阶乘表，不能承诺通过大质数模数的全规模数据。 |
| `template/math/FFT.cpp` | [Convolution (Mod 1000000007)](https://judge.yosupo.jp/problem/convolution_mod_1000000007)、[P3803 多项式乘法（FFT）](https://www.luogu.com.cn/problem/P3803) | 分别验证精确任意模数卷积 `conv_mod` 与浮点卷积 `fft_mul`；浮点版本存在精度、舍入和整数范围限制。 |
| `template/math/FloorSum.cpp` | [Sum of Floor of Linear](https://judge.yosupo.jp/problem/sum_of_floor_of_linear) | 直接；返回 `__int128`，还支持源码规定的有符号 a、b。 |
| `template/math/FPS.cpp` | [Inv](https://judge.yosupo.jp/problem/inv_of_formal_power_series)、[Log](https://judge.yosupo.jp/problem/log_of_formal_power_series)、[Exp](https://judge.yosupo.jp/problem/exp_of_formal_power_series)、[Pow](https://judge.yosupo.jp/problem/pow_of_formal_power_series)、[Sqrt](https://judge.yosupo.jp/problem/sqrt_of_formal_power_series)、[Division](https://judge.yosupo.jp/problem/division_of_polynomials) | 分别对应六种接口；固定模数 `998244353`，常数项和长度限制见源码。 |
| `template/math/Multipoint.cpp` | [Multipoint Evaluation](https://judge.yosupo.jp/problem/multipoint_evaluation)、[Polynomial Interpolation](https://judge.yosupo.jp/problem/polynomial_interpolation) | 直接；复用 FPS，模数固定为 998244353；插值的横坐标必须模意义下互异，求值允许重复点。 |
| `template/math/SubsetConvolution.cpp` | [Subset Convolution](https://judge.yosupo.jp/problem/subset_convolution)、[P6097 子集卷积](https://www.luogu.com.cn/problem/P6097) | 直接；两题模数不同，应显式传入；不是 FWT 的 OR 卷积。n = 20 时主要数组约 180 MiB。 |
| `template/math/FWT.cpp` | [P4717 快速莫比乌斯 / 沃尔什变换](https://www.luogu.com.cn/problem/P4717)、[Bitwise AND Convolution](https://judge.yosupo.jp/problem/bitwise_and_convolution)、[Bitwise XOR Convolution](https://judge.yosupo.jp/problem/bitwise_xor_convolution) | 直接能力；模板提供变换，卷积需变换、逐点乘、逆变换；不是子集卷积。 |
| `template/math/Gauss.cpp` | [P3389 高斯消元法](https://www.luogu.com.cn/problem/P3389)、[System of Linear Equations](https://judge.yosupo.jp/problem/system_of_linear_equations) | 实数题直接；模数题部分，模板只返回一个解及状态，不返回题面要求的零空间基。 |
| `template/math/Lagrange.cpp` | [P4781 拉格朗日插值](https://www.luogu.com.cn/problem/P4781) | 直接；任意互异横坐标的单点求值，`O(n^2)`，不是快速插值或多点求值。 |
| `template/math/LinearSieve.cpp` | [P3383 线性筛素数](https://www.luogu.com.cn/problem/P3383) | 直接；另有最小质因子、phi、mu，通过逐数暴力检查；存储多张表，不等于最省内存的纯筛。 |
| `template/math/Lucas.cpp` | [P3807 卢卡斯定理](https://www.luogu.com.cn/problem/P3807) | 直接；p 必须为质数且能存下 `O(p)` 表，题目查询的是 `C(n + m, n)`。 |
| `template/math/Matrix.cpp` | [Matrix Product](https://judge.yosupo.jp/problem/matrix_product)、[Pow of Matrix](https://judge.yosupo.jp/problem/pow_of_matrix) | 直接；非空、维度匹配、非负指数；不是矩阵逆。 |
| `template/math/MatrixInverse.cpp` | [Inverse Matrix](https://judge.yosupo.jp/problem/inverse_matrix)、[P4783 矩阵求逆](https://www.luogu.com.cn/problem/P4783) | 直接；0-based 方阵，质数模数不超过 INT_MAX，不可逆返回 false。 |
| `template/math/ModArithmetic.cpp` | [P3811 乘法逆元](https://www.luogu.com.cn/problem/P3811)、[P5431 乘法逆元 2](https://www.luogu.com.cn/problem/P5431) | 分别对应线性与批量逆元；P5431 还需按题面求加权和。快速幂另以逐次乘法对拍，逆元只支持质数模数。 |
| `template/math/ModSqrt.cpp` | [Sqrt Mod](https://judge.yosupo.jp/problem/sqrt_mod) | 直接；模数必须为质数，无解返回 `-1`。 |
| `template/math/NTT.cpp` | [Convolution](https://judge.yosupo.jp/problem/convolution_mod) | 直接；固定模数与原根，补零后的变换长度须符合源码上限。 |
| `template/math/PollardRho.cpp` | [Factorize](https://judge.yosupo.jp/problem/factorize)、[Primality Test](https://judge.yosupo.jp/problem/primality_test)、[P4718 Pollard-Rho](https://www.luogu.com.cn/problem/P4718) | 直接；前两题分别覆盖分解和判素，P4718 还需取最大质因子或输出 `Prime`。 |
| `template/math/PrimitiveRoot.cpp` | [Primitive Root](https://judge.yosupo.jp/problem/primitive_root) | 直接；质数模数，先复制 PollardRho 中的依赖；支持 64 位无符号整数。 |
| `template/math/PrimeSieve.cpp` | [Enumerate Primes](https://judge.yosupo.jp/problem/enumerate_primes) | 部分规模；普通筛需 `O(n)` 内存，不能用该题最高规模的压缩筛时限、内存作保证。 |
| `template/math/QuotientBlocks.cpp` | [Enumerate Quotients](https://judge.yosupo.jp/problem/enumerate_quotients) | 直接能力；每块取一次商，题目要求递增输出时需调整顺序。 |
| `template/math/XorBasis.cpp` | [P3812 线性基](https://www.luogu.com.cn/problem/P3812) | 直接求最大子集异或；`kth` 单独随机枚举验证，0-based 且包含 0、只算不同异或值。 |

## 字符串（14 个）

| 文件 | 原题 / 验证题 | 对应关系与注意事项 |
| --- | --- | --- |
| `template/string/Hash2D.cpp` | [Nyaan 2D Rolling Hash](https://nyaannyaan.github.io/library/string/rolling-hash-2d.hpp) | 算法参考；矩阵哈希，比较时同时检查尺寸；随机测试没有碰撞不代表严格判等。 |
| `template/string/ACAutomaton.cpp` | [P5357 AC 自动机](https://www.luogu.com.cn/problem/P5357)、[P3808 AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808) | 直接返回每个模式串出现次数；P3808 统计次数非零的模式编号，重复模式串不能去重。仅小写字母。 |
| `template/string/Lyndon.cpp` | [Lyndon Factorization](https://judge.yosupo.jp/problem/lyndon_factorization) | 直接；返回 0 和 n 在内的分界点，因子按字典序不增。 |
| `template/string/Runs.cpp` | [Run Enumerate](https://judge.yosupo.jp/problem/runenumerate) | 直接；最小周期与 0-based 半开区间，结果按 period、l、r 排序；先复制 SuffixArray.cpp。 |
| `template/string/BitsetLCS.cpp` | [AOJ ALDS1_10_C Longest Common Subsequence](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_C) | 直接求长度；不恢复 LCS 字符串。 |
| `template/string/Hash.cpp` | [AOJ ALDS1_14_B String Search](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B) | 应用；逐个子串与模式哈希比较，仍有碰撞概率，不构成严格字符串判等。 |
| `template/string/KMP.cpp` | [P3375 KMP](https://www.luogu.com.cn/problem/P3375) | 直接；模板匹配位置 0-based，题目输出 1-based。 |
| `template/string/Manacher.cpp` | [Enumerate Palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes)、[P3805 Manacher](https://www.luogu.com.cn/problem/P3805) | 对应回文半径与最长回文；各中心的半径定义需转换。 |
| `template/string/MinRotation.cpp` | [P1368 工艺](https://www.luogu.com.cn/problem/P1368) | 适配；模板只接收字符串，题目整数都在 0..29，可保序编码为单字节字符后调用，再输出旋转后的原序列。不能直接传 `vector`。 |
| `template/string/PAM.cpp` | [Eertree](https://judge.yosupo.jp/problem/eertree)、[P5496 回文树](https://www.luogu.com.cn/problem/P5496) | 对应建树能力；P5496 需另维护 fail 链回文个数并在线解密，不能把 `count()` 出现次数当答案。 |
| `template/string/SAM.cpp` | [P3804 后缀自动机](https://www.luogu.com.cn/problem/P3804)、[Number of Substrings](https://judge.yosupo.jp/problem/number_of_substrings) | 应用；前者用状态长度乘出现次数，后者累计 `len - len[link]`；仅单串小写字母，不是广义 SAM。 |
| `template/string/SuffixArray.cpp` | [Suffix Array](https://judge.yosupo.jp/problem/suffixarray)、[P3809 后缀排序](https://www.luogu.com.cn/problem/P3809) | 直接；SA 的位置 0-based，P3809 输出须加 1。 |
| `template/string/Trie.cpp` | [P2580 于是他错误的点名开始了](https://www.luogu.com.cn/problem/P2580) | 应用；字典树判断存在性，重复点名需另维护已访问标记。仅小写字母。 |
| `template/string/Z.cpp` | [Z Algorithm](https://judge.yosupo.jp/problem/zalgorithm)、[P5410 扩展 KMP](https://www.luogu.com.cn/problem/P5410) | 直接；P5410 还需按题目计算异或校验和。 |

## 其他算法（18 个）

| 文件 | 原题 / 验证题 | 对应关系与注意事项 |
| --- | --- | --- |
| `template/others/LIS.cpp` | [Longest Increasing Subsequence](https://judge.yosupo.jp/problem/longest_increasing_subsequence) | 直接；输入转为 1-based，返回原下标；非递减版另做随机 DP 对拍。 |
| `template/others/SG.cpp` | [Nyaan Grundy Number](https://nyaannyaan.github.io/library/math/grundy-number.hpp) | 算法参考；只适用有限无环正常规则公平游戏，组合状态另用直接胜负 DP 验证。 |
| `template/others/CartesianTree.cpp` | [Cartesian Tree](https://judge.yosupo.jp/problem/cartesian_tree) | 直接；相等元素的决策、根父亲的输出形式按题目转换。 |
| `template/others/DLX.cpp` | [P4929 Dancing Links](https://www.luogu.com.cn/problem/P4929) | 直接；精确覆盖第一解，输入行的列编号须合法且不重复；不是重复覆盖或带权最小覆盖。 |
| `template/others/ConvexHullTrick.cpp` | [P3195 HNOI2008 玩具装箱](https://www.luogu.com.cn/problem/P3195) | 应用；先展开 DP 转移成直线最小值。模板要求斜率不增、查询横坐标不减，不能直接用于任意顺序直线。 |
| `template/others/CDQ.cpp` | [P3810 三维偏序 / 陌上花开](https://www.luogu.com.cn/problem/P3810) | 直接能力；返回每个点被其他点支配的数量，题目最终需要统计数量分布。 |
| `template/others/Common.cpp` | 无独立原题；`chkmin`、`chkmax`、模加减和类型别名 | 辅助定义；烟测中的断言验证边界，不拿 A+B 题冒充验证题。 |
| `template/others/Discretize.cpp` | [P1955 程序自动分析](https://www.luogu.com.cn/problem/P1955) | 应用；离散化编号后配合 DSU，离散化本身不是整题算法。 |
| `template/others/DPOpt.cpp` | [CF 321E Ciel and Gondolas](https://codeforces.com/problemset/problem/321/E)、[P1776 宝物筛选](https://www.luogu.com.cn/problem/P1776) | 分别对应分治 DP 单层转移与单调队列多重背包；前者必须证明最优决策单调并自行准备代价函数。 |
| `template/others/Mo.cpp` | [P1494 小 Z 的袜子](https://www.luogu.com.cn/problem/P1494) | 应用；补全窗口增删与记录答案函数，维护颜色计数、同色点对数与分数约分，单点区间单独处理。 |
| `template/others/MoModify.cpp` | [P1903 带修莫队 / 数颜色](https://www.luogu.com.cn/problem/P1903) | 应用；补全增删与记录答案函数，维护不同值个数，颜色建议离散化。 |
| `template/others/MoTree.cpp` | [SPOJ COT2 Count on a tree II](https://www.spoj.com/problems/COT2/)、[洛谷收录题面](https://www.luogu.com.cn/problem/SP10707) | 应用；静态路径颜色数，补全按点增删与记录答案函数。原站返回 403，已核对洛谷题面；不是 P4074 的带修改树上莫队。 |
| `template/others/MonotonicStack.cpp` | [P5788 单调栈](https://www.luogu.com.cn/problem/P5788) | 直接；greater 设为 true，取 right，不存在时把 n + 1 转为题目的 0。 |
| `template/others/OverallBinary.cpp` | [P2617 Dynamic Rankings](https://www.luogu.com.cn/problem/P2617) | 直接；离线带修改区间第 k 小，按时间记录操作。 |
| `template/others/SlidingWindow.cpp` | [P1886 单调队列 / 滑动窗口](https://www.luogu.com.cn/problem/P1886) | 直接；数组和结果按源码的 0-based 约定使用。 |
| `template/others/SparseTable.cpp` | [Static RMQ](https://judge.yosupo.jp/problem/staticrmq) | 适配；模板固定取 max，没有比较器参数。此题数值可安全取负，建表前取负、查询后取负；半开区间转闭区间。 |
| `template/others/TernarySearch.cpp` | [P3382 三分](https://www.luogu.com.cn/problem/P3382) | 应用；传入多项式求值，必须单峰，题面本身有精度警告。 |
| `template/others/TreeHash.cpp` | [P5043 树同构 / BJOI2015 树的同构](https://www.luogu.com.cn/problem/P5043) | 应用；模板是有根树哈希，题面无根树需先处理重心或枚举根。固定哈希有碰撞风险。 |

## 计算几何（13 个）

二维几何先复制 `Point.cpp`，再按文件头复制依赖；`ConvexHull3D.cpp` 独立使用 `P3`，不依赖二维定义。不引入自定义头文件。下列题目覆盖对应能力，不代表一个题验证了整份文件中的所有函数。

| 文件 | 原题 / 验证题 | 对应关系与注意事项 |
| --- | --- | --- |
| `template/geometry/MinRectangle.cpp` | [CGAL Minimum Rectangle](https://doc.cgal.org/latest/Bounding_volumes/group__PkgBoundingVolumesRef.html) | 算法参考；严格逆时针凸包，另测 0 / 1 / 2 点退化；枚举所有边方向作真值，不声称精确实数几何。 |
| `template/geometry/Circle.cpp` | [AOJ CGL_7_D 圆与直线交点](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_D)、[CGL_7_E 两圆交点](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_E)、[CGL_7_F 点到圆切线](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_F)、[CGL_7_G 公切线](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_G)、[CGL_7_I 两圆交面积](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_I) | 分别对应交点、切点、公切线与交面积；无穷多交点不返回无限列表，退化圆按源码约定。 |
| `template/geometry/ClosestPair.cpp` | [Closest Pair](https://judge.yosupo.jp/problem/closest_pair) | 直接；返回原输入点对下标，少于两点时为 `{-1, -1}`。 |
| `template/geometry/CircleUnion.cpp` | [Green's Theorem](https://sites.science.oregonstate.edu/BridgeBook/talk/book/sguide/green) | 算法参考；面积用解析值与独立竖切片积分对拍，周长用解析值与独立圆弧采样验证；支持包含、重复圆和洞，浮点近退化须另核精度。 |
| `template/geometry/ConvexHull3D.cpp` | [CGAL 3D Convex Hulls](https://doc.cgal.org/latest/Convex_hull_3/index.html) | 算法参考；独立增量实现，dim < 3 时三角面为空、面积体积为 0，不返回平面凸包面积。 |
| `template/geometry/ConvexHull.cpp` | [Static Convex Hull](https://judge.yosupo.jp/problem/static_convex_hull)、[AOJ CGL_4_B Diameter of a Convex Polygon](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_B) | 凸包和直径分别对应；共线边界点规则及输出起点需转换。闵可夫斯基和另做点对和凸包对拍。 |
| `template/geometry/ConvexQuery.cpp` | [CF 166B Polygons](https://codeforces.com/problemset/problem/166/B)、[AOJ CGL_3_C Polygon-Point Containment](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_C) | 前者逐点判断严格内部，边界不接受；后者只适合凸多边形子任务。`build` 会取凸包，不能用于一般凹多边形。 |
| `template/geometry/HalfPlane.cpp` | [P4196 半平面交 / CQOI2006 凸多边形](https://www.luogu.com.cn/problem/P4196) | 直接；有向边保留左侧，求交后取面积。无界与退化交返回空。 |
| `template/geometry/Line.cpp` | [AOJ CGL_1_A Projection](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_1_A)、[CGL_2_B Intersection](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_B)、[CGL_2_D Distance](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_D) | 分别验证投影、线段相交与距离；直线端点不能重合，求两直线交点必须不平行。 |
| `template/geometry/MinCircle.cpp` | [P1742 最小圆覆盖](https://www.luogu.com.cn/problem/P1742) | 直接；随机增量与浮点判定，实际精度另须检查，不以随机小整数测试保证所有实数退化输入。 |
| `template/geometry/Point.cpp` | [Sort Points by Argument](https://judge.yosupo.jp/problem/sort_points_by_argument) | 基础定义；分别用极角排序、几何恒等式和下游几何测试验证，不是独立完整解题代码。 |
| `template/geometry/Polygon.cpp` | [AOJ CGL_3_A Area](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_A)、[CGL_3_C Polygon-Point Containment](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_C)、[CGL_4_C Convex Cut](https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_C) | 分别对应面积、一般简单多边形点包含与凸多边形切割；原切割入口在 `geometry_cut_submission.cpp`。 |
| `template/geometry/RectangleUnion.cpp` | [Area of Union of Rectangles](https://judge.yosupo.jp/problem/area_of_union_of_rectangles)、[P5490 扫描线](https://www.luogu.com.cn/problem/P5490)、[P1856 USACO5.5 矩形周长](https://www.luogu.com.cn/problem/P1856) | 面积并与周长并分别验证；不把面积题通过当周长题通过。 |

## 链接维护

```sh
python3 support/verify/check_sources.py
python3 support/verify/check_sources.py --network
```

第一条核对文件一一对应、源码哈希和测试入口；第二条额外读取 Library Checker 官方固定版本的 `info.toml` 与题面、核对线上内容 SHA-256，保存记录到 `build/source-audit/`。其他站点的人工题面核对与访问限制见本表，不用 HTTP 200 冒充内容校验。

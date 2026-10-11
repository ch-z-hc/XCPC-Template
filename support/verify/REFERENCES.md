# 新增算法的参考资料

本次 14 个片段均按项目接口和个人码风整理，不包含题目入口。论文链接用于核对算法，不表示复制论文代码；公开题链接与实际通过记录分开维护。

| 片段 | 算法 / 实现参考 |
| --- | --- |
| `PersistentXorTrie.cpp` | 路径复制持久化；[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)。 |
| `KShortestWalk.cpp` | Eppstein，[Finding the k Shortest Paths](https://www.ics.uci.edu/~eppstein/pubs/Epp-SJC-98.pdf)；使用反向最短路树和可持久化左偏堆。 |
| `LowerBoundMinCostFlow.cpp` | [Minimum Cost b-flow 官方定义](https://github.com/yosupo06/library-checker-problems/blob/master/graph/min_cost_b_flow/task.md)；复用本库 MCMF，负边先饱和再修正平衡。 |
| `Min25.cpp` | Min_25 的素数多项式筛表和素数幂枚举；[官方题目与参考解](https://github.com/yosupo06/library-checker-problems/tree/master/number_theory/sum_of_multiplicative_function_large)。本片段独立实现固定二次素数表，不复制其模整数、优化除法或 DFS 实现。 |
| `Simplex.cpp` | Stanford Notebook / [KACTL Simplex](https://github.com/kth-competitive-programming/kactl/blob/main/content/numerical/Simplex.h)，MIT；改为独立结构体、long double、状态返回，加入零尺寸处理和 Bland 入基规则。 |
| `MaxClique.cpp` | Tomita、Seki，[An Efficient Branch-and-Bound Algorithm for Finding a Maximum Clique](https://doi.org/10.1007/3-540-45066-1_22)；独立实现染色上界，不复制 KACTL 的 GPL 实现。 |
| `CircleUnion.cpp` | [Green's Theorem](https://sites.science.oregonstate.edu/BridgeBook/talk/book/sguide/green)，对圆并边界未覆盖弧积分。 |
| `KDTree.cpp` | [CGAL Spatial Searching](https://doc.cgal.org/latest/Spatial_searching/index.html)、[KACTL kdTree](https://github.com/kth-competitive-programming/kactl/blob/main/content/geometry/kdTree.h)；独立静态二维实现，不复制 CGAL 代码。 |
| `DLX.cpp` | Knuth，[Dancing Links](https://arxiv.org/abs/cs/0011047)，Algorithm X 与可逆十字链。 |
| `Dominator.cpp` | Lengauer、Tarjan，[A Fast Algorithm for Finding Dominators in a Flowgraph](https://doi.org/10.1145/357062.357071)。 |
| `WeightedMatching.cpp` | [bqi343/cp-notebook GeneralWeightedMatch.h](https://github.com/bqi343/cp-notebook/blob/master/Implementations/content/graphs%20(12)/Matching/GeneralWeightedMatch.h)，CC0-1.0；改为动态存储、ll、可重复 solve，保留 Blossom 收缩、展开与对偶调整算法。 |
| `ConvexHull3D.cpp` | [CGAL 3D Convex Hulls](https://doc.cgal.org/latest/Convex_hull_3/index.html)；独立增量实现，不复制 CGAL 代码，不扰动输入。 |
| `Lyndon.cpp` | Duval 算法；[Lyndon Factorization 官方题面与参考解](https://github.com/yosupo06/library-checker-problems/tree/master/string/lyndon_factorization)。独立实现字符串字节比较与分界点返回。 |
| `Runs.cpp` | Bannai 等，[The Runs Theorem](https://arxiv.org/abs/1406.0263)；独立实现两种字符序的 Lyndon 根，复用本库后缀数组，不复制官方分治 Z 算法。 |

## Simplex 的 MIT 许可

原文件作者标注为 Stanford，来源 Stanford Notebook，License: MIT。

```text
MIT License

Copyright holders: the original Stanford Notebook contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

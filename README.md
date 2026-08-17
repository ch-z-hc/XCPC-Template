# XCPC Template

## 约定

- 算法片段放在分类目录中，默认不包含 `int main()`。
- `int main()` 只保留在比赛入口或验证程序中。
- 每个片段应在开头注明适用条件、复杂度、下标约定和调用方式。
- 对拍程序属于验证工具，可以保留自己的 `main()`，但不要混入算法片段。

## To Do 对应关系

Microsoft To Do 中的每个模板任务，对应一个可直接复制的 `.cpp` 片段。
完成一个模板的标准是：代码整理、接口说明、验证题和一次编译/运行验证都完成。

## 调用方式

这些文件是可直接复制的代码片段，不依赖自定义头文件。复制需要的 `.cpp` 内容到比赛代码中即可；每个算法文件都不包含 `int main()`。

唯一的 `main()` 在 `verify/` 中，用于烟测。正式比赛代码自行保留一个入口。

## 文档站

文档站使用 MkDocs Material，并从模板 `.cpp` 文件自动生成页面：

```text
python -m pip install -r requirements-docs.txt
mkdocs serve
```

严格构建使用 `mkdocs build --strict`。`main` 分支更新后，GitHub Actions 会构建并部署到 GitHub Pages；首次部署前需在仓库设置中将 Pages 来源设为 GitHub Actions。

## 对拍工具

`tools/stress/` 包含随机数据生成器、暴力程序、标准程序和统一运行脚本。当前示例对拍非空最大子段和：

```text
python tools/stress/run.py --tests 1000 --seed 1
```

发现差异时，失败种子、输入和两份输出会保存在 `build/stress/`。修改三个 `.cpp` 文件即可用于具体题目，详细约定见文档站的“对拍工具”页面。

## 接口索引

| 文件 | 对外接口 |
| --- | --- |
| `ds/LiChao_SegT.cpp` | `LiChaoTree::add_line`、`add_segment`、`query_line`、`query` |
| `ds/disjoint_set_union.cpp` | `DisjointSetUnion::unite`、`same`、`component_size` |
| `ds/fenwick_tree.cpp` | `FenwickTree::add`、`prefix_sum`、`range_sum` |
| `ds/rollback_dsu.cpp` | `RollbackDSU::unite`、`snapshot`、`rollback` |
| `ds/segment_tree.cpp` | `SegmentTree::build`、`add`、`query` |
| `geometry/geo.cpp` | `Point`、`segments_intersect`、`convex_hull`、`convex_diameter`、`minkowski_sum` 等 |
| `graph/BigraphMatch.cpp` | `BipartiteMatcher::add_edge`、`max_matching` |
| `graph/CutEdge.cpp` | `BridgeFinder::add_edge`、`find` |
| `graph/CutVertex.cpp` | `ArticulationFinder::add_edge`、`find` |
| `graph/DiffConstraints.cpp` | `DifferenceConstraints::add_constraint`、`solve` |
| `graph/Dijkstra.cpp` | `dijkstra` |
| `graph/DSUOnTree.cpp` | `DSUOnTree::add_edge`、`build` |
| `graph/Hierholzer_D.cpp` | `directed_euler_trail` |
| `graph/Hierholzer_UnD.cpp` | `undirected_euler_trail` |
| `graph/kruskal.cpp` | `kruskal_mst` |
| `graph/LCA.cpp` | `BinaryLCA::add_edge`、`build`、`lca` |
| `graph/MaxFlow.cpp` | `Dinic::add_edge`、`max_flow` |
| `graph/MaxFlowMinMaxCost.cpp` | `MinCostMaxFlow::add_edge`、`flow` |
| `graph/SCC.cpp` | `SCC::add_edge`、`build`、`component` |
| `math/AdaptiveSimpson.cpp` | `adaptive_simpson` |
| `math/C(n, m).cpp` | `CombinationMod::init`、`choose` |
| `math/FWT.cpp` | `fwt_or`、`fwt_and`、`fwt_xor` 及逆变换 |
| `math/NTT.cpp` | `ntt`、`polynomial_multiply` |
| `math/O(n)'s Prime.cpp` | `linear_primes` |
| `math/O(n)'s multiplicative function.cpp` | `LinearSieve::build` |
| `math/sqrt-decomposition.cpp` | `quotient_blocks` |
| `math/xorbasis.cpp` | `XorBasis::insert`、`can_represent`、`max_xor` |
| `others/CartesianTree.cpp` | `CartesianTree::build` |
| `others/Mo's algorithm.cpp` | `run_mo` |
| `others/MonotonousQueue.cpp` | `sliding_window_minimum`、`sliding_window_maximum` |
| `others/S.T..cpp` | `SparseTable::build`、`query` |
| `others/template.cpp` | `chmin`、`chmax` 及常用类型别名 |
| `others/ThirdMid.cpp` | `ternary_min`、`ternary_max` |
| `string/AC-automaton.cpp` | `AhoCorasick::add_pattern`、`build`、`count_occurrences` |
| `string/KMP.cpp` | `prefix_function`、`kmp_search` |
| `string/Manacher.cpp` | `Manacher::build`、`is_palindrome`、`longest_palindrome` |
| `string/SAM.cpp` | `SuffixAutomaton::build`、`occurrence_count`、`longest_repeated_product` |
| `string/Trie.cpp` | `Trie::insert`、`prefix_count`、`word_count` |
| `others/discretization.cpp` | `discretize` |
| `string/SA.cpp` | `SuffixArray::build`、`lcp` |
| `graph/olca.cpp` | `O1LCA::add_edge`、`build`、`lca` |
| `graph/reroot.cpp` | `tree_distance_sums` |
| `graph/tree_diameter.cpp` | `tree_diameter` |
| `graph/topological_sort.cpp` | `topological_sort` |
| `others/tree_hash.cpp` | `rooted_tree_hash` |
| `math/matrix.cpp` | `matrix_mul`、`matrix_pow` |
| `math/interpolation.cpp` | `interpolation_eval` |
| `math/bm.cpp` | `berlekamp_massey`、`linear_recurrence_kth` |

## 约定

- 图和树模板通常使用 1-based 顶点编号；具体例外写在文件注释中。
- `math/NTT.cpp` 使用模数 `998244353`。
- `math/interpolation.cpp` 要求横坐标互异且模数为质数。
- `graph/tree_diameter.cpp` 的边权默认为非负数。
- `graph/reroot.cpp` 是“所有点到所有点距离和”的换根 DP 实例；其他换根转移需按题目修改。
- `graph/DSUOnTree.cpp` 通过加点、删点和读取答案回调维护题目状态；回调必须支持可逆更新。
- `others/tree_hash.cpp` 是随机化哈希，不能当作严格同构判定。

## 待补模板

- `凸包计数`：待确认具体题型后实现；当前 `geometry/geo.cpp` 只有基础凸包、凸包切割和旋转卡壳等操作。

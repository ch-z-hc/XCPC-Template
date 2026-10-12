# 竞赛接口审计

日期：2026-10-12。扫描范围为 `template/*/*.cpp` 的全部 145 个模板。

## 改动范围

18 个模板存在 `optional`，现已全部移除。失败判断改为 `bool` 加输出参数；只有合法值均非负的逆元、模平方根和斯坦纳树使用 `-1`。李超树额外移除 `if constexpr`、`is_integral_v`、`conditional_t`，改为明确的整数与浮点插入接口。最小圆覆盖同步调用新的外接圆接口。

返回 `false` 时输出参数无保证，不应读取；返回 `true` 才代表成功。合法的零、负数、整数极值和空解均不充当失败标记。模板不保留旧接口兼容层，复制调用代码时应按下表一起修改。

保留算法确实需要的宽整数计算；差分约束仍仅使用 `ll`。保留常用 `vector`、`pair`、`array`、`auto`、lambda、结构化绑定和简单函数模板，不扩大为对 C++17 语法的全面重写。测试程序内部的独立暴力真值不属于可复制模板。

## 逐项审计

| 模板 | 当前接口与失败语义 | 随机真值与特殊数据 |
| --- | --- | --- |
| `math/扩展欧几里得与中国剩余定理.cpp` | `inv(a, mod)` 无逆元返回 `-1`；`crt(r, m, ans, mod)` 返回是否成功 | 枚举最小解、扩欧恒等式；空方程组、模数 1、负余数、非互质冲突、lcm 溢出、`LLONG_MIN/MAX`。入口 `new_math_random`。 |
| `math/多项式基本运算.cpp` | `FPS::sqrt(a, n, ans)` 返回是否有根 | 朴素卷积复平方、二次剩余判定；`n = 0`、零多项式、奇数零前缀、非剩余、截断平方。入口 `extra_math_random`。 |
| `math/矩阵求逆.cpp` | `matrix_inv(a, mod, b)` 返回是否可逆 | 排列枚举行列式、左右矩阵乘积；空矩阵、零矩阵、换主元、负元素、模数 2 和 `INT_MAX`。入口 `priority_templates_random`。 |
| `math/二次剩余.cpp` | `sqrt_mod(a, p)` 无根返回 `-1` | 小质数枚举、随机平方；合法根 0、模数 2、负输入、`LLONG_MIN`。入口 `extra_math_random`。 |
| `math/异或线性基.cpp` | `Basis::kth(k, ans)` 越界返回 `false` | 枚举不同子集异或值排序；空基第 0 项、满秩 63 位、`LLONG_MAX` 合法结果、`ULLONG_MAX` 下标越界。入口 `extra_math_random`。 |
| `ds/无旋平衡树.cpp` | `kth(k, ans)`、`pre(x, ans)`、`suf(x, ans)` 返回是否存在 | `multiset` 真值；空集、重复键、严格前驱后继、非法次序、负数与 `LLONG_MIN/MAX`。入口 `new_ds_random`。 |
| `ds/二维最近邻.cpp` | `nearest(x, y, ans, exclude = -1)` 返回是否找到点 | 扫描全部点；空集、排除唯一点、等距取较小下标、重复点、距离 `8e18`、大规模建树。入口 `kdtree_random`。 |
| `ds/可持久化二进制字典树.cpp` | `max_xor(left, right, x, ans)` 空差集返回 `false` | 扫描版本祖先链及前缀区间；分叉历史、空版本差、0、最高位、完整 `ULLONG_MAX` 答案。入口 `persistent_xor_trie_random`。 |
| `ds/带权与异或并查集.cpp` | `WDSU/XDSU::query(u, v, ans)` 不连通返回 `false` | BFS 求差值 / 异或关系；断开、自点、矛盾约束、负差值、最高位与重复初始化。入口 `new_ds_random`。 |
| `ds/二进制字典树.cpp` | `max_xor(x, ans)` 空集返回 `false` | 多重集扫描最大异或；空集、重数删除、0、最高位及 `ULLONG_MAX`。入口 `new_ds_random`。 |
| `graph/最小树形图.cpp` | `DMST::solve(root, ans)` 无树形图返回 `false` | 穷举每点入边并检查从根可达；单点、自环、负权环缩点、不可达、合法 `LLONG_MAX` 权值、重复求解。入口 `extra_graph_random`。 |
| `graph/重构树.cpp` | `query(u, v, ans)` 不连通返回 `false` | Floyd 极小极大 / 极大极小路径；负权、自点 0、非连通图、切换建树方向、递归深链。入口 `advanced_tree_random`、`deep_stack`。 |
| `graph/重赋权全源最短路.cpp` | `solve(dis)` 全图负环返回 `false` | 独立宽整数 Floyd；空图、负边无负环、不可达负环、重边、自环和重复调用。入口 `extra_graph_random`。 |
| `graph/最小斯坦纳树.cpp` | `solve(terminals)` 不连通返回 `-1` | 穷举边集判断终端连通；空终端、单点、重复终端、零权、大权、孤立点。入口 `more_graph_random`。 |
| `graph/拓扑排序.cpp` | `topo(g, order)` 有环返回 `false` | 传递闭包判环、逐边检查拓扑序；空图、自环、重边、断开图。入口 `audit_gaps_random`、`common_templates_random`。 |
| `others/舞蹈链精确覆盖.cpp` | `solve(rows)` 无精确覆盖返回 `false` | 穷举行集、检查每列恰好覆盖一次；0 列空解、无解、重复求解和求解前后链结构完全恢复。入口 `dlx_random`。 |
| `others/博弈状态函数.cpp` | `grundy(g, sg)` 有向环返回 `false` | 独立双子游戏胜负 DP、mex 集合；空图、终点 SG 为 0、重边、自环、深链闭环。入口 `completion_structures_random`、`deep_stack`。 |
| `geometry/圆.cpp` | `circumcircle(a, b, c, ans)` 共线返回 `false` | 固定外接圆几何关系及随机最小圆覆盖；共线、重复点、退化点集。入口 `new_geometry_random`。 |
| `geometry/最小圆覆盖.cpp` | `min_circle` 不变，内部直接接收外接圆输出 | 随机点集对照点、点对、三点候选圆；空集、共线和重复点。入口 `new_geometry_random`。 |
| `ds/李超线段树.cpp` | 整数 `add_line/add_seg`；浮点 `add_line_real/add_seg_real` | 逐横坐标扫描直线 / 线段；整数用有理数交叉乘真值、浮点微小差、端点同值优先较早编号、竖直线段、反向端点、`INT_MIN/MAX` 横坐标。入口 `li_chao_random` 及独立 Python 精确有理数测试。 |

## 复验记录

本轮全部完成，以下结果均为改动后重新执行，不引用旧版本通过记录。

- `snippets_smoke.cpp` 使用 C++17、`-O2 -Wall -Wextra -pedantic` 编译并运行，退出 0。
- `bash support/verify/run_linux.sh` 完整退出 0：145 个模板及工具 / 验证入口语法检查通过；45 个标准验证程序通过 ASan / UBSan / 标准库断言；独立 Python 整数真值通过 400 组李超树数据；对拍工具的正常、错误答案、崩溃与超时保存流程全部通过。
- salt `2026101212` 追加重播 `new_ds_random`、`extra_math_random`、`extra_graph_random`、`kdtree_random`、`li_chao_random`，5 个入口均通过，启用 ASan / UBSan。
- `python3 support/verify/run_public.py` 退出 0：20 题、409 组本地官方生成数据全部通过。树形图只比较最小权，其余使用官方 checker；不是在线提交。
- `print_book_check.py` 退出 0：147 项、232 页，源码行完整且版面检查通过；目视检查第 7、23、24、56、128、145、159、160、161、201、204 页，无裁切或乱码。
- 扫描模板中的 `optional/nullopt`、异常、4 种具名类型转换、类型萃取、`variant`、智能指针及 `std::function`，无匹配。`git diff --check` 通过。

证据目录：`build/contest-interfaces-20261012/`。完整流程为 `full-linux.log`，官方数据为 `official.log` 与 `official-results.json`；5 个专项各有 `reseed-*.log`，详细结果位于 `build/audit-20261012/reseed-2026101212-*/results.json`。首次循环调用因 shell 参数转义得到空参数，尚未执行测试，原错误保留在 `reseed.log`；上述 5 个独立调用均正常退出 0。

`source-index.json` 中 `template_sha256` 保存全部 145 个模板的 SHA-256，结束时逐文件核对无漂移。打印生成与检查见 `print-generate-final.log`、`print-check-final.log`。正式打印册与根目录同名副本的 SHA-256 均为 `7c7cb30f0d0522d2666c6ad1184fefeb35e86c50aad7722607bd905e10ac4039`。

旧逐模板审计继续保留历史真值来源与测试范围，但其中旧调用示例、旧哈希和旧打印页码不代表当前接口。当前接口以源码、主 README 和本文为准。本轮在声明输入域与上述随机 / 特殊数据内未发现新反例；有限测试不等于对所有输入的绝对正确性证明。

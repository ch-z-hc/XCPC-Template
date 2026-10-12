# 模板工程化语法检查清单

后续处理：按用户确认，已将 `Mo / MoModify / MoTree / DSUOnTree` 的多回调接口改为普通成员函数；`self(self, ...)` 和单回调接口保留。以下为改动前的审查快照，统计和原行号不再代表改动后的源码；当前接口以源码与 README 为准。

2026-10-13 继续按用户确认移除比赛模板中的 `tie`、`move`，改为普通比较、`swap` 或原位构造；宽整数保留，别名统一使用 `__int128_t` / `__uint128_t`，不再使用 `__extension__`。具体处理与验证见 [打印前最终审查](AUDIT_PRINT_USABILITY_20261013.md)。

日期：2026-10-12。范围：当前工作区 `template/` 下全部 145 个 `.cpp`，不把验证程序和打印脚本算作比赛模板。

本轮只新增本清单，没有修改算法、接口、测试、README 或打印册。检查的是当前未提交版本，不是 Git HEAD。以下是风格整理候选，不是算法错误清单；“工程化”没有严格语法边界，因此把可能让你不顺手的现代 C++ 写法也一起列出，并区分用途。

检查顺序：全库语法扫描 → 阅读涉及泛型回调、递归 lambda、工具类和多层封装的实现 → 分类列出原位置。按 ponytail 的精简原则和 chzhc-xcpc-style 判断，不能因为语法较新就认定它多余。

## 先看这些

### 1. 多个回调参数的泛型接口：最值得先整理

`template < class Add, class Remove, class Answer >` 把具体维护逻辑交给调用者，算法中反复调用传入的函数对象。

| 位置 | 当前形式 | 可选整理方向 |
| --- | --- | --- |
| [template/others/Mo.cpp](D:/OI/template/template/others/Mo.cpp:13) | `mo(n, qs, add, remove, query)` | 直接写 `add / del / query`，使用时修改函数体。 |
| [template/others/MoModify.cpp](D:/OI/template/template/others/MoModify.cpp:31) | `solve(add, del, answer)` | 改为普通成员函数处理增删、修改与答案。 |
| [template/others/MoTree.cpp](D:/OI/template/template/others/MoTree.cpp:53) | `solve(add, del, answer)` | 改为普通成员函数处理点统计。 |
| [template/graph/DSUOnTree.cpp](D:/OI/template/template/graph/DSUOnTree.cpp:35)、[递归层](D:/OI/template/template/graph/DSUOnTree.cpp:57) | `build` 和 `dfs` 都携带三个回调类型 | 直接维护状态，调用普通 `add / del / query`。 |

这些是实质上的通用框架，不是单纯用了 `auto`。去掉后会从“直接传回调即可运行”变成“复制后按题目填写维护逻辑”；这是使用方式变化，不应假装完全无影响。不要替换成 `std::function`，那会增加另一层间接调用。

### 2. 单个函数对象参数：是否保留取决于使用习惯

| 位置 | 当前形式 | 可选整理方向 |
| --- | --- | --- |
| [template/graph/HLD.cpp](D:/OI/template/template/graph/HLD.cpp:60) | `template < class F > path(u, v, F f) const` | 在跳链循环里直接查询或修改线段树；代价是 HLD 不再独立于区间维护。 |
| [template/others/DPOpt.cpp](D:/OI/template/template/others/DPOpt.cpp:13) | `dc_dp(prev, Cost cost, inf)` | 普通 `cost(l, r)` 加普通分治函数。 |
| [template/math/AdaptiveSimpson.cpp](D:/OI/template/template/math/AdaptiveSimpson.cpp:13) | `simpson(F f, ...)` | 普通被积函数 `f(x)`，普通 Simpson 与递归函数。 |
| [template/math/Min25.cpp](D:/OI/template/template/math/Min25.cpp:71) | `sum(c, F prime_power) const` | 普通成员函数计算 `f(p^e)`。 |
| [template/others/TernarySearch.cpp](D:/OI/template/template/others/TernarySearch.cpp:11)、[最大值版](D:/OI/template/template/others/TernarySearch.cpp:20) | `ternary_min/max(F f, ...)` | 普通 `f(x)`。 |

它们都有实际用途，不属于纯粹摆设；如果你更习惯“复制后改函数”，就可以去掉这层通用参数。

### 3. 自传参递归 lambda：12 个文件、13 处

写法是 `auto dfs = [&](auto &&self, ...) -> ...`，递归调用为 `self(self, ...)`。`auto &&` 在这里用于泛型 lambda 的引用参数，并没有调用 `forward`；不能把它误写成已经使用了完美转发。

完整位置：

| 文件 | 声明行 |
| --- | --- |
| [template\graph\DirectedEulerTrail.cpp](D:/OI/template/template/graph/DirectedEulerTrail.cpp:31) | `auto dfs = [&](auto &&self, int u) -> void {` |
| [template\graph\Kruskal.cpp](D:/OI/template/template/graph/Kruskal.cpp:32) | `auto find = [&](auto &&self, int x) -> int {` |
| [template\graph\Reroot.cpp](D:/OI/template/template/graph/Reroot.cpp:20) | `auto dfs1 = [&](auto &&self, int u, int fa) -> void {` |
| [template\graph\Reroot.cpp](D:/OI/template/template/graph/Reroot.cpp:27) | `auto dfs2 = [&](auto &&self, int u, int fa) -> void {` |
| [template\graph\TreeDiameter.cpp](D:/OI/template/template/graph/TreeDiameter.cpp:17) | `auto dfs = [&](auto &&self, int u, int fa, ll dis) -> void {` |
| [template\graph\UndirectedEulerTrail.cpp](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:32) | `auto dfs = [&](auto &&self, int u) -> void {` |
| [template\math\AdaptiveSimpson.cpp](D:/OI/template/template/math/AdaptiveSimpson.cpp:21) | `auto solve = [&](auto &&self, double a, double b, double s,` |
| [template\math\BSGS.cpp](D:/OI/template/template/math/BSGS.cpp:12) | `auto dfs = [&](auto &&self, ll a_, ll b, ll &x, ll &y) -> void {` |
| [template\math\Min25.cpp](D:/OI/template/template/math/Min25.cpp:84) | `auto dfs = [&](auto &&self, ll x, int k) -> ll {` |
| [template\math\PollardRho.cpp](D:/OI/template/template/math/PollardRho.cpp:73) | `auto dfs = [&](auto &&self, ull x) -> void {` |
| [template\others\CDQ.cpp](D:/OI/template/template/others/CDQ.cpp:41) | `auto cdq = [&](auto &&self, int l, int r) -> void {` |
| [template\others\DPOpt.cpp](D:/OI/template/template/others/DPOpt.cpp:17) | `auto solve = [&](auto &&self, int l, int r, int lo, int hi) -> void {` |
| [template\others\SG.cpp](D:/OI/template/template/others/SG.cpp:16) | `auto dfs = [&](auto &&self, int u) -> bool {` |

可以换成普通递归函数或已有结构体的成员函数；但不能简单移出函数体后丢掉捕获的数组、图、模数等状态，也不要为了去掉 lambda 另造一套框架。按项目约定，继续保持递归，不换显式栈。

### 4. 只装静态函数的结构体

- [template/math/FPS.cpp](D:/OI/template/template/math/FPS.cpp:20)：`struct FPS` 内全部是静态函数和静态模数，通过 `FPS::inv / mul / ln` 等调用。
- [template/math/FFT.cpp](D:/OI/template/template/math/FFT.cpp:61)：`struct ModConv` 装三个静态函数，通过 `ModConv::mul / power` 调用。

这是工具函数分组，不是有状态的数据结构。如果不喜欢，可以改普通函数。好处是去掉 `static` 和限定名；代价是处理 `mul / inv / ntt / power` 等重名，并同步 [template/math/Multipoint.cpp](D:/OI/template/template/math/Multipoint.cpp:31) 等调用。直接拆开未必减少代码行数。

### 5. 较深的内部封装

- [template/string/Hash2D.cpp](D:/OI/template/template/string/Hash2D.cpp:15)：`Hash2D::Table` 封装单模哈希，再用两个 `Table` 组合双模。确实有两次复用，不建议仅为“少一层”复制两份实现。
- [template/graph/LowerBoundFlow.cpp](D:/OI/template/template/graph/LowerBoundFlow.cpp:22)：`LRFlow::Net` 内再定义残量边 `Net::Edge`，外面另有原始边 `LRFlow::Edge`。可以整理，但原边和残量边是不同数据，不能直接合并概念。
- [template/graph/ScalingMinCostFlow.cpp](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:42)：`flow` 内局部 `Arc`，配合 `add_pair / reduced / dijkstra / add_one` 四个捕获 lambda。若感觉嵌套太多，可以将算法状态和辅助操作改为成员；现在这么写是为了每次求解使用局部状态，不消耗原始边。

普通 `Node / Edge / Query / Factor` 内嵌类型符合项目约定，不自动算作过度封装。

### 6. 专门的结果结构体

- [template/graph/Kruskal.cpp](D:/OI/template/template/graph/Kruskal.cpp:18)：`MST { sum, ids, ok }`。
- [template/math/Simplex.cpp](D:/OI/template/template/math/Simplex.cpp:17)：`Result { status, value, x }`。
- [template/geometry/MinRectangle.cpp](D:/OI/template/template/geometry/MinRectangle.cpp:13)：`MinRect` 保存面积和矩形顶点。

若不喜欢“结果对象”，可以改返回状态加输出参数；但这些都是多个实际结果，字段名也比多层 `pair` 清楚。没有发现为返回单个数值单独设计的复杂结果包装。

## 其余语法：一起列出，但不应一刀切

| 类别 | 当前位置或规模 | 判断与整理注意事项 |
| --- | --- | --- |
| 普通类型模板 `template < class T >` | `Common / Discretize / LIS / SlidingWindow`，详见附录 | 可以固定 `int / ll`，但会失去现有类型通用性。`chkmin / chkmax` 本来就是个人常用写法，不建议删。 |
| 非类型模板 `template < size_t N >` | [template/math/Gauss.cpp](D:/OI/template/template/math/Gauss.cpp:79) | 用于 `bitset<N>` 的编译期长度，是实际类型要求；可固定最大宽度，但不是无意义的模板层。 |
| 成员函数尾部 `const` | 扫描命中 117 行；含跨行签名的末行 | 表示不修改对象；不是封装层。若去掉，必须检查 `const` 对象、`const` 成员函数和传参的连锁影响。 |
| 参数 `const T &`、局部 `const` | 各类 `const` 合计 237 行、105 个文件，含上一项 | 大容器的引用避免复制；不能把 `const &` 整体删除成按值传参。 |
| `constexpr / static constexpr` | 12 行、10 个文件 | 常量声明。命名空间常量可写 `const`；类内常量不能不看使用方式就机械替换。 |
| 普通静态辅助函数 | `BerlekampMassey / FWT / Lagrange / TreeHash` | `static` 影响链接，不是访问控制；单文件复制通常不需要，但删掉也不解决同一文件中的重名。 |
| 默认构造函数 `= default` | [template/string/Manacher.cpp](D:/OI/template/template/string/Manacher.cpp:14)、[template/string/SuffixArray.cpp](D:/OI/template/template/string/SuffixArray.cpp:15) | 可写空函数体；已有带参构造函数时，不能直接删除默认构造函数而仍假定支持无参构造。 |
| 构造函数初始化列表 | `Point / Line / DiffConstraints / LowerBoundFlow`，共 4 行 | `P(...) : x(...), y(...)` 是普通初始化。改成函数体赋值未必更简单；容器会先默认构造再赋值。 |
| 构造函数直接调用 `init/build` | 55 行、50 个文件 | 这是普通构造函数里调用函数，不是 C++ 的委托构造函数。可统一手动初始化，但会改变调用方式。 |
| 成员默认值、花括号初始化 | 例如 [template/ds/LCT.cpp](D:/OI/template/template/ds/LCT.cpp:14)、[template/string/Trie.cpp](D:/OI/template/template/string/Trie.cpp:12) | `ch[2]{} / ch{} / n = 0` 保证初值；去掉后必须在所有创建路径补初始化。 |
| `move(...)` | [template/math/BerlekampMassey.cpp](D:/OI/template/template/math/BerlekampMassey.cpp:49)、[template/math/ExLucas.cpp](D:/OI/template/template/math/ExLucas.cpp:39)、[template/graph/VBCC.cpp](D:/OI/template/template/graph/VBCC.cpp:60) | 3 行、4 次调用，避免复制容器；可研究用 `swap` 等价改写，不建议直接删掉而额外深拷贝。 |
| `tie(...)` 多关键字比较 | [template/ds/KDTree.cpp](D:/OI/template/template/ds/KDTree.cpp:31)、[template/ds/KDTree.cpp](D:/OI/template/template/ds/KDTree.cpp:32)、[template/ds/LeftistHeap.cpp](D:/OI/template/template/ds/LeftistHeap.cpp:33) | 可换手写分支；要保留相等时的后续排序条件和编号 tie-break。 |
| `tuple` | [template/graph/DiffConstraints.cpp](D:/OI/template/template/graph/DiffConstraints.cpp:16)、[template/graph/TreeDiameter.cpp](D:/OI/template/template/graph/TreeDiameter.cpp:29)、[template/math/Determinant.cpp](D:/OI/template/template/math/Determinant.cpp:48) | 可用 `Edge` 或结果结构体替换，不影响算法；属于表达偏好。 |
| `array` | 多个 Trie、AC、SAM、PAM，以及 `Min25 / Basis / Runs / FunctionalGraph / CDQ / RectangleUnion` | 可按需要换原生数组，但整个数组的赋值、比较、返回能力不同，不能纯文本替换。 |
| `numeric_limits<T>::infinity()` | `LiChaoTree / ClosestPair / MinRectangle / Simplex` | 浮点无穷大有实际意义；换 `1e18` 会改变数值边界。 |
| `size_t` | `BostanMori / BSGS / ExLucas / Gauss` | 容器大小类型；转为 `int` 前确认长度和加法不会溢出，特别是 `size_t(q) + 1`。 |
| `complex<ld>` | [template/math/FFT.cpp](D:/OI/template/template/math/FFT.cpp:12) | 标准复数库，不是自造框架；换自定义复数反而增加代码。 |
| 随机数与时间种子 | `FHQTreap / MinCircle / ModSqrt / PollardRho` | `mt19937 / mt19937_64 / chrono / uniform_int_distribution` 有算法用途，不建议直接退成 `rand()`。 |
| `__extension__ / __int128 / __int128_t` | 12 个文件 | `__extension__` 是 GNU 扩展诊断抑制；宽整数用于正确计算，不能为了简单统一改为 `ll`。 |
| 数字分隔符 `1'000'...` | [template/graph/ScalingMinCostFlow.cpp](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:38)、[template/graph/ScalingMinCostFlow.cpp](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:39) | 纯字面量写法，可去掉单引号，不改数值。 |
| `assert(...)` | 27 行、10 个文件 | 算法前置条件和范围检查，保留。断言不是复杂错误处理框架，不能当成多余代码统一删。 |
| 重复 `#include <cassert>` | 7 个文件 | 已含 `bits/stdc++.h`，这个头在当前 GCC 环境下冗余，可以单独删；断言本身不删。 |
| `(void)y` | [template/ds/Fenwick2D.cpp](D:/OI/template/template/ds/Fenwick2D.cpp:18) | 仅抑制未使用警告。可改成遍历 `const auto &p` 后取 `p.first`，直接省掉不用的绑定。 |
| `auto`、结构化绑定、范围循环 | `auto` 命中 169 行、71 个文件 | 普通竞赛写法，项目已明确允许。不与递归 lambda、泛型回调混为一谈。 |
| 普通 lambda | 52 个声明起始行，含前述 13 个递归 lambda | 排序比较器、局部快幂等通常很短，单独移出未必更清楚；全部位置见附录。 |
| `__builtin_clz/ctz/popcount*` | `KthAncestor / SubsetConvolution / SparseTable / BitsetLCS` | 编译器位运算内建，不是工程封装；替换时保持非零前提与位宽。 |
| `Graph / Matrix / Complex` 别名 | `Dijkstra / Reroot / TreeDiameter / Matrix / FFT` | 只是缩短已有类型，不是额外对象或包装。 |
| 几何运算符重载 `operator + / - / * / / / < / == / <<` | `Point` 和 `ConvexHull3D` | 向量运算属于常见几何模板写法；按项目要求优先保留原接口。 |
| 一个函数用开关兼顾多种操作 | `LRFlow::solve(..., mode)`、`KruskalTree::build(descending)`、`LIS(strict)`、各种变换的逆变换标记等 | 都对应实际数学操作，不是预留的未知配置。可以拆成明确函数名，但函数数目可能增加。 |
| 公开入口加递归重载 | 例如 [template/ds/MergeSegTree.cpp](D:/OI/template/template/ds/MergeSegTree.cpp:35)、[template/ds/PersistentArray.cpp](D:/OI/template/template/ds/PersistentArray.cpp:54) | `query(root,l,r)` 转入带树区间的递归函数，属于常用板子写法；直接删除入口会把内部参数暴露给调用者。 |
| `bool` 加输出引用、`pair<bool,T>` | 上一轮已整理的 CRT、Trie、DSU、树形图等，以及上下界流 | 用于区分失败与合法的 0、负数、极值、空解；不能统一塞回 `-1`。 |

本表中的“行数”统计源码命中行，不是表达式调用次数；同一行和同一文件可能命中多个类别。成员默认值、普通函数重载与容器操作是抽样说明，不是完整 C++ 语法树审计。附录逐行穷举的是表中明确列出的扫描类别。

## 当前没有检出的写法

对全部 145 个模板重新扫描，没有检出：

- `optional / nullopt / variant`。
- `if constexpr`、`enable_if`、`conditional_t`、`is_integral_v`、`decltype`。
- `static_assert`、`noexcept`、`explicit`、`friend`。
- `std::function`、`bind(...)`、`forward(...)` 函数调用。`Runs` 和 `MoModify` 中有名为 `forward` 的普通变量，不属于完美转发。
- `unique_ptr / shared_ptr / weak_ptr`。
- 继承声明、`virtual / override`、`private / protected / public` 访问标签。
- `throw / try / catch` 异常处理。
- `static_cast / reinterpret_cast / const_cast / dynamic_cast`。
- 自定义头文件引用、自定义命名空间。
- `new / delete / malloc / free` 动态内存管理接口。

“没有检出”指当前算法模板的源码扫描，不代表测试程序或工具脚本也没有，也不是对任意宏展开形式的证明。

## 建议整理顺序

1. 先决定是否去掉莫队、DSU on Tree 的多个回调参数。
2. 再决定单回调和 `self(self, ...)` 是否统一改为普通函数。
3. 再挑 `FPS / ModConv` 的静态工具类、默认构造函数写法、`tie` 等个人偏好。
4. `const &`、初始化、移动、断言、宽整数、随机化先保留；每项改动确认用途后再动。

现阶段算法源码变化为 0 行，依赖变化为 0；没有把所有候选都算作可删除行数，因为有些改写会增加代码或者改变接口。净删减量须在选定具体规则后通过真实 diff 统计。本轮没有重新运行算法测试，因为没有修改算法，也不声称完成了新一轮正确性验证。

## 全位置附录

下面列出各扫描类别在当前源码中的所有命中行，注释与调用示例不计入。链接指向绝对路径；表格中每个行号均可点击。重复位置表示同一行用了多种语法，不是多份代码。

### 函数模板声明

20 个命中行，14 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/DSUOnTree.cpp` | [35](D:/OI/template/template/graph/DSUOnTree.cpp:35)、[57](D:/OI/template/template/graph/DSUOnTree.cpp:57) |
| `template/graph/HLD.cpp` | [60](D:/OI/template/template/graph/HLD.cpp:60) |
| `template/math/AdaptiveSimpson.cpp` | [13](D:/OI/template/template/math/AdaptiveSimpson.cpp:13) |
| `template/math/Gauss.cpp` | [79](D:/OI/template/template/math/Gauss.cpp:79) |
| `template/math/Min25.cpp` | [71](D:/OI/template/template/math/Min25.cpp:71) |
| `template/others/Common.cpp` | [15](D:/OI/template/template/others/Common.cpp:15)、[16](D:/OI/template/template/others/Common.cpp:16)、[17](D:/OI/template/template/others/Common.cpp:17)、[18](D:/OI/template/template/others/Common.cpp:18) |
| `template/others/Discretize.cpp` | [9](D:/OI/template/template/others/Discretize.cpp:9) |
| `template/others/DPOpt.cpp` | [13](D:/OI/template/template/others/DPOpt.cpp:13) |
| `template/others/LIS.cpp` | [10](D:/OI/template/template/others/LIS.cpp:10) |
| `template/others/Mo.cpp` | [13](D:/OI/template/template/others/Mo.cpp:13) |
| `template/others/MoModify.cpp` | [31](D:/OI/template/template/others/MoModify.cpp:31) |
| `template/others/MoTree.cpp` | [53](D:/OI/template/template/others/MoTree.cpp:53) |
| `template/others/SlidingWindow.cpp` | [10](D:/OI/template/template/others/SlidingWindow.cpp:10)、[23](D:/OI/template/template/others/SlidingWindow.cpp:23) |
| `template/others/TernarySearch.cpp` | [11](D:/OI/template/template/others/TernarySearch.cpp:11)、[20](D:/OI/template/template/others/TernarySearch.cpp:20) |

### 自传参递归 lambda

13 个命中行，12 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/DirectedEulerTrail.cpp` | [31](D:/OI/template/template/graph/DirectedEulerTrail.cpp:31) |
| `template/graph/Kruskal.cpp` | [32](D:/OI/template/template/graph/Kruskal.cpp:32) |
| `template/graph/Reroot.cpp` | [20](D:/OI/template/template/graph/Reroot.cpp:20)、[27](D:/OI/template/template/graph/Reroot.cpp:27) |
| `template/graph/TreeDiameter.cpp` | [17](D:/OI/template/template/graph/TreeDiameter.cpp:17) |
| `template/graph/UndirectedEulerTrail.cpp` | [32](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:32) |
| `template/math/AdaptiveSimpson.cpp` | [21](D:/OI/template/template/math/AdaptiveSimpson.cpp:21) |
| `template/math/BSGS.cpp` | [12](D:/OI/template/template/math/BSGS.cpp:12) |
| `template/math/Min25.cpp` | [84](D:/OI/template/template/math/Min25.cpp:84) |
| `template/math/PollardRho.cpp` | [73](D:/OI/template/template/math/PollardRho.cpp:73) |
| `template/others/CDQ.cpp` | [41](D:/OI/template/template/others/CDQ.cpp:41) |
| `template/others/DPOpt.cpp` | [17](D:/OI/template/template/others/DPOpt.cpp:17) |
| `template/others/SG.cpp` | [16](D:/OI/template/template/others/SG.cpp:16) |

### 全部 lambda 声明起始行

52 个命中行，32 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/KDTree.cpp` | [30](D:/OI/template/template/ds/KDTree.cpp:30) |
| `template/geometry/CircleUnion.cpp` | [54](D:/OI/template/template/geometry/CircleUnion.cpp:54) |
| `template/geometry/ClosestPair.cpp` | [25](D:/OI/template/template/geometry/ClosestPair.cpp:25)、[34](D:/OI/template/template/geometry/ClosestPair.cpp:34)、[49](D:/OI/template/template/geometry/ClosestPair.cpp:49) |
| `template/geometry/ConvexHull.cpp` | [17](D:/OI/template/template/geometry/ConvexHull.cpp:17)、[38](D:/OI/template/template/geometry/ConvexHull.cpp:38)、[98](D:/OI/template/template/geometry/ConvexHull.cpp:98) |
| `template/geometry/HalfPlane.cpp` | [13](D:/OI/template/template/geometry/HalfPlane.cpp:13)、[28](D:/OI/template/template/geometry/HalfPlane.cpp:28) |
| `template/geometry/MinRectangle.cpp` | [33](D:/OI/template/template/geometry/MinRectangle.cpp:33)、[34](D:/OI/template/template/geometry/MinRectangle.cpp:34) |
| `template/geometry/RectangleUnion.cpp` | [50](D:/OI/template/template/geometry/RectangleUnion.cpp:50) |
| `template/graph/DirectedEulerTrail.cpp` | [31](D:/OI/template/template/graph/DirectedEulerTrail.cpp:31) |
| `template/graph/Kruskal.cpp` | [25](D:/OI/template/template/graph/Kruskal.cpp:25)、[32](D:/OI/template/template/graph/Kruskal.cpp:32) |
| `template/graph/KruskalTree.cpp` | [39](D:/OI/template/template/graph/KruskalTree.cpp:39) |
| `template/graph/Reroot.cpp` | [20](D:/OI/template/template/graph/Reroot.cpp:20)、[27](D:/OI/template/template/graph/Reroot.cpp:27) |
| `template/graph/ScalingMinCostFlow.cpp` | [50](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:50)、[73](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:73)、[78](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:78)、[104](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:104) |
| `template/graph/TreeDiameter.cpp` | [17](D:/OI/template/template/graph/TreeDiameter.cpp:17) |
| `template/graph/UndirectedEulerTrail.cpp` | [32](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:32) |
| `template/graph/VirtualTree.cpp` | [60](D:/OI/template/template/graph/VirtualTree.cpp:60) |
| `template/graph/WeightedMatching.cpp` | [119](D:/OI/template/template/graph/WeightedMatching.cpp:119) |
| `template/math/AdaptiveSimpson.cpp` | [16](D:/OI/template/template/math/AdaptiveSimpson.cpp:16)、[21](D:/OI/template/template/math/AdaptiveSimpson.cpp:21) |
| `template/math/BSGS.cpp` | [12](D:/OI/template/template/math/BSGS.cpp:12) |
| `template/math/Combination.cpp` | [22](D:/OI/template/template/math/Combination.cpp:22) |
| `template/math/Determinant.cpp` | [25](D:/OI/template/template/math/Determinant.cpp:25)、[53](D:/OI/template/template/math/Determinant.cpp:53) |
| `template/math/Gauss.cpp` | [52](D:/OI/template/template/math/Gauss.cpp:52) |
| `template/math/MatrixInverse.cpp` | [20](D:/OI/template/template/math/MatrixInverse.cpp:20) |
| `template/math/Min25.cpp` | [84](D:/OI/template/template/math/Min25.cpp:84) |
| `template/math/ModSqrt.cpp` | [16](D:/OI/template/template/math/ModSqrt.cpp:16)、[32](D:/OI/template/template/math/ModSqrt.cpp:32) |
| `template/math/PollardRho.cpp` | [49](D:/OI/template/template/math/PollardRho.cpp:49)、[73](D:/OI/template/template/math/PollardRho.cpp:73) |
| `template/math/SubsetConvolution.cpp` | [24](D:/OI/template/template/math/SubsetConvolution.cpp:24) |
| `template/others/CDQ.cpp` | [18](D:/OI/template/template/others/CDQ.cpp:18)、[32](D:/OI/template/template/others/CDQ.cpp:32)、[35](D:/OI/template/template/others/CDQ.cpp:35)、[41](D:/OI/template/template/others/CDQ.cpp:41) |
| `template/others/DPOpt.cpp` | [17](D:/OI/template/template/others/DPOpt.cpp:17) |
| `template/others/Mo.cpp` | [16](D:/OI/template/template/others/Mo.cpp:16) |
| `template/others/MoModify.cpp` | [35](D:/OI/template/template/others/MoModify.cpp:35)、[44](D:/OI/template/template/others/MoModify.cpp:44) |
| `template/others/MoTree.cpp` | [57](D:/OI/template/template/others/MoTree.cpp:57)、[62](D:/OI/template/template/others/MoTree.cpp:62) |
| `template/others/SG.cpp` | [16](D:/OI/template/template/others/SG.cpp:16) |

### 静态函数声明

20 个命中行，6 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/math/BerlekampMassey.cpp` | [13](D:/OI/template/template/math/BerlekampMassey.cpp:13)、[18](D:/OI/template/template/math/BerlekampMassey.cpp:18)、[62](D:/OI/template/template/math/BerlekampMassey.cpp:62) |
| `template/math/FFT.cpp` | [62](D:/OI/template/template/math/FFT.cpp:62)、[69](D:/OI/template/template/math/FFT.cpp:69)、[97](D:/OI/template/template/math/FFT.cpp:97) |
| `template/math/FPS.cpp` | [23](D:/OI/template/template/math/FPS.cpp:23)、[30](D:/OI/template/template/math/FPS.cpp:30)、[58](D:/OI/template/template/math/FPS.cpp:58)、[71](D:/OI/template/template/math/FPS.cpp:71)、[86](D:/OI/template/template/math/FPS.cpp:86)、[100](D:/OI/template/template/math/FPS.cpp:100)、[117](D:/OI/template/template/math/FPS.cpp:117)、[141](D:/OI/template/template/math/FPS.cpp:141)、[159](D:/OI/template/template/math/FPS.cpp:159)、[184](D:/OI/template/template/math/FPS.cpp:184) |
| `template/math/FWT.cpp` | [13](D:/OI/template/template/math/FWT.cpp:13)、[16](D:/OI/template/template/math/FWT.cpp:16) |
| `template/math/Lagrange.cpp` | [13](D:/OI/template/template/math/Lagrange.cpp:13) |
| `template/others/TreeHash.cpp` | [10](D:/OI/template/template/others/TreeHash.cpp:10) |

### constexpr 常量

12 个命中行，10 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/SegmentTreeBeats.cpp` | [16](D:/OI/template/template/ds/SegmentTreeBeats.cpp:16) |
| `template/graph/Johnson.cpp` | [17](D:/OI/template/template/graph/Johnson.cpp:17) |
| `template/graph/KShortestWalk.cpp` | [16](D:/OI/template/template/graph/KShortestWalk.cpp:16) |
| `template/graph/ScalingMinCostFlow.cpp` | [38](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:38)、[39](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:39) |
| `template/graph/SteinerTree.cpp` | [16](D:/OI/template/template/graph/SteinerTree.cpp:16) |
| `template/math/FPS.cpp` | [21](D:/OI/template/template/math/FPS.cpp:21) |
| `template/math/Multipoint.cpp` | [13](D:/OI/template/template/math/Multipoint.cpp:13) |
| `template/math/NTT.cpp` | [12](D:/OI/template/template/math/NTT.cpp:12)、[13](D:/OI/template/template/math/NTT.cpp:13) |
| `template/string/Hash.cpp` | [14](D:/OI/template/template/string/Hash.cpp:14) |
| `template/string/Hash2D.cpp` | [16](D:/OI/template/template/string/Hash2D.cpp:16) |

### 全部 const 命中行

237 个命中行，105 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/AffineSegTree.cpp` | [17](D:/OI/template/template/ds/AffineSegTree.cpp:17)、[33](D:/OI/template/template/ds/AffineSegTree.cpp:33)、[40](D:/OI/template/template/ds/AffineSegTree.cpp:40) |
| `template/ds/Fenwick.cpp` | [17](D:/OI/template/template/ds/Fenwick.cpp:17)、[24](D:/OI/template/template/ds/Fenwick.cpp:24)、[37](D:/OI/template/template/ds/Fenwick.cpp:37)、[43](D:/OI/template/template/ds/Fenwick.cpp:43)、[49](D:/OI/template/template/ds/Fenwick.cpp:49) |
| `template/ds/Fenwick2D.cpp` | [16](D:/OI/template/template/ds/Fenwick2D.cpp:16)、[42](D:/OI/template/template/ds/Fenwick2D.cpp:42)、[54](D:/OI/template/template/ds/Fenwick2D.cpp:54) |
| `template/ds/FHQTreap.cpp` | [31](D:/OI/template/template/ds/FHQTreap.cpp:31)、[79](D:/OI/template/template/ds/FHQTreap.cpp:79)、[88](D:/OI/template/template/ds/FHQTreap.cpp:88)、[100](D:/OI/template/template/ds/FHQTreap.cpp:100)、[109](D:/OI/template/template/ds/FHQTreap.cpp:109)、[132](D:/OI/template/template/ds/FHQTreap.cpp:132) |
| `template/ds/IntervalSet.cpp` | [41](D:/OI/template/template/ds/IntervalSet.cpp:41)、[46](D:/OI/template/template/ds/IntervalSet.cpp:46) |
| `template/ds/KDTree.cpp` | [30](D:/OI/template/template/ds/KDTree.cpp:30)、[46](D:/OI/template/template/ds/KDTree.cpp:46)、[54](D:/OI/template/template/ds/KDTree.cpp:54)、[61](D:/OI/template/template/ds/KDTree.cpp:61)、[63](D:/OI/template/template/ds/KDTree.cpp:63)、[74](D:/OI/template/template/ds/KDTree.cpp:74)、[80](D:/OI/template/template/ds/KDTree.cpp:80)、[82](D:/OI/template/template/ds/KDTree.cpp:82)、[90](D:/OI/template/template/ds/KDTree.cpp:90) |
| `template/ds/LCT.cpp` | [17](D:/OI/template/template/ds/LCT.cpp:17)、[21](D:/OI/template/template/ds/LCT.cpp:21) |
| `template/ds/LeftistHeap.cpp` | [29](D:/OI/template/template/ds/LeftistHeap.cpp:29) |
| `template/ds/LiChaoTree.cpp` | [24](D:/OI/template/template/ds/LiChaoTree.cpp:24)、[92](D:/OI/template/template/ds/LiChaoTree.cpp:92)、[109](D:/OI/template/template/ds/LiChaoTree.cpp:109)、[120](D:/OI/template/template/ds/LiChaoTree.cpp:120)、[124](D:/OI/template/template/ds/LiChaoTree.cpp:124) |
| `template/ds/MergeSegTree.cpp` | [35](D:/OI/template/template/ds/MergeSegTree.cpp:35)、[42](D:/OI/template/template/ds/MergeSegTree.cpp:42)、[44](D:/OI/template/template/ds/MergeSegTree.cpp:44)、[52](D:/OI/template/template/ds/MergeSegTree.cpp:52) |
| `template/ds/PersistentArray.cpp` | [22](D:/OI/template/template/ds/PersistentArray.cpp:22)、[32](D:/OI/template/template/ds/PersistentArray.cpp:32)、[54](D:/OI/template/template/ds/PersistentArray.cpp:54)、[65](D:/OI/template/template/ds/PersistentArray.cpp:65) |
| `template/ds/PersistentSegTree.cpp` | [29](D:/OI/template/template/ds/PersistentSegTree.cpp:29)、[41](D:/OI/template/template/ds/PersistentSegTree.cpp:41)、[48](D:/OI/template/template/ds/PersistentSegTree.cpp:48)、[52](D:/OI/template/template/ds/PersistentSegTree.cpp:52)、[60](D:/OI/template/template/ds/PersistentSegTree.cpp:60) |
| `template/ds/PersistentXorTrie.cpp` | [35](D:/OI/template/template/ds/PersistentXorTrie.cpp:35) |
| `template/ds/RollbackDSU.cpp` | [23](D:/OI/template/template/ds/RollbackDSU.cpp:23)、[28](D:/OI/template/template/ds/RollbackDSU.cpp:28) |
| `template/ds/SegmentTreeBeats.cpp` | [81](D:/OI/template/template/ds/SegmentTreeBeats.cpp:81)、[93](D:/OI/template/template/ds/SegmentTreeBeats.cpp:93) |
| `template/ds/SegTree.cpp` | [38](D:/OI/template/template/ds/SegTree.cpp:38)、[46](D:/OI/template/template/ds/SegTree.cpp:46) |
| `template/ds/XorTrie.cpp` | [45](D:/OI/template/template/ds/XorTrie.cpp:45) |
| `template/geometry/Circle.cpp` | [14](D:/OI/template/template/geometry/Circle.cpp:14)、[93](D:/OI/template/template/geometry/Circle.cpp:93) |
| `template/geometry/CircleUnion.cpp` | [16](D:/OI/template/template/geometry/CircleUnion.cpp:16)、[17](D:/OI/template/template/geometry/CircleUnion.cpp:17) |
| `template/geometry/ClosestPair.cpp` | [46](D:/OI/template/template/geometry/ClosestPair.cpp:46) |
| `template/geometry/ConvexHull3D.cpp` | [19](D:/OI/template/template/geometry/ConvexHull3D.cpp:19)、[20](D:/OI/template/template/geometry/ConvexHull3D.cpp:20)、[21](D:/OI/template/template/geometry/ConvexHull3D.cpp:21)、[22](D:/OI/template/template/geometry/ConvexHull3D.cpp:22)、[23](D:/OI/template/template/geometry/ConvexHull3D.cpp:23)、[24](D:/OI/template/template/geometry/ConvexHull3D.cpp:24)、[28](D:/OI/template/template/geometry/ConvexHull3D.cpp:28)、[39](D:/OI/template/template/geometry/ConvexHull3D.cpp:39)、[49](D:/OI/template/template/geometry/ConvexHull3D.cpp:49)、[94](D:/OI/template/template/geometry/ConvexHull3D.cpp:94)、[100](D:/OI/template/template/geometry/ConvexHull3D.cpp:100) |
| `template/geometry/HalfPlane.cpp` | [13](D:/OI/template/template/geometry/HalfPlane.cpp:13) |
| `template/geometry/Point.cpp` | [10](D:/OI/template/template/geometry/Point.cpp:10)、[28](D:/OI/template/template/geometry/Point.cpp:28)、[33](D:/OI/template/template/geometry/Point.cpp:33)、[44](D:/OI/template/template/geometry/Point.cpp:44)、[45](D:/OI/template/template/geometry/Point.cpp:45)、[54](D:/OI/template/template/geometry/Point.cpp:54) |
| `template/geometry/RectangleUnion.cpp` | [39](D:/OI/template/template/geometry/RectangleUnion.cpp:39) |
| `template/graph/BlockCutTree.cpp` | [12](D:/OI/template/template/graph/BlockCutTree.cpp:12) |
| `template/graph/BridgeTree.cpp` | [11](D:/OI/template/template/graph/BridgeTree.cpp:11)、[12](D:/OI/template/template/graph/BridgeTree.cpp:12) |
| `template/graph/Centroid.cpp` | [51](D:/OI/template/template/graph/Centroid.cpp:51) |
| `template/graph/CentroidTree.cpp` | [30](D:/OI/template/template/graph/CentroidTree.cpp:30)、[106](D:/OI/template/template/graph/CentroidTree.cpp:106) |
| `template/graph/DiffConstraints.cpp` | [23](D:/OI/template/template/graph/DiffConstraints.cpp:23) |
| `template/graph/Dijkstra.cpp` | [14](D:/OI/template/template/graph/Dijkstra.cpp:14) |
| `template/graph/DirectedEulerTrail.cpp` | [12](D:/OI/template/template/graph/DirectedEulerTrail.cpp:12) |
| `template/graph/DirectedMST.cpp` | [29](D:/OI/template/template/graph/DirectedMST.cpp:29) |
| `template/graph/DynamicConnectivity.cpp` | [29](D:/OI/template/template/graph/DynamicConnectivity.cpp:29) |
| `template/graph/EulerLCA.cpp` | [61](D:/OI/template/template/graph/EulerLCA.cpp:61) |
| `template/graph/FunctionalGraph.cpp` | [28](D:/OI/template/template/graph/FunctionalGraph.cpp:28)、[51](D:/OI/template/template/graph/FunctionalGraph.cpp:51)、[58](D:/OI/template/template/graph/FunctionalGraph.cpp:58)、[63](D:/OI/template/template/graph/FunctionalGraph.cpp:63) |
| `template/graph/GlobalMinCut.cpp` | [31](D:/OI/template/template/graph/GlobalMinCut.cpp:31) |
| `template/graph/GomoryHu.cpp` | [68](D:/OI/template/template/graph/GomoryHu.cpp:68) |
| `template/graph/HLD.cpp` | [52](D:/OI/template/template/graph/HLD.cpp:52)、[60](D:/OI/template/template/graph/HLD.cpp:60) |
| `template/graph/HopcroftKarp.cpp` | [73](D:/OI/template/template/graph/HopcroftKarp.cpp:73) |
| `template/graph/Johnson.cpp` | [36](D:/OI/template/template/graph/Johnson.cpp:36) |
| `template/graph/KM.cpp` | [37](D:/OI/template/template/graph/KM.cpp:37) |
| `template/graph/Kruskal.cpp` | [25](D:/OI/template/template/graph/Kruskal.cpp:25)、[37](D:/OI/template/template/graph/Kruskal.cpp:37) |
| `template/graph/KruskalTree.cpp` | [56](D:/OI/template/template/graph/KruskalTree.cpp:56)、[67](D:/OI/template/template/graph/KruskalTree.cpp:67) |
| `template/graph/KthAncestor.cpp` | [55](D:/OI/template/template/graph/KthAncestor.cpp:55) |
| `template/graph/LCA.cpp` | [46](D:/OI/template/template/graph/LCA.cpp:46) |
| `template/graph/MaxClique.cpp` | [29](D:/OI/template/template/graph/MaxClique.cpp:29)、[31](D:/OI/template/template/graph/MaxClique.cpp:31) |
| `template/graph/Reroot.cpp` | [16](D:/OI/template/template/graph/Reroot.cpp:16) |
| `template/graph/ScalingMinCostFlow.cpp` | [34](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:34)、[40](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:40)、[59](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:59)、[62](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:62)、[68](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:68) |
| `template/graph/SteinerTree.cpp` | [27](D:/OI/template/template/graph/SteinerTree.cpp:27) |
| `template/graph/TopologicalSort.cpp` | [11](D:/OI/template/template/graph/TopologicalSort.cpp:11) |
| `template/graph/TreeDiameter.cpp` | [15](D:/OI/template/template/graph/TreeDiameter.cpp:15)、[29](D:/OI/template/template/graph/TreeDiameter.cpp:29) |
| `template/graph/UndirectedEulerTrail.cpp` | [12](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:12) |
| `template/graph/VirtualTree.cpp` | [46](D:/OI/template/template/graph/VirtualTree.cpp:46)、[48](D:/OI/template/template/graph/VirtualTree.cpp:48) |
| `template/graph/WeightedMatching.cpp` | [49](D:/OI/template/template/graph/WeightedMatching.cpp:49) |
| `template/graph/ZeroOneBFS.cpp` | [9](D:/OI/template/template/graph/ZeroOneBFS.cpp:9) |
| `template/math/BerlekampMassey.cpp` | [27](D:/OI/template/template/math/BerlekampMassey.cpp:27)、[62](D:/OI/template/template/math/BerlekampMassey.cpp:62)、[63](D:/OI/template/template/math/BerlekampMassey.cpp:63)、[77](D:/OI/template/template/math/BerlekampMassey.cpp:77) |
| `template/math/BostanMori.cpp` | [29](D:/OI/template/template/math/BostanMori.cpp:29) |
| `template/math/Combination.cpp` | [33](D:/OI/template/template/math/Combination.cpp:33) |
| `template/math/CRT.cpp` | [33](D:/OI/template/template/math/CRT.cpp:33) |
| `template/math/Determinant.cpp` | [48](D:/OI/template/template/math/Determinant.cpp:48) |
| `template/math/EquidistantLagrange.cpp` | [17](D:/OI/template/template/math/EquidistantLagrange.cpp:17)、[22](D:/OI/template/template/math/EquidistantLagrange.cpp:22) |
| `template/math/ExLucas.cpp` | [60](D:/OI/template/template/math/ExLucas.cpp:60)、[70](D:/OI/template/template/math/ExLucas.cpp:70) |
| `template/math/FFT.cpp` | [30](D:/OI/template/template/math/FFT.cpp:30)、[46](D:/OI/template/template/math/FFT.cpp:46)、[97](D:/OI/template/template/math/FFT.cpp:97)、[111](D:/OI/template/template/math/FFT.cpp:111)、[118](D:/OI/template/template/math/FFT.cpp:118) |
| `template/math/FPS.cpp` | [71](D:/OI/template/template/math/FPS.cpp:71)、[86](D:/OI/template/template/math/FPS.cpp:86)、[100](D:/OI/template/template/math/FPS.cpp:100)、[117](D:/OI/template/template/math/FPS.cpp:117)、[159](D:/OI/template/template/math/FPS.cpp:159) |
| `template/math/FWT.cpp` | [13](D:/OI/template/template/math/FWT.cpp:13) |
| `template/math/Lagrange.cpp` | [22](D:/OI/template/template/math/Lagrange.cpp:22)、[23](D:/OI/template/template/math/Lagrange.cpp:23) |
| `template/math/Lucas.cpp` | [29](D:/OI/template/template/math/Lucas.cpp:29) |
| `template/math/Matrix.cpp` | [16](D:/OI/template/template/math/Matrix.cpp:16) |
| `template/math/Min25.cpp` | [24](D:/OI/template/template/math/Min25.cpp:24)、[25](D:/OI/template/template/math/Min25.cpp:25)、[69](D:/OI/template/template/math/Min25.cpp:69)、[72](D:/OI/template/template/math/Min25.cpp:72) |
| `template/math/ModArithmetic.cpp` | [32](D:/OI/template/template/math/ModArithmetic.cpp:32) |
| `template/math/Multipoint.cpp` | [25](D:/OI/template/template/math/Multipoint.cpp:25)、[31](D:/OI/template/template/math/Multipoint.cpp:31)、[35](D:/OI/template/template/math/Multipoint.cpp:35)、[36](D:/OI/template/template/math/Multipoint.cpp:36)、[48](D:/OI/template/template/math/Multipoint.cpp:48)、[55](D:/OI/template/template/math/Multipoint.cpp:55)、[68](D:/OI/template/template/math/Multipoint.cpp:68) |
| `template/math/Simplex.cpp` | [56](D:/OI/template/template/math/Simplex.cpp:56)、[57](D:/OI/template/template/math/Simplex.cpp:57) |
| `template/math/SubsetConvolution.cpp` | [12](D:/OI/template/template/math/SubsetConvolution.cpp:12) |
| `template/math/XorBasis.cpp` | [21](D:/OI/template/template/math/XorBasis.cpp:21)、[28](D:/OI/template/template/math/XorBasis.cpp:28)、[35](D:/OI/template/template/math/XorBasis.cpp:35) |
| `template/others/CartesianTree.cpp` | [16](D:/OI/template/template/others/CartesianTree.cpp:16) |
| `template/others/CDQ.cpp` | [13](D:/OI/template/template/others/CDQ.cpp:13)、[60](D:/OI/template/template/others/CDQ.cpp:60) |
| `template/others/Common.cpp` | [15](D:/OI/template/template/others/Common.cpp:15)、[16](D:/OI/template/template/others/Common.cpp:16) |
| `template/others/ConvexHullTrick.cpp` | [19](D:/OI/template/template/others/ConvexHullTrick.cpp:19)、[26](D:/OI/template/template/others/ConvexHullTrick.cpp:26) |
| `template/others/Discretize.cpp` | [10](D:/OI/template/template/others/Discretize.cpp:10)、[17](D:/OI/template/template/others/Discretize.cpp:17) |
| `template/others/DLX.cpp` | [31](D:/OI/template/template/others/DLX.cpp:31) |
| `template/others/DPOpt.cpp` | [14](D:/OI/template/template/others/DPOpt.cpp:14) |
| `template/others/LIS.cpp` | [11](D:/OI/template/template/others/LIS.cpp:11) |
| `template/others/Mo.cpp` | [16](D:/OI/template/template/others/Mo.cpp:16)、[22](D:/OI/template/template/others/Mo.cpp:22) |
| `template/others/MoModify.cpp` | [20](D:/OI/template/template/others/MoModify.cpp:20)、[32](D:/OI/template/template/others/MoModify.cpp:32) |
| `template/others/MonotonicStack.cpp` | [12](D:/OI/template/template/others/MonotonicStack.cpp:12) |
| `template/others/MoTree.cpp` | [37](D:/OI/template/template/others/MoTree.cpp:37)、[54](D:/OI/template/template/others/MoTree.cpp:54) |
| `template/others/OverallBinary.cpp` | [27](D:/OI/template/template/others/OverallBinary.cpp:27)、[51](D:/OI/template/template/others/OverallBinary.cpp:51)、[78](D:/OI/template/template/others/OverallBinary.cpp:78) |
| `template/others/SG.cpp` | [12](D:/OI/template/template/others/SG.cpp:12) |
| `template/others/SlidingWindow.cpp` | [11](D:/OI/template/template/others/SlidingWindow.cpp:11)、[24](D:/OI/template/template/others/SlidingWindow.cpp:24) |
| `template/others/SparseTable.cpp` | [14](D:/OI/template/template/others/SparseTable.cpp:14)、[24](D:/OI/template/template/others/SparseTable.cpp:24) |
| `template/others/TreeHash.cpp` | [17](D:/OI/template/template/others/TreeHash.cpp:17) |
| `template/string/ACAutomaton.cpp` | [29](D:/OI/template/template/string/ACAutomaton.cpp:29)、[66](D:/OI/template/template/string/ACAutomaton.cpp:66) |
| `template/string/BitsetLCS.cpp` | [10](D:/OI/template/template/string/BitsetLCS.cpp:10) |
| `template/string/Hash.cpp` | [18](D:/OI/template/template/string/Hash.cpp:18)、[20](D:/OI/template/template/string/Hash.cpp:20)、[33](D:/OI/template/template/string/Hash.cpp:33)、[40](D:/OI/template/template/string/Hash.cpp:40) |
| `template/string/Hash2D.cpp` | [21](D:/OI/template/template/string/Hash2D.cpp:21)、[36](D:/OI/template/template/string/Hash2D.cpp:36)、[47](D:/OI/template/template/string/Hash2D.cpp:47)、[51](D:/OI/template/template/string/Hash2D.cpp:51) |
| `template/string/KMP.cpp` | [10](D:/OI/template/template/string/KMP.cpp:10)、[21](D:/OI/template/template/string/KMP.cpp:21) |
| `template/string/Lyndon.cpp` | [11](D:/OI/template/template/string/Lyndon.cpp:11) |
| `template/string/Manacher.cpp` | [15](D:/OI/template/template/string/Manacher.cpp:15)、[17](D:/OI/template/template/string/Manacher.cpp:17)、[47](D:/OI/template/template/string/Manacher.cpp:47)、[54](D:/OI/template/template/string/Manacher.cpp:54) |
| `template/string/MinRotation.cpp` | [8](D:/OI/template/template/string/MinRotation.cpp:8) |
| `template/string/PAM.cpp` | [25](D:/OI/template/template/string/PAM.cpp:25)、[33](D:/OI/template/template/string/PAM.cpp:33)、[54](D:/OI/template/template/string/PAM.cpp:54)、[59](D:/OI/template/template/string/PAM.cpp:59)、[61](D:/OI/template/template/string/PAM.cpp:61) |
| `template/string/Runs.cpp` | [13](D:/OI/template/template/string/Runs.cpp:13) |
| `template/string/SAM.cpp` | [59](D:/OI/template/template/string/SAM.cpp:59)、[64](D:/OI/template/template/string/SAM.cpp:64)、[79](D:/OI/template/template/string/SAM.cpp:79) |
| `template/string/SuffixArray.cpp` | [16](D:/OI/template/template/string/SuffixArray.cpp:16)、[18](D:/OI/template/template/string/SuffixArray.cpp:18)、[81](D:/OI/template/template/string/SuffixArray.cpp:81) |
| `template/string/Trie.cpp` | [21](D:/OI/template/template/string/Trie.cpp:21)、[36](D:/OI/template/template/string/Trie.cpp:36)、[46](D:/OI/template/template/string/Trie.cpp:46)、[51](D:/OI/template/template/string/Trie.cpp:51) |
| `template/string/Z.cpp` | [11](D:/OI/template/template/string/Z.cpp:11)、[23](D:/OI/template/template/string/Z.cpp:23) |

### move 调用

3 个命中行，3 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/VBCC.cpp` | [60](D:/OI/template/template/graph/VBCC.cpp:60) |
| `template/math/BerlekampMassey.cpp` | [49](D:/OI/template/template/math/BerlekampMassey.cpp:49) |
| `template/math/ExLucas.cpp` | [39](D:/OI/template/template/math/ExLucas.cpp:39) |

### 显式默认构造函数

2 个命中行，2 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/string/Manacher.cpp` | [14](D:/OI/template/template/string/Manacher.cpp:14) |
| `template/string/SuffixArray.cpp` | [15](D:/OI/template/template/string/SuffixArray.cpp:15) |

### 构造函数初始化列表

4 个命中行，4 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/geometry/Line.cpp` | [117](D:/OI/template/template/geometry/Line.cpp:117) |
| `template/geometry/Point.cpp` | [21](D:/OI/template/template/geometry/Point.cpp:21) |
| `template/graph/DiffConstraints.cpp` | [18](D:/OI/template/template/graph/DiffConstraints.cpp:18) |
| `template/graph/LowerBoundFlow.cpp` | [31](D:/OI/template/template/graph/LowerBoundFlow.cpp:31) |

### 构造函数中调用 init 或 build

55 个命中行，50 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/DSU.cpp` | [15](D:/OI/template/template/ds/DSU.cpp:15) |
| `template/ds/Fenwick.cpp` | [16](D:/OI/template/template/ds/Fenwick.cpp:16)、[17](D:/OI/template/template/ds/Fenwick.cpp:17) |
| `template/ds/FHQTreap.cpp` | [29](D:/OI/template/template/ds/FHQTreap.cpp:29)、[130](D:/OI/template/template/ds/FHQTreap.cpp:130) |
| `template/ds/LeftistHeap.cpp` | [21](D:/OI/template/template/ds/LeftistHeap.cpp:21) |
| `template/ds/LiChaoTree.cpp` | [33](D:/OI/template/template/ds/LiChaoTree.cpp:33)、[34](D:/OI/template/template/ds/LiChaoTree.cpp:34) |
| `template/ds/MergeSegTree.cpp` | [18](D:/OI/template/template/ds/MergeSegTree.cpp:18) |
| `template/ds/PersistentXorTrie.cpp` | [18](D:/OI/template/template/ds/PersistentXorTrie.cpp:18) |
| `template/ds/RollbackDSU.cpp` | [14](D:/OI/template/template/ds/RollbackDSU.cpp:14) |
| `template/ds/WeightedDSU.cpp` | [19](D:/OI/template/template/ds/WeightedDSU.cpp:19)、[55](D:/OI/template/template/ds/WeightedDSU.cpp:55) |
| `template/ds/XorTrie.cpp` | [16](D:/OI/template/template/ds/XorTrie.cpp:16) |
| `template/graph/BipartiteMatch.cpp` | [15](D:/OI/template/template/graph/BipartiteMatch.cpp:15) |
| `template/graph/Bridge.cpp` | [16](D:/OI/template/template/graph/Bridge.cpp:16) |
| `template/graph/Centroid.cpp` | [20](D:/OI/template/template/graph/Centroid.cpp:20) |
| `template/graph/CentroidTree.cpp` | [48](D:/OI/template/template/graph/CentroidTree.cpp:48) |
| `template/graph/CutVertex.cpp` | [16](D:/OI/template/template/graph/CutVertex.cpp:16) |
| `template/graph/Dinic.cpp` | [22](D:/OI/template/template/graph/Dinic.cpp:22) |
| `template/graph/DirectedMST.cpp` | [23](D:/OI/template/template/graph/DirectedMST.cpp:23) |
| `template/graph/Dominator.cpp` | [16](D:/OI/template/template/graph/Dominator.cpp:16) |
| `template/graph/DSUOnTree.cpp` | [17](D:/OI/template/template/graph/DSUOnTree.cpp:17) |
| `template/graph/DynamicConnectivity.cpp` | [19](D:/OI/template/template/graph/DynamicConnectivity.cpp:19) |
| `template/graph/EBCC.cpp` | [27](D:/OI/template/template/graph/EBCC.cpp:27) |
| `template/graph/EulerLCA.cpp` | [16](D:/OI/template/template/graph/EulerLCA.cpp:16) |
| `template/graph/GeneralMatching.cpp` | [16](D:/OI/template/template/graph/GeneralMatching.cpp:16) |
| `template/graph/GlobalMinCut.cpp` | [18](D:/OI/template/template/graph/GlobalMinCut.cpp:18) |
| `template/graph/GomoryHu.cpp` | [24](D:/OI/template/template/graph/GomoryHu.cpp:24) |
| `template/graph/HLD.cpp` | [17](D:/OI/template/template/graph/HLD.cpp:17) |
| `template/graph/HopcroftKarp.cpp` | [16](D:/OI/template/template/graph/HopcroftKarp.cpp:16) |
| `template/graph/Johnson.cpp` | [23](D:/OI/template/template/graph/Johnson.cpp:23) |
| `template/graph/KM.cpp` | [21](D:/OI/template/template/graph/KM.cpp:21) |
| `template/graph/KruskalTree.cpp` | [23](D:/OI/template/template/graph/KruskalTree.cpp:23) |
| `template/graph/KShortestWalk.cpp` | [27](D:/OI/template/template/graph/KShortestWalk.cpp:27) |
| `template/graph/KthAncestor.cpp` | [16](D:/OI/template/template/graph/KthAncestor.cpp:16) |
| `template/graph/LCA.cpp` | [15](D:/OI/template/template/graph/LCA.cpp:15) |
| `template/graph/LowerBoundFlow.cpp` | [87](D:/OI/template/template/graph/LowerBoundFlow.cpp:87) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [26](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:26) |
| `template/graph/MaxClique.cpp` | [17](D:/OI/template/template/graph/MaxClique.cpp:17) |
| `template/graph/MinCostFlow.cpp` | [24](D:/OI/template/template/graph/MinCostFlow.cpp:24) |
| `template/graph/ScalingMinCostFlow.cpp` | [23](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:23) |
| `template/graph/SCC.cpp` | [16](D:/OI/template/template/graph/SCC.cpp:16) |
| `template/graph/SteinerTree.cpp` | [20](D:/OI/template/template/graph/SteinerTree.cpp:20) |
| `template/graph/TwoSAT.cpp` | [17](D:/OI/template/template/graph/TwoSAT.cpp:17) |
| `template/graph/VBCC.cpp` | [23](D:/OI/template/template/graph/VBCC.cpp:23) |
| `template/graph/VirtualTree.cpp` | [18](D:/OI/template/template/graph/VirtualTree.cpp:18) |
| `template/graph/WeightedMatching.cpp` | [27](D:/OI/template/template/graph/WeightedMatching.cpp:27) |
| `template/others/DLX.cpp` | [20](D:/OI/template/template/others/DLX.cpp:20) |
| `template/others/MoTree.cpp` | [18](D:/OI/template/template/others/MoTree.cpp:18) |
| `template/string/Hash.cpp` | [18](D:/OI/template/template/string/Hash.cpp:18) |
| `template/string/Manacher.cpp` | [15](D:/OI/template/template/string/Manacher.cpp:15) |
| `template/string/PAM.cpp` | [24](D:/OI/template/template/string/PAM.cpp:24)、[25](D:/OI/template/template/string/PAM.cpp:25) |
| `template/string/SuffixArray.cpp` | [16](D:/OI/template/template/string/SuffixArray.cpp:16) |

### 内部结构体声明

50 个命中行，42 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/FHQTreap.cpp` | [20](D:/OI/template/template/ds/FHQTreap.cpp:20)、[120](D:/OI/template/template/ds/FHQTreap.cpp:120) |
| `template/ds/KDTree.cpp` | [19](D:/OI/template/template/ds/KDTree.cpp:19) |
| `template/ds/LCT.cpp` | [14](D:/OI/template/template/ds/LCT.cpp:14) |
| `template/ds/LeftistHeap.cpp` | [15](D:/OI/template/template/ds/LeftistHeap.cpp:15) |
| `template/ds/LiChaoTree.cpp` | [18](D:/OI/template/template/ds/LiChaoTree.cpp:18) |
| `template/ds/MergeSegTree.cpp` | [14](D:/OI/template/template/ds/MergeSegTree.cpp:14) |
| `template/ds/PersistentArray.cpp` | [15](D:/OI/template/template/ds/PersistentArray.cpp:15) |
| `template/ds/PersistentSegTree.cpp` | [14](D:/OI/template/template/ds/PersistentSegTree.cpp:14) |
| `template/ds/PersistentXorTrie.cpp` | [15](D:/OI/template/template/ds/PersistentXorTrie.cpp:15) |
| `template/ds/SegmentTreeBeats.cpp` | [17](D:/OI/template/template/ds/SegmentTreeBeats.cpp:17) |
| `template/ds/XorTrie.cpp` | [13](D:/OI/template/template/ds/XorTrie.cpp:13) |
| `template/geometry/ClosestPair.cpp` | [12](D:/OI/template/template/geometry/ClosestPair.cpp:12) |
| `template/geometry/ConvexHull3D.cpp` | [32](D:/OI/template/template/geometry/ConvexHull3D.cpp:32) |
| `template/geometry/RectangleUnion.cpp` | [13](D:/OI/template/template/geometry/RectangleUnion.cpp:13)、[14](D:/OI/template/template/geometry/RectangleUnion.cpp:14) |
| `template/graph/CentroidTree.cpp` | [16](D:/OI/template/template/graph/CentroidTree.cpp:16)、[38](D:/OI/template/template/graph/CentroidTree.cpp:38) |
| `template/graph/Dinic.cpp` | [13](D:/OI/template/template/graph/Dinic.cpp:13) |
| `template/graph/DirectedMST.cpp` | [15](D:/OI/template/template/graph/DirectedMST.cpp:15) |
| `template/graph/DynamicConnectivity.cpp` | [12](D:/OI/template/template/graph/DynamicConnectivity.cpp:12) |
| `template/graph/EBCC.cpp` | [14](D:/OI/template/template/graph/EBCC.cpp:14) |
| `template/graph/GomoryHu.cpp` | [15](D:/OI/template/template/graph/GomoryHu.cpp:15) |
| `template/graph/Johnson.cpp` | [18](D:/OI/template/template/graph/Johnson.cpp:18) |
| `template/graph/KruskalTree.cpp` | [16](D:/OI/template/template/graph/KruskalTree.cpp:16) |
| `template/graph/KShortestWalk.cpp` | [17](D:/OI/template/template/graph/KShortestWalk.cpp:17)、[18](D:/OI/template/template/graph/KShortestWalk.cpp:18) |
| `template/graph/LowerBoundFlow.cpp` | [17](D:/OI/template/template/graph/LowerBoundFlow.cpp:17)、[22](D:/OI/template/template/graph/LowerBoundFlow.cpp:22)、[23](D:/OI/template/template/graph/LowerBoundFlow.cpp:23) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [21](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:21) |
| `template/graph/MinCostFlow.cpp` | [17](D:/OI/template/template/graph/MinCostFlow.cpp:17) |
| `template/graph/ScalingMinCostFlow.cpp` | [15](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:15)、[42](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:42) |
| `template/graph/VBCC.cpp` | [12](D:/OI/template/template/graph/VBCC.cpp:12) |
| `template/graph/WeightedMatching.cpp` | [18](D:/OI/template/template/graph/WeightedMatching.cpp:18) |
| `template/math/ExLucas.cpp` | [17](D:/OI/template/template/math/ExLucas.cpp:17) |
| `template/math/Simplex.cpp` | [17](D:/OI/template/template/math/Simplex.cpp:17) |
| `template/others/CDQ.cpp` | [14](D:/OI/template/template/others/CDQ.cpp:14) |
| `template/others/ConvexHullTrick.cpp` | [16](D:/OI/template/template/others/ConvexHullTrick.cpp:16) |
| `template/others/DLX.cpp` | [15](D:/OI/template/template/others/DLX.cpp:15) |
| `template/others/MoModify.cpp` | [13](D:/OI/template/template/others/MoModify.cpp:13)、[14](D:/OI/template/template/others/MoModify.cpp:14) |
| `template/others/MoTree.cpp` | [12](D:/OI/template/template/others/MoTree.cpp:12) |
| `template/others/OverallBinary.cpp` | [16](D:/OI/template/template/others/OverallBinary.cpp:16) |
| `template/string/ACAutomaton.cpp` | [13](D:/OI/template/template/string/ACAutomaton.cpp:13) |
| `template/string/Hash2D.cpp` | [15](D:/OI/template/template/string/Hash2D.cpp:15) |
| `template/string/PAM.cpp` | [14](D:/OI/template/template/string/PAM.cpp:14) |
| `template/string/SAM.cpp` | [14](D:/OI/template/template/string/SAM.cpp:14) |
| `template/string/Trie.cpp` | [11](D:/OI/template/template/string/Trie.cpp:11) |

### tuple、array、bitset、complex、tie、numeric_limits、size_t

41 个命中行，26 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/KDTree.cpp` | [31](D:/OI/template/template/ds/KDTree.cpp:31)、[32](D:/OI/template/template/ds/KDTree.cpp:32) |
| `template/ds/LeftistHeap.cpp` | [33](D:/OI/template/template/ds/LeftistHeap.cpp:33) |
| `template/ds/LiChaoTree.cpp` | [111](D:/OI/template/template/ds/LiChaoTree.cpp:111) |
| `template/ds/PersistentXorTrie.cpp` | [15](D:/OI/template/template/ds/PersistentXorTrie.cpp:15) |
| `template/ds/XorTrie.cpp` | [13](D:/OI/template/template/ds/XorTrie.cpp:13)、[34](D:/OI/template/template/ds/XorTrie.cpp:34) |
| `template/geometry/ClosestPair.cpp` | [50](D:/OI/template/template/geometry/ClosestPair.cpp:50) |
| `template/geometry/MinRectangle.cpp` | [30](D:/OI/template/template/geometry/MinRectangle.cpp:30) |
| `template/geometry/RectangleUnion.cpp` | [39](D:/OI/template/template/geometry/RectangleUnion.cpp:39) |
| `template/graph/DiffConstraints.cpp` | [16](D:/OI/template/template/graph/DiffConstraints.cpp:16) |
| `template/graph/FunctionalGraph.cpp` | [19](D:/OI/template/template/graph/FunctionalGraph.cpp:19) |
| `template/graph/TreeDiameter.cpp` | [29](D:/OI/template/template/graph/TreeDiameter.cpp:29) |
| `template/math/BostanMori.cpp` | [22](D:/OI/template/template/math/BostanMori.cpp:22)、[23](D:/OI/template/template/math/BostanMori.cpp:23)、[31](D:/OI/template/template/math/BostanMori.cpp:31) |
| `template/math/BSGS.cpp` | [28](D:/OI/template/template/math/BSGS.cpp:28) |
| `template/math/Determinant.cpp` | [48](D:/OI/template/template/math/Determinant.cpp:48) |
| `template/math/ExLucas.cpp` | [34](D:/OI/template/template/math/ExLucas.cpp:34) |
| `template/math/FFT.cpp` | [12](D:/OI/template/template/math/FFT.cpp:12)、[22](D:/OI/template/template/math/FFT.cpp:22)、[33](D:/OI/template/template/math/FFT.cpp:33)、[35](D:/OI/template/template/math/FFT.cpp:35)、[37](D:/OI/template/template/math/FFT.cpp:37)、[50](D:/OI/template/template/math/FFT.cpp:50) |
| `template/math/Gauss.cpp` | [79](D:/OI/template/template/math/Gauss.cpp:79)、[80](D:/OI/template/template/math/Gauss.cpp:80) |
| `template/math/Min25.cpp` | [22](D:/OI/template/template/math/Min25.cpp:22)、[53](D:/OI/template/template/math/Min25.cpp:53)、[56](D:/OI/template/template/math/Min25.cpp:56)、[72](D:/OI/template/template/math/Min25.cpp:72) |
| `template/math/Simplex.cpp` | [86](D:/OI/template/template/math/Simplex.cpp:86) |
| `template/math/XorBasis.cpp` | [11](D:/OI/template/template/math/XorBasis.cpp:11) |
| `template/others/CDQ.cpp` | [13](D:/OI/template/template/others/CDQ.cpp:13)、[22](D:/OI/template/template/others/CDQ.cpp:22) |
| `template/string/ACAutomaton.cpp` | [14](D:/OI/template/template/string/ACAutomaton.cpp:14) |
| `template/string/PAM.cpp` | [15](D:/OI/template/template/string/PAM.cpp:15) |
| `template/string/Runs.cpp` | [13](D:/OI/template/template/string/Runs.cpp:13)、[17](D:/OI/template/template/string/Runs.cpp:17) |
| `template/string/SAM.cpp` | [15](D:/OI/template/template/string/SAM.cpp:15) |
| `template/string/Trie.cpp` | [12](D:/OI/template/template/string/Trie.cpp:12) |

### GNU 宽整数及扩展声明

12 个命中行，12 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/LiChaoTree.cpp` | [6](D:/OI/template/template/ds/LiChaoTree.cpp:6) |
| `template/graph/KM.cpp` | [36](D:/OI/template/template/graph/KM.cpp:36) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [20](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:20) |
| `template/graph/MinCostFlow.cpp` | [38](D:/OI/template/template/graph/MinCostFlow.cpp:38) |
| `template/math/CRT.cpp` | [6](D:/OI/template/template/math/CRT.cpp:6) |
| `template/math/DuJiao.cpp` | [7](D:/OI/template/template/math/DuJiao.cpp:7) |
| `template/math/FFT.cpp` | [8](D:/OI/template/template/math/FFT.cpp:8) |
| `template/math/FloorSum.cpp` | [4](D:/OI/template/template/math/FloorSum.cpp:4) |
| `template/math/Min25.cpp` | [6](D:/OI/template/template/math/Min25.cpp:6) |
| `template/math/ModArithmetic.cpp` | [6](D:/OI/template/template/math/ModArithmetic.cpp:6) |
| `template/math/PollardRho.cpp` | [6](D:/OI/template/template/math/PollardRho.cpp:6) |
| `template/others/ConvexHullTrick.cpp` | [6](D:/OI/template/template/others/ConvexHullTrick.cpp:6) |

### 断言

27 个命中行，10 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/FHQTreap.cpp` | [169](D:/OI/template/template/ds/FHQTreap.cpp:169)、[177](D:/OI/template/template/ds/FHQTreap.cpp:177)、[184](D:/OI/template/template/ds/FHQTreap.cpp:184)、[191](D:/OI/template/template/ds/FHQTreap.cpp:191) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [33](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:33)、[40](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:40)、[55](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:55) |
| `template/graph/WeightedMatching.cpp` | [45](D:/OI/template/template/graph/WeightedMatching.cpp:45) |
| `template/math/BostanMori.cpp` | [15](D:/OI/template/template/math/BostanMori.cpp:15) |
| `template/math/Determinant.cpp` | [19](D:/OI/template/template/math/Determinant.cpp:19)、[22](D:/OI/template/template/math/Determinant.cpp:22)、[50](D:/OI/template/template/math/Determinant.cpp:50)、[51](D:/OI/template/template/math/Determinant.cpp:51)、[59](D:/OI/template/template/math/Determinant.cpp:59) |
| `template/math/DuJiao.cpp` | [25](D:/OI/template/template/math/DuJiao.cpp:25) |
| `template/math/FFT.cpp` | [112](D:/OI/template/template/math/FFT.cpp:112)、[114](D:/OI/template/template/math/FFT.cpp:114) |
| `template/math/FPS.cpp` | [62](D:/OI/template/template/math/FPS.cpp:62)、[73](D:/OI/template/template/math/FPS.cpp:73)、[88](D:/OI/template/template/math/FPS.cpp:88)、[102](D:/OI/template/template/math/FPS.cpp:102)、[120](D:/OI/template/template/math/FPS.cpp:120)、[161](D:/OI/template/template/math/FPS.cpp:161)、[186](D:/OI/template/template/math/FPS.cpp:186)、[189](D:/OI/template/template/math/FPS.cpp:189) |
| `template/math/FWT.cpp` | [14](D:/OI/template/template/math/FWT.cpp:14) |
| `template/math/Multipoint.cpp` | [74](D:/OI/template/template/math/Multipoint.cpp:74) |

### 随机数库及时间种子

7 个命中行，4 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/FHQTreap.cpp` | [27](D:/OI/template/template/ds/FHQTreap.cpp:27)、[128](D:/OI/template/template/ds/FHQTreap.cpp:128) |
| `template/geometry/MinCircle.cpp` | [12](D:/OI/template/template/geometry/MinCircle.cpp:12) |
| `template/math/ModSqrt.cpp` | [24](D:/OI/template/template/math/ModSqrt.cpp:24)、[25](D:/OI/template/template/math/ModSqrt.cpp:25) |
| `template/math/PollardRho.cpp` | [45](D:/OI/template/template/math/PollardRho.cpp:45)、[72](D:/OI/template/template/math/PollardRho.cpp:72) |

### 全部 auto 命中行

169 个命中行，71 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/Fenwick2D.cpp` | [18](D:/OI/template/template/ds/Fenwick2D.cpp:18)、[22](D:/OI/template/template/ds/Fenwick2D.cpp:22)、[43](D:/OI/template/template/ds/Fenwick2D.cpp:43)、[47](D:/OI/template/template/ds/Fenwick2D.cpp:47) |
| `template/ds/IntervalSet.cpp` | [20](D:/OI/template/template/ds/IntervalSet.cpp:20)、[31](D:/OI/template/template/ds/IntervalSet.cpp:31)、[34](D:/OI/template/template/ds/IntervalSet.cpp:34)、[42](D:/OI/template/template/ds/IntervalSet.cpp:42)、[47](D:/OI/template/template/ds/IntervalSet.cpp:47) |
| `template/ds/LiChaoTree.cpp` | [66](D:/OI/template/template/ds/LiChaoTree.cpp:66)、[124](D:/OI/template/template/ds/LiChaoTree.cpp:124) |
| `template/ds/MergeSegTree.cpp` | [72](D:/OI/template/template/ds/MergeSegTree.cpp:72)、[73](D:/OI/template/template/ds/MergeSegTree.cpp:73)、[81](D:/OI/template/template/ds/MergeSegTree.cpp:81) |
| `template/ds/RollbackDSU.cpp` | [42](D:/OI/template/template/ds/RollbackDSU.cpp:42) |
| `template/ds/SegmentTreeBeats.cpp` | [26](D:/OI/template/template/ds/SegmentTreeBeats.cpp:26)、[27](D:/OI/template/template/ds/SegmentTreeBeats.cpp:27)、[42](D:/OI/template/template/ds/SegmentTreeBeats.cpp:42)、[51](D:/OI/template/template/ds/SegmentTreeBeats.cpp:51)、[60](D:/OI/template/template/ds/SegmentTreeBeats.cpp:60) |
| `template/geometry/CircleUnion.cpp` | [54](D:/OI/template/template/geometry/CircleUnion.cpp:54)、[62](D:/OI/template/template/geometry/CircleUnion.cpp:62) |
| `template/geometry/ConvexHull.cpp` | [98](D:/OI/template/template/geometry/ConvexHull.cpp:98) |
| `template/geometry/ConvexHull3D.cpp` | [82](D:/OI/template/template/geometry/ConvexHull3D.cpp:82)、[84](D:/OI/template/template/geometry/ConvexHull3D.cpp:84)、[90](D:/OI/template/template/geometry/ConvexHull3D.cpp:90) |
| `template/geometry/HalfPlane.cpp` | [17](D:/OI/template/template/geometry/HalfPlane.cpp:17)、[28](D:/OI/template/template/geometry/HalfPlane.cpp:28)、[32](D:/OI/template/template/geometry/HalfPlane.cpp:32) |
| `template/geometry/MinCircle.cpp` | [22](D:/OI/template/template/geometry/MinCircle.cpp:22) |
| `template/geometry/MinRectangle.cpp` | [33](D:/OI/template/template/geometry/MinRectangle.cpp:33)、[34](D:/OI/template/template/geometry/MinRectangle.cpp:34) |
| `template/geometry/RectangleUnion.cpp` | [25](D:/OI/template/template/geometry/RectangleUnion.cpp:25)、[41](D:/OI/template/template/geometry/RectangleUnion.cpp:41)、[56](D:/OI/template/template/geometry/RectangleUnion.cpp:56) |
| `template/graph/Bridge.cpp` | [30](D:/OI/template/template/graph/Bridge.cpp:30) |
| `template/graph/BridgeTree.cpp` | [15](D:/OI/template/template/graph/BridgeTree.cpp:15) |
| `template/graph/Centroid.cpp` | [32](D:/OI/template/template/graph/Centroid.cpp:32)、[38](D:/OI/template/template/graph/Centroid.cpp:38)、[47](D:/OI/template/template/graph/Centroid.cpp:47)、[67](D:/OI/template/template/graph/Centroid.cpp:67)、[74](D:/OI/template/template/graph/Centroid.cpp:74) |
| `template/graph/CentroidTree.cpp` | [60](D:/OI/template/template/graph/CentroidTree.cpp:60)、[66](D:/OI/template/template/graph/CentroidTree.cpp:66)、[75](D:/OI/template/template/graph/CentroidTree.cpp:75)、[83](D:/OI/template/template/graph/CentroidTree.cpp:83)、[90](D:/OI/template/template/graph/CentroidTree.cpp:90)、[100](D:/OI/template/template/graph/CentroidTree.cpp:100)、[108](D:/OI/template/template/graph/CentroidTree.cpp:108) |
| `template/graph/DiffConstraints.cpp` | [27](D:/OI/template/template/graph/DiffConstraints.cpp:27) |
| `template/graph/Dijkstra.cpp` | [22](D:/OI/template/template/graph/Dijkstra.cpp:22)、[24](D:/OI/template/template/graph/Dijkstra.cpp:24) |
| `template/graph/Dinic.cpp` | [44](D:/OI/template/template/graph/Dinic.cpp:44)、[55](D:/OI/template/template/graph/Dinic.cpp:55) |
| `template/graph/DirectedEulerTrail.cpp` | [16](D:/OI/template/template/graph/DirectedEulerTrail.cpp:16)、[31](D:/OI/template/template/graph/DirectedEulerTrail.cpp:31) |
| `template/graph/DirectedMST.cpp` | [36](D:/OI/template/template/graph/DirectedMST.cpp:36)、[60](D:/OI/template/template/graph/DirectedMST.cpp:60) |
| `template/graph/DynamicConnectivity.cpp` | [38](D:/OI/template/template/graph/DynamicConnectivity.cpp:38)、[53](D:/OI/template/template/graph/DynamicConnectivity.cpp:53)、[68](D:/OI/template/template/graph/DynamicConnectivity.cpp:68)、[71](D:/OI/template/template/graph/DynamicConnectivity.cpp:71)、[76](D:/OI/template/template/graph/DynamicConnectivity.cpp:76) |
| `template/graph/EBCC.cpp` | [52](D:/OI/template/template/graph/EBCC.cpp:52)、[67](D:/OI/template/template/graph/EBCC.cpp:67) |
| `template/graph/FunctionalGraph.cpp` | [51](D:/OI/template/template/graph/FunctionalGraph.cpp:51) |
| `template/graph/GlobalMinCut.cpp` | [33](D:/OI/template/template/graph/GlobalMinCut.cpp:33) |
| `template/graph/GomoryHu.cpp` | [34](D:/OI/template/template/graph/GomoryHu.cpp:34)、[46](D:/OI/template/template/graph/GomoryHu.cpp:46) |
| `template/graph/Johnson.cpp` | [41](D:/OI/template/template/graph/Johnson.cpp:41)、[59](D:/OI/template/template/graph/Johnson.cpp:59)、[61](D:/OI/template/template/graph/Johnson.cpp:61) |
| `template/graph/Kruskal.cpp` | [32](D:/OI/template/template/graph/Kruskal.cpp:32) |
| `template/graph/KruskalTree.cpp` | [38](D:/OI/template/template/graph/KruskalTree.cpp:38)、[42](D:/OI/template/template/graph/KruskalTree.cpp:42) |
| `template/graph/KShortestWalk.cpp` | [59](D:/OI/template/template/graph/KShortestWalk.cpp:59)、[63](D:/OI/template/template/graph/KShortestWalk.cpp:63)、[75](D:/OI/template/template/graph/KShortestWalk.cpp:75)、[88](D:/OI/template/template/graph/KShortestWalk.cpp:88) |
| `template/graph/LowerBoundFlow.cpp` | [41](D:/OI/template/template/graph/LowerBoundFlow.cpp:41)、[51](D:/OI/template/template/graph/LowerBoundFlow.cpp:51)、[62](D:/OI/template/template/graph/LowerBoundFlow.cpp:62)、[104](D:/OI/template/template/graph/LowerBoundFlow.cpp:104)、[127](D:/OI/template/template/graph/LowerBoundFlow.cpp:127)、[131](D:/OI/template/template/graph/LowerBoundFlow.cpp:131) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [51](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:51)、[62](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:62)、[79](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:79) |
| `template/graph/MinCostFlow.cpp` | [53](D:/OI/template/template/graph/MinCostFlow.cpp:53)、[76](D:/OI/template/template/graph/MinCostFlow.cpp:76)、[80](D:/OI/template/template/graph/MinCostFlow.cpp:80)、[101](D:/OI/template/template/graph/MinCostFlow.cpp:101) |
| `template/graph/Reroot.cpp` | [20](D:/OI/template/template/graph/Reroot.cpp:20)、[21](D:/OI/template/template/graph/Reroot.cpp:21)、[27](D:/OI/template/template/graph/Reroot.cpp:27)、[28](D:/OI/template/template/graph/Reroot.cpp:28) |
| `template/graph/ScalingMinCostFlow.cpp` | [50](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:50)、[59](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:59)、[73](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:73)、[78](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:78)、[88](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:88)、[104](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:104) |
| `template/graph/SteinerTree.cpp` | [49](D:/OI/template/template/graph/SteinerTree.cpp:49)、[51](D:/OI/template/template/graph/SteinerTree.cpp:51) |
| `template/graph/TreeDiameter.cpp` | [17](D:/OI/template/template/graph/TreeDiameter.cpp:17)、[20](D:/OI/template/template/graph/TreeDiameter.cpp:20)、[32](D:/OI/template/template/graph/TreeDiameter.cpp:32) |
| `template/graph/UndirectedEulerTrail.cpp` | [16](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:16)、[32](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:32)、[34](D:/OI/template/template/graph/UndirectedEulerTrail.cpp:34) |
| `template/graph/VBCC.cpp` | [65](D:/OI/template/template/graph/VBCC.cpp:65) |
| `template/graph/VirtualTree.cpp` | [60](D:/OI/template/template/graph/VirtualTree.cpp:60) |
| `template/graph/WeightedMatching.cpp` | [119](D:/OI/template/template/graph/WeightedMatching.cpp:119)、[223](D:/OI/template/template/graph/WeightedMatching.cpp:223) |
| `template/graph/ZeroOneBFS.cpp` | [17](D:/OI/template/template/graph/ZeroOneBFS.cpp:17) |
| `template/math/AdaptiveSimpson.cpp` | [16](D:/OI/template/template/math/AdaptiveSimpson.cpp:16)、[21](D:/OI/template/template/math/AdaptiveSimpson.cpp:21) |
| `template/math/BostanMori.cpp` | [20](D:/OI/template/template/math/BostanMori.cpp:20)、[35](D:/OI/template/template/math/BostanMori.cpp:35) |
| `template/math/BSGS.cpp` | [12](D:/OI/template/template/math/BSGS.cpp:12)、[36](D:/OI/template/template/math/BSGS.cpp:36) |
| `template/math/Combination.cpp` | [22](D:/OI/template/template/math/Combination.cpp:22) |
| `template/math/Determinant.cpp` | [21](D:/OI/template/template/math/Determinant.cpp:21)、[25](D:/OI/template/template/math/Determinant.cpp:25)、[53](D:/OI/template/template/math/Determinant.cpp:53)、[58](D:/OI/template/template/math/Determinant.cpp:58) |
| `template/math/DuJiao.cpp` | [53](D:/OI/template/template/math/DuJiao.cpp:53)、[65](D:/OI/template/template/math/DuJiao.cpp:65) |
| `template/math/ExLucas.cpp` | [70](D:/OI/template/template/math/ExLucas.cpp:70) |
| `template/math/FFT.cpp` | [43](D:/OI/template/template/math/FFT.cpp:43) |
| `template/math/Gauss.cpp` | [50](D:/OI/template/template/math/Gauss.cpp:50)、[52](D:/OI/template/template/math/Gauss.cpp:52) |
| `template/math/MatrixInverse.cpp` | [20](D:/OI/template/template/math/MatrixInverse.cpp:20) |
| `template/math/Min25.cpp` | [84](D:/OI/template/template/math/Min25.cpp:84) |
| `template/math/ModSqrt.cpp` | [16](D:/OI/template/template/math/ModSqrt.cpp:16)、[32](D:/OI/template/template/math/ModSqrt.cpp:32) |
| `template/math/Multipoint.cpp` | [58](D:/OI/template/template/math/Multipoint.cpp:58)、[59](D:/OI/template/template/math/Multipoint.cpp:59)、[72](D:/OI/template/template/math/Multipoint.cpp:72)、[77](D:/OI/template/template/math/Multipoint.cpp:77) |
| `template/math/NTT.cpp` | [45](D:/OI/template/template/math/NTT.cpp:45) |
| `template/math/PollardRho.cpp` | [49](D:/OI/template/template/math/PollardRho.cpp:49)、[73](D:/OI/template/template/math/PollardRho.cpp:73) |
| `template/math/PrimitiveRoot.cpp` | [18](D:/OI/template/template/math/PrimitiveRoot.cpp:18) |
| `template/math/SubsetConvolution.cpp` | [24](D:/OI/template/template/math/SubsetConvolution.cpp:24) |
| `template/math/XorBasis.cpp` | [36](D:/OI/template/template/math/XorBasis.cpp:36) |
| `template/others/CDQ.cpp` | [29](D:/OI/template/template/others/CDQ.cpp:29)、[32](D:/OI/template/template/others/CDQ.cpp:32)、[35](D:/OI/template/template/others/CDQ.cpp:35)、[41](D:/OI/template/template/others/CDQ.cpp:41)、[60](D:/OI/template/template/others/CDQ.cpp:60) |
| `template/others/DPOpt.cpp` | [17](D:/OI/template/template/others/DPOpt.cpp:17) |
| `template/others/Mo.cpp` | [22](D:/OI/template/template/others/Mo.cpp:22) |
| `template/others/MoModify.cpp` | [34](D:/OI/template/template/others/MoModify.cpp:34)、[44](D:/OI/template/template/others/MoModify.cpp:44)、[45](D:/OI/template/template/others/MoModify.cpp:45)、[50](D:/OI/template/template/others/MoModify.cpp:50) |
| `template/others/MoTree.cpp` | [55](D:/OI/template/template/others/MoTree.cpp:55)、[62](D:/OI/template/template/others/MoTree.cpp:62)、[64](D:/OI/template/template/others/MoTree.cpp:64) |
| `template/others/OverallBinary.cpp` | [65](D:/OI/template/template/others/OverallBinary.cpp:65)、[78](D:/OI/template/template/others/OverallBinary.cpp:78)、[94](D:/OI/template/template/others/OverallBinary.cpp:94) |
| `template/others/SG.cpp` | [16](D:/OI/template/template/others/SG.cpp:16) |
| `template/string/ACAutomaton.cpp` | [47](D:/OI/template/template/string/ACAutomaton.cpp:47) |
| `template/string/BitsetLCS.cpp` | [27](D:/OI/template/template/string/BitsetLCS.cpp:27) |
| `template/string/SAM.cpp` | [66](D:/OI/template/template/string/SAM.cpp:66)、[68](D:/OI/template/template/string/SAM.cpp:68)、[80](D:/OI/template/template/string/SAM.cpp:80) |

### bits/stdc++.h 以外的头文件

7 个命中行，7 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/FHQTreap.cpp` | [2](D:/OI/template/template/ds/FHQTreap.cpp:2) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [2](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:2) |
| `template/graph/WeightedMatching.cpp` | [2](D:/OI/template/template/graph/WeightedMatching.cpp:2) |
| `template/math/Determinant.cpp` | [2](D:/OI/template/template/math/Determinant.cpp:2) |
| `template/math/DuJiao.cpp` | [2](D:/OI/template/template/math/DuJiao.cpp:2) |
| `template/math/FFT.cpp` | [2](D:/OI/template/template/math/FFT.cpp:2) |
| `template/math/FPS.cpp` | [2](D:/OI/template/template/math/FPS.cpp:2) |

### 其他类型别名声明

8 个命中行，8 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/Dijkstra.cpp` | [9](D:/OI/template/template/graph/Dijkstra.cpp:9) |
| `template/graph/KM.cpp` | [36](D:/OI/template/template/graph/KM.cpp:36) |
| `template/graph/LowerBoundMinCostFlow.cpp` | [20](D:/OI/template/template/graph/LowerBoundMinCostFlow.cpp:20) |
| `template/graph/MinCostFlow.cpp` | [38](D:/OI/template/template/graph/MinCostFlow.cpp:38) |
| `template/graph/Reroot.cpp` | [14](D:/OI/template/template/graph/Reroot.cpp:14) |
| `template/graph/TreeDiameter.cpp` | [13](D:/OI/template/template/graph/TreeDiameter.cpp:13) |
| `template/math/FFT.cpp` | [12](D:/OI/template/template/math/FFT.cpp:12) |
| `template/math/Matrix.cpp` | [14](D:/OI/template/template/math/Matrix.cpp:14) |

### 数字分隔符

2 个命中行，1 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/ScalingMinCostFlow.cpp` | [38](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:38)、[39](D:/OI/template/template/graph/ScalingMinCostFlow.cpp:39) |

### 编译器位运算内建

5 个命中行，4 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/KthAncestor.cpp` | [58](D:/OI/template/template/graph/KthAncestor.cpp:58) |
| `template/math/SubsetConvolution.cpp` | [14](D:/OI/template/template/math/SubsetConvolution.cpp:14)、[18](D:/OI/template/template/math/SubsetConvolution.cpp:18) |
| `template/others/SparseTable.cpp` | [15](D:/OI/template/template/others/SparseTable.cpp:15) |
| `template/string/BitsetLCS.cpp` | [27](D:/OI/template/template/string/BitsetLCS.cpp:27) |

### 显式忽略未使用变量

1 个命中行，1 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/Fenwick2D.cpp` | [18](D:/OI/template/template/ds/Fenwick2D.cpp:18) |

### 默认 bool 开关或 mode 参数

11 个命中行，9 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/Fenwick2D.cpp` | [42](D:/OI/template/template/ds/Fenwick2D.cpp:42) |
| `template/graph/KruskalTree.cpp` | [34](D:/OI/template/template/graph/KruskalTree.cpp:34) |
| `template/graph/LowerBoundFlow.cpp` | [96](D:/OI/template/template/graph/LowerBoundFlow.cpp:96) |
| `template/math/DivisorTransform.cpp` | [15](D:/OI/template/template/math/DivisorTransform.cpp:15)、[26](D:/OI/template/template/math/DivisorTransform.cpp:26) |
| `template/math/FFT.cpp` | [22](D:/OI/template/template/math/FFT.cpp:22)、[69](D:/OI/template/template/math/FFT.cpp:69) |
| `template/math/FPS.cpp` | [30](D:/OI/template/template/math/FPS.cpp:30) |
| `template/math/FWT.cpp` | [53](D:/OI/template/template/math/FWT.cpp:53) |
| `template/math/NTT.cpp` | [23](D:/OI/template/template/math/NTT.cpp:23) |
| `template/others/LIS.cpp` | [11](D:/OI/template/template/others/LIS.cpp:11) |

### 运算符重载

11 个命中行，2 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/geometry/ConvexHull3D.cpp` | [19](D:/OI/template/template/geometry/ConvexHull3D.cpp:19)、[20](D:/OI/template/template/geometry/ConvexHull3D.cpp:20)、[21](D:/OI/template/template/geometry/ConvexHull3D.cpp:21)、[22](D:/OI/template/template/geometry/ConvexHull3D.cpp:22) |
| `template/geometry/Point.cpp` | [23](D:/OI/template/template/geometry/Point.cpp:23)、[24](D:/OI/template/template/geometry/Point.cpp:24)、[25](D:/OI/template/template/geometry/Point.cpp:25)、[26](D:/OI/template/template/geometry/Point.cpp:26)、[28](D:/OI/template/template/geometry/Point.cpp:28)、[33](D:/OI/template/template/geometry/Point.cpp:33)、[54](D:/OI/template/template/geometry/Point.cpp:54) |

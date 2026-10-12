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
| [template/others/莫队.cpp](D:/OI/template/template/others/莫队.cpp:13) | `mo(n, qs, add, remove, query)` | 直接写 `add / del / query`，使用时修改函数体。 |
| [template/others/带修改莫队.cpp](D:/OI/template/template/others/带修改莫队.cpp:31) | `solve(add, del, answer)` | 改为普通成员函数处理增删、修改与答案。 |
| [template/others/树上莫队.cpp](D:/OI/template/template/others/树上莫队.cpp:53) | `solve(add, del, answer)` | 改为普通成员函数处理点统计。 |
| [template/graph/树上启发式合并.cpp](D:/OI/template/template/graph/树上启发式合并.cpp:35)、[递归层](D:/OI/template/template/graph/树上启发式合并.cpp:57) | `build` 和 `dfs` 都携带三个回调类型 | 直接维护状态，调用普通 `add / del / query`。 |

这些是实质上的通用框架，不是单纯用了 `auto`。去掉后会从“直接传回调即可运行”变成“复制后按题目填写维护逻辑”；这是使用方式变化，不应假装完全无影响。不要替换成 `std::function`，那会增加另一层间接调用。

### 2. 单个函数对象参数：是否保留取决于使用习惯

| 位置 | 当前形式 | 可选整理方向 |
| --- | --- | --- |
| [template/graph/树链剖分.cpp](D:/OI/template/template/graph/树链剖分.cpp:60) | `template < class F > path(u, v, F f) const` | 在跳链循环里直接查询或修改线段树；代价是 HLD 不再独立于区间维护。 |
| [template/others/分治动态规划与单调队列多重背包.cpp](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:13) | `dc_dp(prev, Cost cost, inf)` | 普通 `cost(l, r)` 加普通分治函数。 |
| [template/math/自适应辛普森积分.cpp](D:/OI/template/template/math/自适应辛普森积分.cpp:13) | `simpson(F f, ...)` | 普通被积函数 `f(x)`，普通 Simpson 与递归函数。 |
| [template/math/积性函数求和.cpp](D:/OI/template/template/math/积性函数求和.cpp:71) | `sum(c, F prime_power) const` | 普通成员函数计算 `f(p^e)`。 |
| [template/others/三分搜索.cpp](D:/OI/template/template/others/三分搜索.cpp:11)、[最大值版](D:/OI/template/template/others/三分搜索.cpp:20) | `ternary_min/max(F f, ...)` | 普通 `f(x)`。 |

它们都有实际用途，不属于纯粹摆设；如果你更习惯“复制后改函数”，就可以去掉这层通用参数。

### 3. 自传参递归 lambda：12 个文件、13 处

写法是 `auto dfs = [&](auto &&self, ...) -> ...`，递归调用为 `self(self, ...)`。`auto &&` 在这里用于泛型 lambda 的引用参数，并没有调用 `forward`；不能把它误写成已经使用了完美转发。

完整位置：

| 文件 | 声明行 |
| --- | --- |
| [template\graph\有向图欧拉路.cpp](D:/OI/template/template/graph/有向图欧拉路.cpp:31) | `auto dfs = [&](auto &&self, int u) -> void {` |
| [template\graph\最小生成树.cpp](D:/OI/template/template/graph/最小生成树.cpp:32) | `auto find = [&](auto &&self, int x) -> int {` |
| [template\graph\换根动态规划与距离和.cpp](D:/OI/template/template/graph/换根动态规划与距离和.cpp:20) | `auto dfs1 = [&](auto &&self, int u, int fa) -> void {` |
| [template\graph\换根动态规划与距离和.cpp](D:/OI/template/template/graph/换根动态规划与距离和.cpp:27) | `auto dfs2 = [&](auto &&self, int u, int fa) -> void {` |
| [template\graph\树的直径.cpp](D:/OI/template/template/graph/树的直径.cpp:17) | `auto dfs = [&](auto &&self, int u, int fa, ll dis) -> void {` |
| [template\graph\无向图欧拉路.cpp](D:/OI/template/template/graph/无向图欧拉路.cpp:32) | `auto dfs = [&](auto &&self, int u) -> void {` |
| [template\math\自适应辛普森积分.cpp](D:/OI/template/template/math/自适应辛普森积分.cpp:21) | `auto solve = [&](auto &&self, double a, double b, double s,` |
| [template\math\离散对数.cpp](D:/OI/template/template/math/离散对数.cpp:12) | `auto dfs = [&](auto &&self, ll a_, ll b, ll &x, ll &y) -> void {` |
| [template\math\积性函数求和.cpp](D:/OI/template/template/math/积性函数求和.cpp:84) | `auto dfs = [&](auto &&self, ll x, int k) -> ll {` |
| [template\math\素性测试与整数分解.cpp](D:/OI/template/template/math/素性测试与整数分解.cpp:73) | `auto dfs = [&](auto &&self, ull x) -> void {` |
| [template\others\分治三维偏序.cpp](D:/OI/template/template/others/分治三维偏序.cpp:41) | `auto cdq = [&](auto &&self, int l, int r) -> void {` |
| [template\others\分治动态规划与单调队列多重背包.cpp](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:17) | `auto solve = [&](auto &&self, int l, int r, int lo, int hi) -> void {` |
| [template\others\博弈状态函数.cpp](D:/OI/template/template/others/博弈状态函数.cpp:16) | `auto dfs = [&](auto &&self, int u) -> bool {` |

可以换成普通递归函数或已有结构体的成员函数；但不能简单移出函数体后丢掉捕获的数组、图、模数等状态，也不要为了去掉 lambda 另造一套框架。按项目约定，继续保持递归，不换显式栈。

### 4. 只装静态函数的结构体

- [template/math/多项式基本运算.cpp](D:/OI/template/template/math/多项式基本运算.cpp:20)：`struct FPS` 内全部是静态函数和静态模数，通过 `FPS::inv / mul / ln` 等调用。
- [template/math/快速傅里叶变换与任意模数卷积.cpp](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:61)：`struct ModConv` 装三个静态函数，通过 `ModConv::mul / power` 调用。

这是工具函数分组，不是有状态的数据结构。如果不喜欢，可以改普通函数。好处是去掉 `static` 和限定名；代价是处理 `mul / inv / ntt / power` 等重名，并同步 [template/math/多项式多点求值与快速插值.cpp](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:31) 等调用。直接拆开未必减少代码行数。

### 5. 较深的内部封装

- [template/string/二维哈希.cpp](D:/OI/template/template/string/二维哈希.cpp:15)：`Hash2D::Table` 封装单模哈希，再用两个 `Table` 组合双模。确实有两次复用，不建议仅为“少一层”复制两份实现。
- [template/graph/上下界网络流.cpp](D:/OI/template/template/graph/上下界网络流.cpp:22)：`LRFlow::Net` 内再定义残量边 `Net::Edge`，外面另有原始边 `LRFlow::Edge`。可以整理，但原边和残量边是不同数据，不能直接合并概念。
- [template/graph/容量缩放费用流.cpp](D:/OI/template/template/graph/容量缩放费用流.cpp:42)：`flow` 内局部 `Arc`，配合 `add_pair / reduced / dijkstra / add_one` 四个捕获 lambda。若感觉嵌套太多，可以将算法状态和辅助操作改为成员；现在这么写是为了每次求解使用局部状态，不消耗原始边。

普通 `Node / Edge / Query / Factor` 内嵌类型符合项目约定，不自动算作过度封装。

### 6. 专门的结果结构体

- [template/graph/最小生成树.cpp](D:/OI/template/template/graph/最小生成树.cpp:18)：`MST { sum, ids, ok }`。
- [template/math/单纯形与线性规划.cpp](D:/OI/template/template/math/单纯形与线性规划.cpp:17)：`Result { status, value, x }`。
- [template/geometry/最小面积包围矩形.cpp](D:/OI/template/template/geometry/最小面积包围矩形.cpp:13)：`MinRect` 保存面积和矩形顶点。

若不喜欢“结果对象”，可以改返回状态加输出参数；但这些都是多个实际结果，字段名也比多层 `pair` 清楚。没有发现为返回单个数值单独设计的复杂结果包装。

## 其余语法：一起列出，但不应一刀切

| 类别 | 当前位置或规模 | 判断与整理注意事项 |
| --- | --- | --- |
| 普通类型模板 `template < class T >` | `Common / Discretize / LIS / SlidingWindow`，详见附录 | 可以固定 `int / ll`，但会失去现有类型通用性。`chkmin / chkmax` 本来就是个人常用写法，不建议删。 |
| 非类型模板 `template < size_t N >` | [template/math/高斯消元.cpp](D:/OI/template/template/math/高斯消元.cpp:79) | 用于 `bitset<N>` 的编译期长度，是实际类型要求；可固定最大宽度，但不是无意义的模板层。 |
| 成员函数尾部 `const` | 扫描命中 117 行；含跨行签名的末行 | 表示不修改对象；不是封装层。若去掉，必须检查 `const` 对象、`const` 成员函数和传参的连锁影响。 |
| 参数 `const T &`、局部 `const` | 各类 `const` 合计 237 行、105 个文件，含上一项 | 大容器的引用避免复制；不能把 `const &` 整体删除成按值传参。 |
| `constexpr / static constexpr` | 12 行、10 个文件 | 常量声明。命名空间常量可写 `const`；类内常量不能不看使用方式就机械替换。 |
| 普通静态辅助函数 | `BerlekampMassey / FWT / Lagrange / TreeHash` | `static` 影响链接，不是访问控制；单文件复制通常不需要，但删掉也不解决同一文件中的重名。 |
| 默认构造函数 `= default` | [template/string/最长回文子串.cpp](D:/OI/template/template/string/最长回文子串.cpp:14)、[template/string/后缀数组.cpp](D:/OI/template/template/string/后缀数组.cpp:15) | 可写空函数体；已有带参构造函数时，不能直接删除默认构造函数而仍假定支持无参构造。 |
| 构造函数初始化列表 | `Point / Line / DiffConstraints / LowerBoundFlow`，共 4 行 | `P(...) : x(...), y(...)` 是普通初始化。改成函数体赋值未必更简单；容器会先默认构造再赋值。 |
| 构造函数直接调用 `init/build` | 55 行、50 个文件 | 这是普通构造函数里调用函数，不是 C++ 的委托构造函数。可统一手动初始化，但会改变调用方式。 |
| 成员默认值、花括号初始化 | 例如 [template/ds/动态树.cpp](D:/OI/template/template/ds/动态树.cpp:14)、[template/string/字典树.cpp](D:/OI/template/template/string/字典树.cpp:12) | `ch[2]{} / ch{} / n = 0` 保证初值；去掉后必须在所有创建路径补初始化。 |
| `move(...)` | [template/math/最短线性递推.cpp](D:/OI/template/template/math/最短线性递推.cpp:49)、[template/math/扩展卢卡斯定理.cpp](D:/OI/template/template/math/扩展卢卡斯定理.cpp:39)、[template/graph/点双连通分量.cpp](D:/OI/template/template/graph/点双连通分量.cpp:60) | 3 行、4 次调用，避免复制容器；可研究用 `swap` 等价改写，不建议直接删掉而额外深拷贝。 |
| `tie(...)` 多关键字比较 | [template/ds/二维最近邻.cpp](D:/OI/template/template/ds/二维最近邻.cpp:31)、[template/ds/二维最近邻.cpp](D:/OI/template/template/ds/二维最近邻.cpp:32)、[template/ds/左偏树与可并堆.cpp](D:/OI/template/template/ds/左偏树与可并堆.cpp:33) | 可换手写分支；要保留相等时的后续排序条件和编号 tie-break。 |
| `tuple` | [template/graph/差分约束.cpp](D:/OI/template/template/graph/差分约束.cpp:16)、[template/graph/树的直径.cpp](D:/OI/template/template/graph/树的直径.cpp:29)、[template/math/行列式与矩阵树定理.cpp](D:/OI/template/template/math/行列式与矩阵树定理.cpp:48) | 可用 `Edge` 或结果结构体替换，不影响算法；属于表达偏好。 |
| `array` | 多个 Trie、AC、SAM、PAM，以及 `Min25 / Basis / Runs / FunctionalGraph / CDQ / RectangleUnion` | 可按需要换原生数组，但整个数组的赋值、比较、返回能力不同，不能纯文本替换。 |
| `numeric_limits<T>::infinity()` | `LiChaoTree / ClosestPair / MinRectangle / Simplex` | 浮点无穷大有实际意义；换 `1e18` 会改变数值边界。 |
| `size_t` | `BostanMori / BSGS / ExLucas / Gauss` | 容器大小类型；转为 `int` 前确认长度和加法不会溢出，特别是 `size_t(q) + 1`。 |
| `complex<ld>` | [template/math/快速傅里叶变换与任意模数卷积.cpp](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:12) | 标准复数库，不是自造框架；换自定义复数反而增加代码。 |
| 随机数与时间种子 | `FHQTreap / MinCircle / ModSqrt / PollardRho` | `mt19937 / mt19937_64 / chrono / uniform_int_distribution` 有算法用途，不建议直接退成 `rand()`。 |
| `__extension__ / __int128 / __int128_t` | 12 个文件 | `__extension__` 是 GNU 扩展诊断抑制；宽整数用于正确计算，不能为了简单统一改为 `ll`。 |
| 数字分隔符 `1'000'...` | [template/graph/容量缩放费用流.cpp](D:/OI/template/template/graph/容量缩放费用流.cpp:38)、[template/graph/容量缩放费用流.cpp](D:/OI/template/template/graph/容量缩放费用流.cpp:39) | 纯字面量写法，可去掉单引号，不改数值。 |
| `assert(...)` | 27 行、10 个文件 | 算法前置条件和范围检查，保留。断言不是复杂错误处理框架，不能当成多余代码统一删。 |
| 重复 `#include <cassert>` | 7 个文件 | 已含 `bits/stdc++.h`，这个头在当前 GCC 环境下冗余，可以单独删；断言本身不删。 |
| `(void)y` | [template/ds/二维树状数组.cpp](D:/OI/template/template/ds/二维树状数组.cpp:18) | 仅抑制未使用警告。可改成遍历 `const auto &p` 后取 `p.first`，直接省掉不用的绑定。 |
| `auto`、结构化绑定、范围循环 | `auto` 命中 169 行、71 个文件 | 普通竞赛写法，项目已明确允许。不与递归 lambda、泛型回调混为一谈。 |
| 普通 lambda | 52 个声明起始行，含前述 13 个递归 lambda | 排序比较器、局部快幂等通常很短，单独移出未必更清楚；全部位置见附录。 |
| `__builtin_clz/ctz/popcount*` | `KthAncestor / SubsetConvolution / SparseTable / BitsetLCS` | 编译器位运算内建，不是工程封装；替换时保持非零前提与位宽。 |
| `Graph / Matrix / Complex` 别名 | `Dijkstra / Reroot / TreeDiameter / Matrix / FFT` | 只是缩短已有类型，不是额外对象或包装。 |
| 几何运算符重载 `operator + / - / * / / / < / == / <<` | `Point` 和 `ConvexHull3D` | 向量运算属于常见几何模板写法；按项目要求优先保留原接口。 |
| 一个函数用开关兼顾多种操作 | `LRFlow::solve(..., mode)`、`KruskalTree::build(descending)`、`LIS(strict)`、各种变换的逆变换标记等 | 都对应实际数学操作，不是预留的未知配置。可以拆成明确函数名，但函数数目可能增加。 |
| 公开入口加递归重载 | 例如 [template/ds/动态开点线段树合并与分裂.cpp](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:35)、[template/ds/可持久化数组.cpp](D:/OI/template/template/ds/可持久化数组.cpp:54) | `query(root,l,r)` 转入带树区间的递归函数，属于常用板子写法；直接删除入口会把内部参数暴露给调用者。 |
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
| `template/graph/树上启发式合并.cpp` | [35](D:/OI/template/template/graph/树上启发式合并.cpp:35)、[57](D:/OI/template/template/graph/树上启发式合并.cpp:57) |
| `template/graph/树链剖分.cpp` | [60](D:/OI/template/template/graph/树链剖分.cpp:60) |
| `template/math/自适应辛普森积分.cpp` | [13](D:/OI/template/template/math/自适应辛普森积分.cpp:13) |
| `template/math/高斯消元.cpp` | [79](D:/OI/template/template/math/高斯消元.cpp:79) |
| `template/math/积性函数求和.cpp` | [71](D:/OI/template/template/math/积性函数求和.cpp:71) |
| `template/others/常用定义.cpp` | [15](D:/OI/template/template/others/常用定义.cpp:15)、[16](D:/OI/template/template/others/常用定义.cpp:16)、[17](D:/OI/template/template/others/常用定义.cpp:17)、[18](D:/OI/template/template/others/常用定义.cpp:18) |
| `template/others/离散化.cpp` | [9](D:/OI/template/template/others/离散化.cpp:9) |
| `template/others/分治动态规划与单调队列多重背包.cpp` | [13](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:13) |
| `template/others/最长上升子序列与方案.cpp` | [10](D:/OI/template/template/others/最长上升子序列与方案.cpp:10) |
| `template/others/莫队.cpp` | [13](D:/OI/template/template/others/莫队.cpp:13) |
| `template/others/带修改莫队.cpp` | [31](D:/OI/template/template/others/带修改莫队.cpp:31) |
| `template/others/树上莫队.cpp` | [53](D:/OI/template/template/others/树上莫队.cpp:53) |
| `template/others/滑动窗口最值.cpp` | [10](D:/OI/template/template/others/滑动窗口最值.cpp:10)、[23](D:/OI/template/template/others/滑动窗口最值.cpp:23) |
| `template/others/三分搜索.cpp` | [11](D:/OI/template/template/others/三分搜索.cpp:11)、[20](D:/OI/template/template/others/三分搜索.cpp:20) |

### 自传参递归 lambda

13 个命中行，12 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/有向图欧拉路.cpp` | [31](D:/OI/template/template/graph/有向图欧拉路.cpp:31) |
| `template/graph/最小生成树.cpp` | [32](D:/OI/template/template/graph/最小生成树.cpp:32) |
| `template/graph/换根动态规划与距离和.cpp` | [20](D:/OI/template/template/graph/换根动态规划与距离和.cpp:20)、[27](D:/OI/template/template/graph/换根动态规划与距离和.cpp:27) |
| `template/graph/树的直径.cpp` | [17](D:/OI/template/template/graph/树的直径.cpp:17) |
| `template/graph/无向图欧拉路.cpp` | [32](D:/OI/template/template/graph/无向图欧拉路.cpp:32) |
| `template/math/自适应辛普森积分.cpp` | [21](D:/OI/template/template/math/自适应辛普森积分.cpp:21) |
| `template/math/离散对数.cpp` | [12](D:/OI/template/template/math/离散对数.cpp:12) |
| `template/math/积性函数求和.cpp` | [84](D:/OI/template/template/math/积性函数求和.cpp:84) |
| `template/math/素性测试与整数分解.cpp` | [73](D:/OI/template/template/math/素性测试与整数分解.cpp:73) |
| `template/others/分治三维偏序.cpp` | [41](D:/OI/template/template/others/分治三维偏序.cpp:41) |
| `template/others/分治动态规划与单调队列多重背包.cpp` | [17](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:17) |
| `template/others/博弈状态函数.cpp` | [16](D:/OI/template/template/others/博弈状态函数.cpp:16) |

### 全部 lambda 声明起始行

52 个命中行，32 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/二维最近邻.cpp` | [30](D:/OI/template/template/ds/二维最近邻.cpp:30) |
| `template/geometry/圆面积并与周长并.cpp` | [54](D:/OI/template/template/geometry/圆面积并与周长并.cpp:54) |
| `template/geometry/平面最近点对.cpp` | [25](D:/OI/template/template/geometry/平面最近点对.cpp:25)、[34](D:/OI/template/template/geometry/平面最近点对.cpp:34)、[49](D:/OI/template/template/geometry/平面最近点对.cpp:49) |
| `template/geometry/凸包与直径与闵可夫斯基和.cpp` | [17](D:/OI/template/template/geometry/凸包与直径与闵可夫斯基和.cpp:17)、[38](D:/OI/template/template/geometry/凸包与直径与闵可夫斯基和.cpp:38)、[98](D:/OI/template/template/geometry/凸包与直径与闵可夫斯基和.cpp:98) |
| `template/geometry/半平面交.cpp` | [13](D:/OI/template/template/geometry/半平面交.cpp:13)、[28](D:/OI/template/template/geometry/半平面交.cpp:28) |
| `template/geometry/最小面积包围矩形.cpp` | [33](D:/OI/template/template/geometry/最小面积包围矩形.cpp:33)、[34](D:/OI/template/template/geometry/最小面积包围矩形.cpp:34) |
| `template/geometry/矩形面积并与周长并.cpp` | [50](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:50) |
| `template/graph/有向图欧拉路.cpp` | [31](D:/OI/template/template/graph/有向图欧拉路.cpp:31) |
| `template/graph/最小生成树.cpp` | [25](D:/OI/template/template/graph/最小生成树.cpp:25)、[32](D:/OI/template/template/graph/最小生成树.cpp:32) |
| `template/graph/重构树.cpp` | [39](D:/OI/template/template/graph/重构树.cpp:39) |
| `template/graph/换根动态规划与距离和.cpp` | [20](D:/OI/template/template/graph/换根动态规划与距离和.cpp:20)、[27](D:/OI/template/template/graph/换根动态规划与距离和.cpp:27) |
| `template/graph/容量缩放费用流.cpp` | [50](D:/OI/template/template/graph/容量缩放费用流.cpp:50)、[73](D:/OI/template/template/graph/容量缩放费用流.cpp:73)、[78](D:/OI/template/template/graph/容量缩放费用流.cpp:78)、[104](D:/OI/template/template/graph/容量缩放费用流.cpp:104) |
| `template/graph/树的直径.cpp` | [17](D:/OI/template/template/graph/树的直径.cpp:17) |
| `template/graph/无向图欧拉路.cpp` | [32](D:/OI/template/template/graph/无向图欧拉路.cpp:32) |
| `template/graph/虚树.cpp` | [60](D:/OI/template/template/graph/虚树.cpp:60) |
| `template/graph/一般图最大权匹配.cpp` | [119](D:/OI/template/template/graph/一般图最大权匹配.cpp:119) |
| `template/math/自适应辛普森积分.cpp` | [16](D:/OI/template/template/math/自适应辛普森积分.cpp:16)、[21](D:/OI/template/template/math/自适应辛普森积分.cpp:21) |
| `template/math/离散对数.cpp` | [12](D:/OI/template/template/math/离散对数.cpp:12) |
| `template/math/组合数.cpp` | [22](D:/OI/template/template/math/组合数.cpp:22) |
| `template/math/行列式与矩阵树定理.cpp` | [25](D:/OI/template/template/math/行列式与矩阵树定理.cpp:25)、[53](D:/OI/template/template/math/行列式与矩阵树定理.cpp:53) |
| `template/math/高斯消元.cpp` | [52](D:/OI/template/template/math/高斯消元.cpp:52) |
| `template/math/矩阵求逆.cpp` | [20](D:/OI/template/template/math/矩阵求逆.cpp:20) |
| `template/math/积性函数求和.cpp` | [84](D:/OI/template/template/math/积性函数求和.cpp:84) |
| `template/math/二次剩余.cpp` | [16](D:/OI/template/template/math/二次剩余.cpp:16)、[32](D:/OI/template/template/math/二次剩余.cpp:32) |
| `template/math/素性测试与整数分解.cpp` | [49](D:/OI/template/template/math/素性测试与整数分解.cpp:49)、[73](D:/OI/template/template/math/素性测试与整数分解.cpp:73) |
| `template/math/子集卷积.cpp` | [24](D:/OI/template/template/math/子集卷积.cpp:24) |
| `template/others/分治三维偏序.cpp` | [18](D:/OI/template/template/others/分治三维偏序.cpp:18)、[32](D:/OI/template/template/others/分治三维偏序.cpp:32)、[35](D:/OI/template/template/others/分治三维偏序.cpp:35)、[41](D:/OI/template/template/others/分治三维偏序.cpp:41) |
| `template/others/分治动态规划与单调队列多重背包.cpp` | [17](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:17) |
| `template/others/莫队.cpp` | [16](D:/OI/template/template/others/莫队.cpp:16) |
| `template/others/带修改莫队.cpp` | [35](D:/OI/template/template/others/带修改莫队.cpp:35)、[44](D:/OI/template/template/others/带修改莫队.cpp:44) |
| `template/others/树上莫队.cpp` | [57](D:/OI/template/template/others/树上莫队.cpp:57)、[62](D:/OI/template/template/others/树上莫队.cpp:62) |
| `template/others/博弈状态函数.cpp` | [16](D:/OI/template/template/others/博弈状态函数.cpp:16) |

### 静态函数声明

20 个命中行，6 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/math/最短线性递推.cpp` | [13](D:/OI/template/template/math/最短线性递推.cpp:13)、[18](D:/OI/template/template/math/最短线性递推.cpp:18)、[62](D:/OI/template/template/math/最短线性递推.cpp:62) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [62](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:62)、[69](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:69)、[97](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:97) |
| `template/math/多项式基本运算.cpp` | [23](D:/OI/template/template/math/多项式基本运算.cpp:23)、[30](D:/OI/template/template/math/多项式基本运算.cpp:30)、[58](D:/OI/template/template/math/多项式基本运算.cpp:58)、[71](D:/OI/template/template/math/多项式基本运算.cpp:71)、[86](D:/OI/template/template/math/多项式基本运算.cpp:86)、[100](D:/OI/template/template/math/多项式基本运算.cpp:100)、[117](D:/OI/template/template/math/多项式基本运算.cpp:117)、[141](D:/OI/template/template/math/多项式基本运算.cpp:141)、[159](D:/OI/template/template/math/多项式基本运算.cpp:159)、[184](D:/OI/template/template/math/多项式基本运算.cpp:184) |
| `template/math/快速沃尔什变换.cpp` | [13](D:/OI/template/template/math/快速沃尔什变换.cpp:13)、[16](D:/OI/template/template/math/快速沃尔什变换.cpp:16) |
| `template/math/拉格朗日插值.cpp` | [13](D:/OI/template/template/math/拉格朗日插值.cpp:13) |
| `template/others/树哈希.cpp` | [10](D:/OI/template/template/others/树哈希.cpp:10) |

### constexpr 常量

12 个命中行，10 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/吉司机线段树.cpp` | [16](D:/OI/template/template/ds/吉司机线段树.cpp:16) |
| `template/graph/重赋权全源最短路.cpp` | [17](D:/OI/template/template/graph/重赋权全源最短路.cpp:17) |
| `template/graph/第几短游走.cpp` | [16](D:/OI/template/template/graph/第几短游走.cpp:16) |
| `template/graph/容量缩放费用流.cpp` | [38](D:/OI/template/template/graph/容量缩放费用流.cpp:38)、[39](D:/OI/template/template/graph/容量缩放费用流.cpp:39) |
| `template/graph/最小斯坦纳树.cpp` | [16](D:/OI/template/template/graph/最小斯坦纳树.cpp:16) |
| `template/math/多项式基本运算.cpp` | [21](D:/OI/template/template/math/多项式基本运算.cpp:21) |
| `template/math/多项式多点求值与快速插值.cpp` | [13](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:13) |
| `template/math/快速数论变换与多项式乘法.cpp` | [12](D:/OI/template/template/math/快速数论变换与多项式乘法.cpp:12)、[13](D:/OI/template/template/math/快速数论变换与多项式乘法.cpp:13) |
| `template/string/字符串哈希.cpp` | [14](D:/OI/template/template/string/字符串哈希.cpp:14) |
| `template/string/二维哈希.cpp` | [16](D:/OI/template/template/string/二维哈希.cpp:16) |

### 全部 const 命中行

237 个命中行，105 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/仿射线段树.cpp` | [17](D:/OI/template/template/ds/仿射线段树.cpp:17)、[33](D:/OI/template/template/ds/仿射线段树.cpp:33)、[40](D:/OI/template/template/ds/仿射线段树.cpp:40) |
| `template/ds/树状数组.cpp` | [17](D:/OI/template/template/ds/树状数组.cpp:17)、[24](D:/OI/template/template/ds/树状数组.cpp:24)、[37](D:/OI/template/template/ds/树状数组.cpp:37)、[43](D:/OI/template/template/ds/树状数组.cpp:43)、[49](D:/OI/template/template/ds/树状数组.cpp:49) |
| `template/ds/二维树状数组.cpp` | [16](D:/OI/template/template/ds/二维树状数组.cpp:16)、[42](D:/OI/template/template/ds/二维树状数组.cpp:42)、[54](D:/OI/template/template/ds/二维树状数组.cpp:54) |
| `template/ds/无旋平衡树.cpp` | [31](D:/OI/template/template/ds/无旋平衡树.cpp:31)、[79](D:/OI/template/template/ds/无旋平衡树.cpp:79)、[88](D:/OI/template/template/ds/无旋平衡树.cpp:88)、[100](D:/OI/template/template/ds/无旋平衡树.cpp:100)、[109](D:/OI/template/template/ds/无旋平衡树.cpp:109)、[132](D:/OI/template/template/ds/无旋平衡树.cpp:132) |
| `template/ds/区间集合与最小未出现数.cpp` | [41](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:41)、[46](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:46) |
| `template/ds/二维最近邻.cpp` | [30](D:/OI/template/template/ds/二维最近邻.cpp:30)、[46](D:/OI/template/template/ds/二维最近邻.cpp:46)、[54](D:/OI/template/template/ds/二维最近邻.cpp:54)、[61](D:/OI/template/template/ds/二维最近邻.cpp:61)、[63](D:/OI/template/template/ds/二维最近邻.cpp:63)、[74](D:/OI/template/template/ds/二维最近邻.cpp:74)、[80](D:/OI/template/template/ds/二维最近邻.cpp:80)、[82](D:/OI/template/template/ds/二维最近邻.cpp:82)、[90](D:/OI/template/template/ds/二维最近邻.cpp:90) |
| `template/ds/动态树.cpp` | [17](D:/OI/template/template/ds/动态树.cpp:17)、[21](D:/OI/template/template/ds/动态树.cpp:21) |
| `template/ds/左偏树与可并堆.cpp` | [29](D:/OI/template/template/ds/左偏树与可并堆.cpp:29) |
| `template/ds/李超线段树.cpp` | [24](D:/OI/template/template/ds/李超线段树.cpp:24)、[92](D:/OI/template/template/ds/李超线段树.cpp:92)、[109](D:/OI/template/template/ds/李超线段树.cpp:109)、[120](D:/OI/template/template/ds/李超线段树.cpp:120)、[124](D:/OI/template/template/ds/李超线段树.cpp:124) |
| `template/ds/动态开点线段树合并与分裂.cpp` | [35](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:35)、[42](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:42)、[44](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:44)、[52](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:52) |
| `template/ds/可持久化数组.cpp` | [22](D:/OI/template/template/ds/可持久化数组.cpp:22)、[32](D:/OI/template/template/ds/可持久化数组.cpp:32)、[54](D:/OI/template/template/ds/可持久化数组.cpp:54)、[65](D:/OI/template/template/ds/可持久化数组.cpp:65) |
| `template/ds/主席树.cpp` | [29](D:/OI/template/template/ds/主席树.cpp:29)、[41](D:/OI/template/template/ds/主席树.cpp:41)、[48](D:/OI/template/template/ds/主席树.cpp:48)、[52](D:/OI/template/template/ds/主席树.cpp:52)、[60](D:/OI/template/template/ds/主席树.cpp:60) |
| `template/ds/可持久化二进制字典树.cpp` | [35](D:/OI/template/template/ds/可持久化二进制字典树.cpp:35) |
| `template/ds/可撤销并查集.cpp` | [23](D:/OI/template/template/ds/可撤销并查集.cpp:23)、[28](D:/OI/template/template/ds/可撤销并查集.cpp:28) |
| `template/ds/吉司机线段树.cpp` | [81](D:/OI/template/template/ds/吉司机线段树.cpp:81)、[93](D:/OI/template/template/ds/吉司机线段树.cpp:93) |
| `template/ds/线段树与树上二分.cpp` | [38](D:/OI/template/template/ds/线段树与树上二分.cpp:38)、[46](D:/OI/template/template/ds/线段树与树上二分.cpp:46) |
| `template/ds/二进制字典树.cpp` | [45](D:/OI/template/template/ds/二进制字典树.cpp:45) |
| `template/geometry/圆.cpp` | [14](D:/OI/template/template/geometry/圆.cpp:14)、[93](D:/OI/template/template/geometry/圆.cpp:93) |
| `template/geometry/圆面积并与周长并.cpp` | [16](D:/OI/template/template/geometry/圆面积并与周长并.cpp:16)、[17](D:/OI/template/template/geometry/圆面积并与周长并.cpp:17) |
| `template/geometry/平面最近点对.cpp` | [46](D:/OI/template/template/geometry/平面最近点对.cpp:46) |
| `template/geometry/三维凸包.cpp` | [19](D:/OI/template/template/geometry/三维凸包.cpp:19)、[20](D:/OI/template/template/geometry/三维凸包.cpp:20)、[21](D:/OI/template/template/geometry/三维凸包.cpp:21)、[22](D:/OI/template/template/geometry/三维凸包.cpp:22)、[23](D:/OI/template/template/geometry/三维凸包.cpp:23)、[24](D:/OI/template/template/geometry/三维凸包.cpp:24)、[28](D:/OI/template/template/geometry/三维凸包.cpp:28)、[39](D:/OI/template/template/geometry/三维凸包.cpp:39)、[49](D:/OI/template/template/geometry/三维凸包.cpp:49)、[94](D:/OI/template/template/geometry/三维凸包.cpp:94)、[100](D:/OI/template/template/geometry/三维凸包.cpp:100) |
| `template/geometry/半平面交.cpp` | [13](D:/OI/template/template/geometry/半平面交.cpp:13) |
| `template/geometry/点与向量.cpp` | [10](D:/OI/template/template/geometry/点与向量.cpp:10)、[28](D:/OI/template/template/geometry/点与向量.cpp:28)、[33](D:/OI/template/template/geometry/点与向量.cpp:33)、[44](D:/OI/template/template/geometry/点与向量.cpp:44)、[45](D:/OI/template/template/geometry/点与向量.cpp:45)、[54](D:/OI/template/template/geometry/点与向量.cpp:54) |
| `template/geometry/矩形面积并与周长并.cpp` | [39](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:39) |
| `template/graph/圆方树.cpp` | [12](D:/OI/template/template/graph/圆方树.cpp:12) |
| `template/graph/边双缩点树.cpp` | [11](D:/OI/template/template/graph/边双缩点树.cpp:11)、[12](D:/OI/template/template/graph/边双缩点树.cpp:12) |
| `template/graph/点分治.cpp` | [51](D:/OI/template/template/graph/点分治.cpp:51) |
| `template/graph/点分树.cpp` | [30](D:/OI/template/template/graph/点分树.cpp:30)、[106](D:/OI/template/template/graph/点分树.cpp:106) |
| `template/graph/差分约束.cpp` | [23](D:/OI/template/template/graph/差分约束.cpp:23) |
| `template/graph/堆优化最短路.cpp` | [14](D:/OI/template/template/graph/堆优化最短路.cpp:14) |
| `template/graph/有向图欧拉路.cpp` | [12](D:/OI/template/template/graph/有向图欧拉路.cpp:12) |
| `template/graph/最小树形图.cpp` | [29](D:/OI/template/template/graph/最小树形图.cpp:29) |
| `template/graph/离线动态连通性.cpp` | [29](D:/OI/template/template/graph/离线动态连通性.cpp:29) |
| `template/graph/欧拉序最近公共祖先.cpp` | [61](D:/OI/template/template/graph/欧拉序最近公共祖先.cpp:61) |
| `template/graph/函数图分解与跳跃.cpp` | [28](D:/OI/template/template/graph/函数图分解与跳跃.cpp:28)、[51](D:/OI/template/template/graph/函数图分解与跳跃.cpp:51)、[58](D:/OI/template/template/graph/函数图分解与跳跃.cpp:58)、[63](D:/OI/template/template/graph/函数图分解与跳跃.cpp:63) |
| `template/graph/全局最小割.cpp` | [31](D:/OI/template/template/graph/全局最小割.cpp:31) |
| `template/graph/最小割树.cpp` | [68](D:/OI/template/template/graph/最小割树.cpp:68) |
| `template/graph/树链剖分.cpp` | [52](D:/OI/template/template/graph/树链剖分.cpp:52)、[60](D:/OI/template/template/graph/树链剖分.cpp:60) |
| `template/graph/分层增广二分图匹配.cpp` | [73](D:/OI/template/template/graph/分层增广二分图匹配.cpp:73) |
| `template/graph/重赋权全源最短路.cpp` | [36](D:/OI/template/template/graph/重赋权全源最短路.cpp:36) |
| `template/graph/二分图最大权匹配.cpp` | [37](D:/OI/template/template/graph/二分图最大权匹配.cpp:37) |
| `template/graph/最小生成树.cpp` | [25](D:/OI/template/template/graph/最小生成树.cpp:25)、[37](D:/OI/template/template/graph/最小生成树.cpp:37) |
| `template/graph/重构树.cpp` | [56](D:/OI/template/template/graph/重构树.cpp:56)、[67](D:/OI/template/template/graph/重构树.cpp:67) |
| `template/graph/长链剖分与祖先查询.cpp` | [55](D:/OI/template/template/graph/长链剖分与祖先查询.cpp:55) |
| `template/graph/倍增最近公共祖先.cpp` | [46](D:/OI/template/template/graph/倍增最近公共祖先.cpp:46) |
| `template/graph/最大团与最大独立集.cpp` | [29](D:/OI/template/template/graph/最大团与最大独立集.cpp:29)、[31](D:/OI/template/template/graph/最大团与最大独立集.cpp:31) |
| `template/graph/换根动态规划与距离和.cpp` | [16](D:/OI/template/template/graph/换根动态规划与距离和.cpp:16) |
| `template/graph/容量缩放费用流.cpp` | [34](D:/OI/template/template/graph/容量缩放费用流.cpp:34)、[40](D:/OI/template/template/graph/容量缩放费用流.cpp:40)、[59](D:/OI/template/template/graph/容量缩放费用流.cpp:59)、[62](D:/OI/template/template/graph/容量缩放费用流.cpp:62)、[68](D:/OI/template/template/graph/容量缩放费用流.cpp:68) |
| `template/graph/最小斯坦纳树.cpp` | [27](D:/OI/template/template/graph/最小斯坦纳树.cpp:27) |
| `template/graph/拓扑排序.cpp` | [11](D:/OI/template/template/graph/拓扑排序.cpp:11) |
| `template/graph/树的直径.cpp` | [15](D:/OI/template/template/graph/树的直径.cpp:15)、[29](D:/OI/template/template/graph/树的直径.cpp:29) |
| `template/graph/无向图欧拉路.cpp` | [12](D:/OI/template/template/graph/无向图欧拉路.cpp:12) |
| `template/graph/虚树.cpp` | [46](D:/OI/template/template/graph/虚树.cpp:46)、[48](D:/OI/template/template/graph/虚树.cpp:48) |
| `template/graph/一般图最大权匹配.cpp` | [49](D:/OI/template/template/graph/一般图最大权匹配.cpp:49) |
| `template/graph/零一最短路.cpp` | [9](D:/OI/template/template/graph/零一最短路.cpp:9) |
| `template/math/最短线性递推.cpp` | [27](D:/OI/template/template/math/最短线性递推.cpp:27)、[62](D:/OI/template/template/math/最短线性递推.cpp:62)、[63](D:/OI/template/template/math/最短线性递推.cpp:63)、[77](D:/OI/template/template/math/最短线性递推.cpp:77) |
| `template/math/线性递推快速求项.cpp` | [29](D:/OI/template/template/math/线性递推快速求项.cpp:29) |
| `template/math/组合数.cpp` | [33](D:/OI/template/template/math/组合数.cpp:33) |
| `template/math/扩展欧几里得与中国剩余定理.cpp` | [33](D:/OI/template/template/math/扩展欧几里得与中国剩余定理.cpp:33) |
| `template/math/行列式与矩阵树定理.cpp` | [48](D:/OI/template/template/math/行列式与矩阵树定理.cpp:48) |
| `template/math/等距拉格朗日插值.cpp` | [17](D:/OI/template/template/math/等距拉格朗日插值.cpp:17)、[22](D:/OI/template/template/math/等距拉格朗日插值.cpp:22) |
| `template/math/扩展卢卡斯定理.cpp` | [60](D:/OI/template/template/math/扩展卢卡斯定理.cpp:60)、[70](D:/OI/template/template/math/扩展卢卡斯定理.cpp:70) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [30](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:30)、[46](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:46)、[97](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:97)、[111](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:111)、[118](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:118) |
| `template/math/多项式基本运算.cpp` | [71](D:/OI/template/template/math/多项式基本运算.cpp:71)、[86](D:/OI/template/template/math/多项式基本运算.cpp:86)、[100](D:/OI/template/template/math/多项式基本运算.cpp:100)、[117](D:/OI/template/template/math/多项式基本运算.cpp:117)、[159](D:/OI/template/template/math/多项式基本运算.cpp:159) |
| `template/math/快速沃尔什变换.cpp` | [13](D:/OI/template/template/math/快速沃尔什变换.cpp:13) |
| `template/math/拉格朗日插值.cpp` | [22](D:/OI/template/template/math/拉格朗日插值.cpp:22)、[23](D:/OI/template/template/math/拉格朗日插值.cpp:23) |
| `template/math/卢卡斯定理.cpp` | [29](D:/OI/template/template/math/卢卡斯定理.cpp:29) |
| `template/math/矩阵快速幂.cpp` | [16](D:/OI/template/template/math/矩阵快速幂.cpp:16) |
| `template/math/积性函数求和.cpp` | [24](D:/OI/template/template/math/积性函数求和.cpp:24)、[25](D:/OI/template/template/math/积性函数求和.cpp:25)、[69](D:/OI/template/template/math/积性函数求和.cpp:69)、[72](D:/OI/template/template/math/积性函数求和.cpp:72) |
| `template/math/快速幂与逆元.cpp` | [32](D:/OI/template/template/math/快速幂与逆元.cpp:32) |
| `template/math/多项式多点求值与快速插值.cpp` | [25](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:25)、[31](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:31)、[35](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:35)、[36](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:36)、[48](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:48)、[55](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:55)、[68](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:68) |
| `template/math/单纯形与线性规划.cpp` | [56](D:/OI/template/template/math/单纯形与线性规划.cpp:56)、[57](D:/OI/template/template/math/单纯形与线性规划.cpp:57) |
| `template/math/子集卷积.cpp` | [12](D:/OI/template/template/math/子集卷积.cpp:12) |
| `template/math/异或线性基.cpp` | [21](D:/OI/template/template/math/异或线性基.cpp:21)、[28](D:/OI/template/template/math/异或线性基.cpp:28)、[35](D:/OI/template/template/math/异或线性基.cpp:35) |
| `template/others/笛卡尔树.cpp` | [16](D:/OI/template/template/others/笛卡尔树.cpp:16) |
| `template/others/分治三维偏序.cpp` | [13](D:/OI/template/template/others/分治三维偏序.cpp:13)、[60](D:/OI/template/template/others/分治三维偏序.cpp:60) |
| `template/others/常用定义.cpp` | [15](D:/OI/template/template/others/常用定义.cpp:15)、[16](D:/OI/template/template/others/常用定义.cpp:16) |
| `template/others/单调凸包与斜率优化.cpp` | [19](D:/OI/template/template/others/单调凸包与斜率优化.cpp:19)、[26](D:/OI/template/template/others/单调凸包与斜率优化.cpp:26) |
| `template/others/离散化.cpp` | [10](D:/OI/template/template/others/离散化.cpp:10)、[17](D:/OI/template/template/others/离散化.cpp:17) |
| `template/others/舞蹈链精确覆盖.cpp` | [31](D:/OI/template/template/others/舞蹈链精确覆盖.cpp:31) |
| `template/others/分治动态规划与单调队列多重背包.cpp` | [14](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:14) |
| `template/others/最长上升子序列与方案.cpp` | [11](D:/OI/template/template/others/最长上升子序列与方案.cpp:11) |
| `template/others/莫队.cpp` | [16](D:/OI/template/template/others/莫队.cpp:16)、[22](D:/OI/template/template/others/莫队.cpp:22) |
| `template/others/带修改莫队.cpp` | [20](D:/OI/template/template/others/带修改莫队.cpp:20)、[32](D:/OI/template/template/others/带修改莫队.cpp:32) |
| `template/others/单调栈与左右最近更小与更大.cpp` | [12](D:/OI/template/template/others/单调栈与左右最近更小与更大.cpp:12) |
| `template/others/树上莫队.cpp` | [37](D:/OI/template/template/others/树上莫队.cpp:37)、[54](D:/OI/template/template/others/树上莫队.cpp:54) |
| `template/others/整体二分与动态区间第几小.cpp` | [27](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:27)、[51](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:51)、[78](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:78) |
| `template/others/博弈状态函数.cpp` | [12](D:/OI/template/template/others/博弈状态函数.cpp:12) |
| `template/others/滑动窗口最值.cpp` | [11](D:/OI/template/template/others/滑动窗口最值.cpp:11)、[24](D:/OI/template/template/others/滑动窗口最值.cpp:24) |
| `template/others/稀疏表.cpp` | [14](D:/OI/template/template/others/稀疏表.cpp:14)、[24](D:/OI/template/template/others/稀疏表.cpp:24) |
| `template/others/树哈希.cpp` | [17](D:/OI/template/template/others/树哈希.cpp:17) |
| `template/string/多模式匹配自动机.cpp` | [29](D:/OI/template/template/string/多模式匹配自动机.cpp:29)、[66](D:/OI/template/template/string/多模式匹配自动机.cpp:66) |
| `template/string/位集优化最长公共子序列.cpp` | [10](D:/OI/template/template/string/位集优化最长公共子序列.cpp:10) |
| `template/string/字符串哈希.cpp` | [18](D:/OI/template/template/string/字符串哈希.cpp:18)、[20](D:/OI/template/template/string/字符串哈希.cpp:20)、[33](D:/OI/template/template/string/字符串哈希.cpp:33)、[40](D:/OI/template/template/string/字符串哈希.cpp:40) |
| `template/string/二维哈希.cpp` | [21](D:/OI/template/template/string/二维哈希.cpp:21)、[36](D:/OI/template/template/string/二维哈希.cpp:36)、[47](D:/OI/template/template/string/二维哈希.cpp:47)、[51](D:/OI/template/template/string/二维哈希.cpp:51) |
| `template/string/字符串匹配.cpp` | [10](D:/OI/template/template/string/字符串匹配.cpp:10)、[21](D:/OI/template/template/string/字符串匹配.cpp:21) |
| `template/string/林登分解.cpp` | [11](D:/OI/template/template/string/林登分解.cpp:11) |
| `template/string/最长回文子串.cpp` | [15](D:/OI/template/template/string/最长回文子串.cpp:15)、[17](D:/OI/template/template/string/最长回文子串.cpp:17)、[47](D:/OI/template/template/string/最长回文子串.cpp:47)、[54](D:/OI/template/template/string/最长回文子串.cpp:54) |
| `template/string/最小表示法.cpp` | [8](D:/OI/template/template/string/最小表示法.cpp:8) |
| `template/string/回文自动机.cpp` | [25](D:/OI/template/template/string/回文自动机.cpp:25)、[33](D:/OI/template/template/string/回文自动机.cpp:33)、[54](D:/OI/template/template/string/回文自动机.cpp:54)、[59](D:/OI/template/template/string/回文自动机.cpp:59)、[61](D:/OI/template/template/string/回文自动机.cpp:61) |
| `template/string/极大周期子串.cpp` | [13](D:/OI/template/template/string/极大周期子串.cpp:13) |
| `template/string/后缀自动机.cpp` | [59](D:/OI/template/template/string/后缀自动机.cpp:59)、[64](D:/OI/template/template/string/后缀自动机.cpp:64)、[79](D:/OI/template/template/string/后缀自动机.cpp:79) |
| `template/string/后缀数组.cpp` | [16](D:/OI/template/template/string/后缀数组.cpp:16)、[18](D:/OI/template/template/string/后缀数组.cpp:18)、[81](D:/OI/template/template/string/后缀数组.cpp:81) |
| `template/string/字典树.cpp` | [21](D:/OI/template/template/string/字典树.cpp:21)、[36](D:/OI/template/template/string/字典树.cpp:36)、[46](D:/OI/template/template/string/字典树.cpp:46)、[51](D:/OI/template/template/string/字典树.cpp:51) |
| `template/string/扩展字符串匹配.cpp` | [11](D:/OI/template/template/string/扩展字符串匹配.cpp:11)、[23](D:/OI/template/template/string/扩展字符串匹配.cpp:23) |

### move 调用

3 个命中行，3 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/点双连通分量.cpp` | [60](D:/OI/template/template/graph/点双连通分量.cpp:60) |
| `template/math/最短线性递推.cpp` | [49](D:/OI/template/template/math/最短线性递推.cpp:49) |
| `template/math/扩展卢卡斯定理.cpp` | [39](D:/OI/template/template/math/扩展卢卡斯定理.cpp:39) |

### 显式默认构造函数

2 个命中行，2 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/string/最长回文子串.cpp` | [14](D:/OI/template/template/string/最长回文子串.cpp:14) |
| `template/string/后缀数组.cpp` | [15](D:/OI/template/template/string/后缀数组.cpp:15) |

### 构造函数初始化列表

4 个命中行，4 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/geometry/直线与线段.cpp` | [117](D:/OI/template/template/geometry/直线与线段.cpp:117) |
| `template/geometry/点与向量.cpp` | [21](D:/OI/template/template/geometry/点与向量.cpp:21) |
| `template/graph/差分约束.cpp` | [18](D:/OI/template/template/graph/差分约束.cpp:18) |
| `template/graph/上下界网络流.cpp` | [31](D:/OI/template/template/graph/上下界网络流.cpp:31) |

### 构造函数中调用 init 或 build

55 个命中行，50 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/并查集.cpp` | [15](D:/OI/template/template/ds/并查集.cpp:15) |
| `template/ds/树状数组.cpp` | [16](D:/OI/template/template/ds/树状数组.cpp:16)、[17](D:/OI/template/template/ds/树状数组.cpp:17) |
| `template/ds/无旋平衡树.cpp` | [29](D:/OI/template/template/ds/无旋平衡树.cpp:29)、[130](D:/OI/template/template/ds/无旋平衡树.cpp:130) |
| `template/ds/左偏树与可并堆.cpp` | [21](D:/OI/template/template/ds/左偏树与可并堆.cpp:21) |
| `template/ds/李超线段树.cpp` | [33](D:/OI/template/template/ds/李超线段树.cpp:33)、[34](D:/OI/template/template/ds/李超线段树.cpp:34) |
| `template/ds/动态开点线段树合并与分裂.cpp` | [18](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:18) |
| `template/ds/可持久化二进制字典树.cpp` | [18](D:/OI/template/template/ds/可持久化二进制字典树.cpp:18) |
| `template/ds/可撤销并查集.cpp` | [14](D:/OI/template/template/ds/可撤销并查集.cpp:14) |
| `template/ds/带权与异或并查集.cpp` | [19](D:/OI/template/template/ds/带权与异或并查集.cpp:19)、[55](D:/OI/template/template/ds/带权与异或并查集.cpp:55) |
| `template/ds/二进制字典树.cpp` | [16](D:/OI/template/template/ds/二进制字典树.cpp:16) |
| `template/graph/二分图匹配.cpp` | [15](D:/OI/template/template/graph/二分图匹配.cpp:15) |
| `template/graph/桥.cpp` | [16](D:/OI/template/template/graph/桥.cpp:16) |
| `template/graph/点分治.cpp` | [20](D:/OI/template/template/graph/点分治.cpp:20) |
| `template/graph/点分树.cpp` | [48](D:/OI/template/template/graph/点分树.cpp:48) |
| `template/graph/割点.cpp` | [16](D:/OI/template/template/graph/割点.cpp:16) |
| `template/graph/最大流.cpp` | [22](D:/OI/template/template/graph/最大流.cpp:22) |
| `template/graph/最小树形图.cpp` | [23](D:/OI/template/template/graph/最小树形图.cpp:23) |
| `template/graph/支配树.cpp` | [16](D:/OI/template/template/graph/支配树.cpp:16) |
| `template/graph/树上启发式合并.cpp` | [17](D:/OI/template/template/graph/树上启发式合并.cpp:17) |
| `template/graph/离线动态连通性.cpp` | [19](D:/OI/template/template/graph/离线动态连通性.cpp:19) |
| `template/graph/边双连通分量.cpp` | [27](D:/OI/template/template/graph/边双连通分量.cpp:27) |
| `template/graph/欧拉序最近公共祖先.cpp` | [16](D:/OI/template/template/graph/欧拉序最近公共祖先.cpp:16) |
| `template/graph/一般图匹配.cpp` | [16](D:/OI/template/template/graph/一般图匹配.cpp:16) |
| `template/graph/全局最小割.cpp` | [18](D:/OI/template/template/graph/全局最小割.cpp:18) |
| `template/graph/最小割树.cpp` | [24](D:/OI/template/template/graph/最小割树.cpp:24) |
| `template/graph/树链剖分.cpp` | [17](D:/OI/template/template/graph/树链剖分.cpp:17) |
| `template/graph/分层增广二分图匹配.cpp` | [16](D:/OI/template/template/graph/分层增广二分图匹配.cpp:16) |
| `template/graph/重赋权全源最短路.cpp` | [23](D:/OI/template/template/graph/重赋权全源最短路.cpp:23) |
| `template/graph/二分图最大权匹配.cpp` | [21](D:/OI/template/template/graph/二分图最大权匹配.cpp:21) |
| `template/graph/重构树.cpp` | [23](D:/OI/template/template/graph/重构树.cpp:23) |
| `template/graph/第几短游走.cpp` | [27](D:/OI/template/template/graph/第几短游走.cpp:27) |
| `template/graph/长链剖分与祖先查询.cpp` | [16](D:/OI/template/template/graph/长链剖分与祖先查询.cpp:16) |
| `template/graph/倍增最近公共祖先.cpp` | [15](D:/OI/template/template/graph/倍增最近公共祖先.cpp:15) |
| `template/graph/上下界网络流.cpp` | [87](D:/OI/template/template/graph/上下界网络流.cpp:87) |
| `template/graph/上下界最小费用流.cpp` | [26](D:/OI/template/template/graph/上下界最小费用流.cpp:26) |
| `template/graph/最大团与最大独立集.cpp` | [17](D:/OI/template/template/graph/最大团与最大独立集.cpp:17) |
| `template/graph/势能最短路费用流.cpp` | [24](D:/OI/template/template/graph/势能最短路费用流.cpp:24) |
| `template/graph/容量缩放费用流.cpp` | [23](D:/OI/template/template/graph/容量缩放费用流.cpp:23) |
| `template/graph/强连通分量.cpp` | [16](D:/OI/template/template/graph/强连通分量.cpp:16) |
| `template/graph/最小斯坦纳树.cpp` | [20](D:/OI/template/template/graph/最小斯坦纳树.cpp:20) |
| `template/graph/二元可满足性.cpp` | [17](D:/OI/template/template/graph/二元可满足性.cpp:17) |
| `template/graph/点双连通分量.cpp` | [23](D:/OI/template/template/graph/点双连通分量.cpp:23) |
| `template/graph/虚树.cpp` | [18](D:/OI/template/template/graph/虚树.cpp:18) |
| `template/graph/一般图最大权匹配.cpp` | [27](D:/OI/template/template/graph/一般图最大权匹配.cpp:27) |
| `template/others/舞蹈链精确覆盖.cpp` | [20](D:/OI/template/template/others/舞蹈链精确覆盖.cpp:20) |
| `template/others/树上莫队.cpp` | [18](D:/OI/template/template/others/树上莫队.cpp:18) |
| `template/string/字符串哈希.cpp` | [18](D:/OI/template/template/string/字符串哈希.cpp:18) |
| `template/string/最长回文子串.cpp` | [15](D:/OI/template/template/string/最长回文子串.cpp:15) |
| `template/string/回文自动机.cpp` | [24](D:/OI/template/template/string/回文自动机.cpp:24)、[25](D:/OI/template/template/string/回文自动机.cpp:25) |
| `template/string/后缀数组.cpp` | [16](D:/OI/template/template/string/后缀数组.cpp:16) |

### 内部结构体声明

50 个命中行，42 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/无旋平衡树.cpp` | [20](D:/OI/template/template/ds/无旋平衡树.cpp:20)、[120](D:/OI/template/template/ds/无旋平衡树.cpp:120) |
| `template/ds/二维最近邻.cpp` | [19](D:/OI/template/template/ds/二维最近邻.cpp:19) |
| `template/ds/动态树.cpp` | [14](D:/OI/template/template/ds/动态树.cpp:14) |
| `template/ds/左偏树与可并堆.cpp` | [15](D:/OI/template/template/ds/左偏树与可并堆.cpp:15) |
| `template/ds/李超线段树.cpp` | [18](D:/OI/template/template/ds/李超线段树.cpp:18) |
| `template/ds/动态开点线段树合并与分裂.cpp` | [14](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:14) |
| `template/ds/可持久化数组.cpp` | [15](D:/OI/template/template/ds/可持久化数组.cpp:15) |
| `template/ds/主席树.cpp` | [14](D:/OI/template/template/ds/主席树.cpp:14) |
| `template/ds/可持久化二进制字典树.cpp` | [15](D:/OI/template/template/ds/可持久化二进制字典树.cpp:15) |
| `template/ds/吉司机线段树.cpp` | [17](D:/OI/template/template/ds/吉司机线段树.cpp:17) |
| `template/ds/二进制字典树.cpp` | [13](D:/OI/template/template/ds/二进制字典树.cpp:13) |
| `template/geometry/平面最近点对.cpp` | [12](D:/OI/template/template/geometry/平面最近点对.cpp:12) |
| `template/geometry/三维凸包.cpp` | [32](D:/OI/template/template/geometry/三维凸包.cpp:32) |
| `template/geometry/矩形面积并与周长并.cpp` | [13](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:13)、[14](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:14) |
| `template/graph/点分树.cpp` | [16](D:/OI/template/template/graph/点分树.cpp:16)、[38](D:/OI/template/template/graph/点分树.cpp:38) |
| `template/graph/最大流.cpp` | [13](D:/OI/template/template/graph/最大流.cpp:13) |
| `template/graph/最小树形图.cpp` | [15](D:/OI/template/template/graph/最小树形图.cpp:15) |
| `template/graph/离线动态连通性.cpp` | [12](D:/OI/template/template/graph/离线动态连通性.cpp:12) |
| `template/graph/边双连通分量.cpp` | [14](D:/OI/template/template/graph/边双连通分量.cpp:14) |
| `template/graph/最小割树.cpp` | [15](D:/OI/template/template/graph/最小割树.cpp:15) |
| `template/graph/重赋权全源最短路.cpp` | [18](D:/OI/template/template/graph/重赋权全源最短路.cpp:18) |
| `template/graph/重构树.cpp` | [16](D:/OI/template/template/graph/重构树.cpp:16) |
| `template/graph/第几短游走.cpp` | [17](D:/OI/template/template/graph/第几短游走.cpp:17)、[18](D:/OI/template/template/graph/第几短游走.cpp:18) |
| `template/graph/上下界网络流.cpp` | [17](D:/OI/template/template/graph/上下界网络流.cpp:17)、[22](D:/OI/template/template/graph/上下界网络流.cpp:22)、[23](D:/OI/template/template/graph/上下界网络流.cpp:23) |
| `template/graph/上下界最小费用流.cpp` | [21](D:/OI/template/template/graph/上下界最小费用流.cpp:21) |
| `template/graph/势能最短路费用流.cpp` | [17](D:/OI/template/template/graph/势能最短路费用流.cpp:17) |
| `template/graph/容量缩放费用流.cpp` | [15](D:/OI/template/template/graph/容量缩放费用流.cpp:15)、[42](D:/OI/template/template/graph/容量缩放费用流.cpp:42) |
| `template/graph/点双连通分量.cpp` | [12](D:/OI/template/template/graph/点双连通分量.cpp:12) |
| `template/graph/一般图最大权匹配.cpp` | [18](D:/OI/template/template/graph/一般图最大权匹配.cpp:18) |
| `template/math/扩展卢卡斯定理.cpp` | [17](D:/OI/template/template/math/扩展卢卡斯定理.cpp:17) |
| `template/math/单纯形与线性规划.cpp` | [17](D:/OI/template/template/math/单纯形与线性规划.cpp:17) |
| `template/others/分治三维偏序.cpp` | [14](D:/OI/template/template/others/分治三维偏序.cpp:14) |
| `template/others/单调凸包与斜率优化.cpp` | [16](D:/OI/template/template/others/单调凸包与斜率优化.cpp:16) |
| `template/others/舞蹈链精确覆盖.cpp` | [15](D:/OI/template/template/others/舞蹈链精确覆盖.cpp:15) |
| `template/others/带修改莫队.cpp` | [13](D:/OI/template/template/others/带修改莫队.cpp:13)、[14](D:/OI/template/template/others/带修改莫队.cpp:14) |
| `template/others/树上莫队.cpp` | [12](D:/OI/template/template/others/树上莫队.cpp:12) |
| `template/others/整体二分与动态区间第几小.cpp` | [16](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:16) |
| `template/string/多模式匹配自动机.cpp` | [13](D:/OI/template/template/string/多模式匹配自动机.cpp:13) |
| `template/string/二维哈希.cpp` | [15](D:/OI/template/template/string/二维哈希.cpp:15) |
| `template/string/回文自动机.cpp` | [14](D:/OI/template/template/string/回文自动机.cpp:14) |
| `template/string/后缀自动机.cpp` | [14](D:/OI/template/template/string/后缀自动机.cpp:14) |
| `template/string/字典树.cpp` | [11](D:/OI/template/template/string/字典树.cpp:11) |

### tuple、array、bitset、complex、tie、numeric_limits、size_t

41 个命中行，26 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/二维最近邻.cpp` | [31](D:/OI/template/template/ds/二维最近邻.cpp:31)、[32](D:/OI/template/template/ds/二维最近邻.cpp:32) |
| `template/ds/左偏树与可并堆.cpp` | [33](D:/OI/template/template/ds/左偏树与可并堆.cpp:33) |
| `template/ds/李超线段树.cpp` | [111](D:/OI/template/template/ds/李超线段树.cpp:111) |
| `template/ds/可持久化二进制字典树.cpp` | [15](D:/OI/template/template/ds/可持久化二进制字典树.cpp:15) |
| `template/ds/二进制字典树.cpp` | [13](D:/OI/template/template/ds/二进制字典树.cpp:13)、[34](D:/OI/template/template/ds/二进制字典树.cpp:34) |
| `template/geometry/平面最近点对.cpp` | [50](D:/OI/template/template/geometry/平面最近点对.cpp:50) |
| `template/geometry/最小面积包围矩形.cpp` | [30](D:/OI/template/template/geometry/最小面积包围矩形.cpp:30) |
| `template/geometry/矩形面积并与周长并.cpp` | [39](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:39) |
| `template/graph/差分约束.cpp` | [16](D:/OI/template/template/graph/差分约束.cpp:16) |
| `template/graph/函数图分解与跳跃.cpp` | [19](D:/OI/template/template/graph/函数图分解与跳跃.cpp:19) |
| `template/graph/树的直径.cpp` | [29](D:/OI/template/template/graph/树的直径.cpp:29) |
| `template/math/线性递推快速求项.cpp` | [22](D:/OI/template/template/math/线性递推快速求项.cpp:22)、[23](D:/OI/template/template/math/线性递推快速求项.cpp:23)、[31](D:/OI/template/template/math/线性递推快速求项.cpp:31) |
| `template/math/离散对数.cpp` | [28](D:/OI/template/template/math/离散对数.cpp:28) |
| `template/math/行列式与矩阵树定理.cpp` | [48](D:/OI/template/template/math/行列式与矩阵树定理.cpp:48) |
| `template/math/扩展卢卡斯定理.cpp` | [34](D:/OI/template/template/math/扩展卢卡斯定理.cpp:34) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [12](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:12)、[22](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:22)、[33](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:33)、[35](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:35)、[37](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:37)、[50](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:50) |
| `template/math/高斯消元.cpp` | [79](D:/OI/template/template/math/高斯消元.cpp:79)、[80](D:/OI/template/template/math/高斯消元.cpp:80) |
| `template/math/积性函数求和.cpp` | [22](D:/OI/template/template/math/积性函数求和.cpp:22)、[53](D:/OI/template/template/math/积性函数求和.cpp:53)、[56](D:/OI/template/template/math/积性函数求和.cpp:56)、[72](D:/OI/template/template/math/积性函数求和.cpp:72) |
| `template/math/单纯形与线性规划.cpp` | [86](D:/OI/template/template/math/单纯形与线性规划.cpp:86) |
| `template/math/异或线性基.cpp` | [11](D:/OI/template/template/math/异或线性基.cpp:11) |
| `template/others/分治三维偏序.cpp` | [13](D:/OI/template/template/others/分治三维偏序.cpp:13)、[22](D:/OI/template/template/others/分治三维偏序.cpp:22) |
| `template/string/多模式匹配自动机.cpp` | [14](D:/OI/template/template/string/多模式匹配自动机.cpp:14) |
| `template/string/回文自动机.cpp` | [15](D:/OI/template/template/string/回文自动机.cpp:15) |
| `template/string/极大周期子串.cpp` | [13](D:/OI/template/template/string/极大周期子串.cpp:13)、[17](D:/OI/template/template/string/极大周期子串.cpp:17) |
| `template/string/后缀自动机.cpp` | [15](D:/OI/template/template/string/后缀自动机.cpp:15) |
| `template/string/字典树.cpp` | [12](D:/OI/template/template/string/字典树.cpp:12) |

### GNU 宽整数及扩展声明

12 个命中行，12 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/李超线段树.cpp` | [6](D:/OI/template/template/ds/李超线段树.cpp:6) |
| `template/graph/二分图最大权匹配.cpp` | [36](D:/OI/template/template/graph/二分图最大权匹配.cpp:36) |
| `template/graph/上下界最小费用流.cpp` | [20](D:/OI/template/template/graph/上下界最小费用流.cpp:20) |
| `template/graph/势能最短路费用流.cpp` | [38](D:/OI/template/template/graph/势能最短路费用流.cpp:38) |
| `template/math/扩展欧几里得与中国剩余定理.cpp` | [6](D:/OI/template/template/math/扩展欧几里得与中国剩余定理.cpp:6) |
| `template/math/杜教筛.cpp` | [7](D:/OI/template/template/math/杜教筛.cpp:7) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [8](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:8) |
| `template/math/类欧几里得.cpp` | [4](D:/OI/template/template/math/类欧几里得.cpp:4) |
| `template/math/积性函数求和.cpp` | [6](D:/OI/template/template/math/积性函数求和.cpp:6) |
| `template/math/快速幂与逆元.cpp` | [6](D:/OI/template/template/math/快速幂与逆元.cpp:6) |
| `template/math/素性测试与整数分解.cpp` | [6](D:/OI/template/template/math/素性测试与整数分解.cpp:6) |
| `template/others/单调凸包与斜率优化.cpp` | [6](D:/OI/template/template/others/单调凸包与斜率优化.cpp:6) |

### 断言

27 个命中行，10 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/无旋平衡树.cpp` | [169](D:/OI/template/template/ds/无旋平衡树.cpp:169)、[177](D:/OI/template/template/ds/无旋平衡树.cpp:177)、[184](D:/OI/template/template/ds/无旋平衡树.cpp:184)、[191](D:/OI/template/template/ds/无旋平衡树.cpp:191) |
| `template/graph/上下界最小费用流.cpp` | [33](D:/OI/template/template/graph/上下界最小费用流.cpp:33)、[40](D:/OI/template/template/graph/上下界最小费用流.cpp:40)、[55](D:/OI/template/template/graph/上下界最小费用流.cpp:55) |
| `template/graph/一般图最大权匹配.cpp` | [45](D:/OI/template/template/graph/一般图最大权匹配.cpp:45) |
| `template/math/线性递推快速求项.cpp` | [15](D:/OI/template/template/math/线性递推快速求项.cpp:15) |
| `template/math/行列式与矩阵树定理.cpp` | [19](D:/OI/template/template/math/行列式与矩阵树定理.cpp:19)、[22](D:/OI/template/template/math/行列式与矩阵树定理.cpp:22)、[50](D:/OI/template/template/math/行列式与矩阵树定理.cpp:50)、[51](D:/OI/template/template/math/行列式与矩阵树定理.cpp:51)、[59](D:/OI/template/template/math/行列式与矩阵树定理.cpp:59) |
| `template/math/杜教筛.cpp` | [25](D:/OI/template/template/math/杜教筛.cpp:25) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [112](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:112)、[114](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:114) |
| `template/math/多项式基本运算.cpp` | [62](D:/OI/template/template/math/多项式基本运算.cpp:62)、[73](D:/OI/template/template/math/多项式基本运算.cpp:73)、[88](D:/OI/template/template/math/多项式基本运算.cpp:88)、[102](D:/OI/template/template/math/多项式基本运算.cpp:102)、[120](D:/OI/template/template/math/多项式基本运算.cpp:120)、[161](D:/OI/template/template/math/多项式基本运算.cpp:161)、[186](D:/OI/template/template/math/多项式基本运算.cpp:186)、[189](D:/OI/template/template/math/多项式基本运算.cpp:189) |
| `template/math/快速沃尔什变换.cpp` | [14](D:/OI/template/template/math/快速沃尔什变换.cpp:14) |
| `template/math/多项式多点求值与快速插值.cpp` | [74](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:74) |

### 随机数库及时间种子

7 个命中行，4 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/无旋平衡树.cpp` | [27](D:/OI/template/template/ds/无旋平衡树.cpp:27)、[128](D:/OI/template/template/ds/无旋平衡树.cpp:128) |
| `template/geometry/最小圆覆盖.cpp` | [12](D:/OI/template/template/geometry/最小圆覆盖.cpp:12) |
| `template/math/二次剩余.cpp` | [24](D:/OI/template/template/math/二次剩余.cpp:24)、[25](D:/OI/template/template/math/二次剩余.cpp:25) |
| `template/math/素性测试与整数分解.cpp` | [45](D:/OI/template/template/math/素性测试与整数分解.cpp:45)、[72](D:/OI/template/template/math/素性测试与整数分解.cpp:72) |

### 全部 auto 命中行

169 个命中行，71 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/二维树状数组.cpp` | [18](D:/OI/template/template/ds/二维树状数组.cpp:18)、[22](D:/OI/template/template/ds/二维树状数组.cpp:22)、[43](D:/OI/template/template/ds/二维树状数组.cpp:43)、[47](D:/OI/template/template/ds/二维树状数组.cpp:47) |
| `template/ds/区间集合与最小未出现数.cpp` | [20](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:20)、[31](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:31)、[34](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:34)、[42](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:42)、[47](D:/OI/template/template/ds/区间集合与最小未出现数.cpp:47) |
| `template/ds/李超线段树.cpp` | [66](D:/OI/template/template/ds/李超线段树.cpp:66)、[124](D:/OI/template/template/ds/李超线段树.cpp:124) |
| `template/ds/动态开点线段树合并与分裂.cpp` | [72](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:72)、[73](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:73)、[81](D:/OI/template/template/ds/动态开点线段树合并与分裂.cpp:81) |
| `template/ds/可撤销并查集.cpp` | [42](D:/OI/template/template/ds/可撤销并查集.cpp:42) |
| `template/ds/吉司机线段树.cpp` | [26](D:/OI/template/template/ds/吉司机线段树.cpp:26)、[27](D:/OI/template/template/ds/吉司机线段树.cpp:27)、[42](D:/OI/template/template/ds/吉司机线段树.cpp:42)、[51](D:/OI/template/template/ds/吉司机线段树.cpp:51)、[60](D:/OI/template/template/ds/吉司机线段树.cpp:60) |
| `template/geometry/圆面积并与周长并.cpp` | [54](D:/OI/template/template/geometry/圆面积并与周长并.cpp:54)、[62](D:/OI/template/template/geometry/圆面积并与周长并.cpp:62) |
| `template/geometry/凸包与直径与闵可夫斯基和.cpp` | [98](D:/OI/template/template/geometry/凸包与直径与闵可夫斯基和.cpp:98) |
| `template/geometry/三维凸包.cpp` | [82](D:/OI/template/template/geometry/三维凸包.cpp:82)、[84](D:/OI/template/template/geometry/三维凸包.cpp:84)、[90](D:/OI/template/template/geometry/三维凸包.cpp:90) |
| `template/geometry/半平面交.cpp` | [17](D:/OI/template/template/geometry/半平面交.cpp:17)、[28](D:/OI/template/template/geometry/半平面交.cpp:28)、[32](D:/OI/template/template/geometry/半平面交.cpp:32) |
| `template/geometry/最小圆覆盖.cpp` | [22](D:/OI/template/template/geometry/最小圆覆盖.cpp:22) |
| `template/geometry/最小面积包围矩形.cpp` | [33](D:/OI/template/template/geometry/最小面积包围矩形.cpp:33)、[34](D:/OI/template/template/geometry/最小面积包围矩形.cpp:34) |
| `template/geometry/矩形面积并与周长并.cpp` | [25](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:25)、[41](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:41)、[56](D:/OI/template/template/geometry/矩形面积并与周长并.cpp:56) |
| `template/graph/桥.cpp` | [30](D:/OI/template/template/graph/桥.cpp:30) |
| `template/graph/边双缩点树.cpp` | [15](D:/OI/template/template/graph/边双缩点树.cpp:15) |
| `template/graph/点分治.cpp` | [32](D:/OI/template/template/graph/点分治.cpp:32)、[38](D:/OI/template/template/graph/点分治.cpp:38)、[47](D:/OI/template/template/graph/点分治.cpp:47)、[67](D:/OI/template/template/graph/点分治.cpp:67)、[74](D:/OI/template/template/graph/点分治.cpp:74) |
| `template/graph/点分树.cpp` | [60](D:/OI/template/template/graph/点分树.cpp:60)、[66](D:/OI/template/template/graph/点分树.cpp:66)、[75](D:/OI/template/template/graph/点分树.cpp:75)、[83](D:/OI/template/template/graph/点分树.cpp:83)、[90](D:/OI/template/template/graph/点分树.cpp:90)、[100](D:/OI/template/template/graph/点分树.cpp:100)、[108](D:/OI/template/template/graph/点分树.cpp:108) |
| `template/graph/差分约束.cpp` | [27](D:/OI/template/template/graph/差分约束.cpp:27) |
| `template/graph/堆优化最短路.cpp` | [22](D:/OI/template/template/graph/堆优化最短路.cpp:22)、[24](D:/OI/template/template/graph/堆优化最短路.cpp:24) |
| `template/graph/最大流.cpp` | [44](D:/OI/template/template/graph/最大流.cpp:44)、[55](D:/OI/template/template/graph/最大流.cpp:55) |
| `template/graph/有向图欧拉路.cpp` | [16](D:/OI/template/template/graph/有向图欧拉路.cpp:16)、[31](D:/OI/template/template/graph/有向图欧拉路.cpp:31) |
| `template/graph/最小树形图.cpp` | [36](D:/OI/template/template/graph/最小树形图.cpp:36)、[60](D:/OI/template/template/graph/最小树形图.cpp:60) |
| `template/graph/离线动态连通性.cpp` | [38](D:/OI/template/template/graph/离线动态连通性.cpp:38)、[53](D:/OI/template/template/graph/离线动态连通性.cpp:53)、[68](D:/OI/template/template/graph/离线动态连通性.cpp:68)、[71](D:/OI/template/template/graph/离线动态连通性.cpp:71)、[76](D:/OI/template/template/graph/离线动态连通性.cpp:76) |
| `template/graph/边双连通分量.cpp` | [52](D:/OI/template/template/graph/边双连通分量.cpp:52)、[67](D:/OI/template/template/graph/边双连通分量.cpp:67) |
| `template/graph/函数图分解与跳跃.cpp` | [51](D:/OI/template/template/graph/函数图分解与跳跃.cpp:51) |
| `template/graph/全局最小割.cpp` | [33](D:/OI/template/template/graph/全局最小割.cpp:33) |
| `template/graph/最小割树.cpp` | [34](D:/OI/template/template/graph/最小割树.cpp:34)、[46](D:/OI/template/template/graph/最小割树.cpp:46) |
| `template/graph/重赋权全源最短路.cpp` | [41](D:/OI/template/template/graph/重赋权全源最短路.cpp:41)、[59](D:/OI/template/template/graph/重赋权全源最短路.cpp:59)、[61](D:/OI/template/template/graph/重赋权全源最短路.cpp:61) |
| `template/graph/最小生成树.cpp` | [32](D:/OI/template/template/graph/最小生成树.cpp:32) |
| `template/graph/重构树.cpp` | [38](D:/OI/template/template/graph/重构树.cpp:38)、[42](D:/OI/template/template/graph/重构树.cpp:42) |
| `template/graph/第几短游走.cpp` | [59](D:/OI/template/template/graph/第几短游走.cpp:59)、[63](D:/OI/template/template/graph/第几短游走.cpp:63)、[75](D:/OI/template/template/graph/第几短游走.cpp:75)、[88](D:/OI/template/template/graph/第几短游走.cpp:88) |
| `template/graph/上下界网络流.cpp` | [41](D:/OI/template/template/graph/上下界网络流.cpp:41)、[51](D:/OI/template/template/graph/上下界网络流.cpp:51)、[62](D:/OI/template/template/graph/上下界网络流.cpp:62)、[104](D:/OI/template/template/graph/上下界网络流.cpp:104)、[127](D:/OI/template/template/graph/上下界网络流.cpp:127)、[131](D:/OI/template/template/graph/上下界网络流.cpp:131) |
| `template/graph/上下界最小费用流.cpp` | [51](D:/OI/template/template/graph/上下界最小费用流.cpp:51)、[62](D:/OI/template/template/graph/上下界最小费用流.cpp:62)、[79](D:/OI/template/template/graph/上下界最小费用流.cpp:79) |
| `template/graph/势能最短路费用流.cpp` | [53](D:/OI/template/template/graph/势能最短路费用流.cpp:53)、[76](D:/OI/template/template/graph/势能最短路费用流.cpp:76)、[80](D:/OI/template/template/graph/势能最短路费用流.cpp:80)、[101](D:/OI/template/template/graph/势能最短路费用流.cpp:101) |
| `template/graph/换根动态规划与距离和.cpp` | [20](D:/OI/template/template/graph/换根动态规划与距离和.cpp:20)、[21](D:/OI/template/template/graph/换根动态规划与距离和.cpp:21)、[27](D:/OI/template/template/graph/换根动态规划与距离和.cpp:27)、[28](D:/OI/template/template/graph/换根动态规划与距离和.cpp:28) |
| `template/graph/容量缩放费用流.cpp` | [50](D:/OI/template/template/graph/容量缩放费用流.cpp:50)、[59](D:/OI/template/template/graph/容量缩放费用流.cpp:59)、[73](D:/OI/template/template/graph/容量缩放费用流.cpp:73)、[78](D:/OI/template/template/graph/容量缩放费用流.cpp:78)、[88](D:/OI/template/template/graph/容量缩放费用流.cpp:88)、[104](D:/OI/template/template/graph/容量缩放费用流.cpp:104) |
| `template/graph/最小斯坦纳树.cpp` | [49](D:/OI/template/template/graph/最小斯坦纳树.cpp:49)、[51](D:/OI/template/template/graph/最小斯坦纳树.cpp:51) |
| `template/graph/树的直径.cpp` | [17](D:/OI/template/template/graph/树的直径.cpp:17)、[20](D:/OI/template/template/graph/树的直径.cpp:20)、[32](D:/OI/template/template/graph/树的直径.cpp:32) |
| `template/graph/无向图欧拉路.cpp` | [16](D:/OI/template/template/graph/无向图欧拉路.cpp:16)、[32](D:/OI/template/template/graph/无向图欧拉路.cpp:32)、[34](D:/OI/template/template/graph/无向图欧拉路.cpp:34) |
| `template/graph/点双连通分量.cpp` | [65](D:/OI/template/template/graph/点双连通分量.cpp:65) |
| `template/graph/虚树.cpp` | [60](D:/OI/template/template/graph/虚树.cpp:60) |
| `template/graph/一般图最大权匹配.cpp` | [119](D:/OI/template/template/graph/一般图最大权匹配.cpp:119)、[223](D:/OI/template/template/graph/一般图最大权匹配.cpp:223) |
| `template/graph/零一最短路.cpp` | [17](D:/OI/template/template/graph/零一最短路.cpp:17) |
| `template/math/自适应辛普森积分.cpp` | [16](D:/OI/template/template/math/自适应辛普森积分.cpp:16)、[21](D:/OI/template/template/math/自适应辛普森积分.cpp:21) |
| `template/math/线性递推快速求项.cpp` | [20](D:/OI/template/template/math/线性递推快速求项.cpp:20)、[35](D:/OI/template/template/math/线性递推快速求项.cpp:35) |
| `template/math/离散对数.cpp` | [12](D:/OI/template/template/math/离散对数.cpp:12)、[36](D:/OI/template/template/math/离散对数.cpp:36) |
| `template/math/组合数.cpp` | [22](D:/OI/template/template/math/组合数.cpp:22) |
| `template/math/行列式与矩阵树定理.cpp` | [21](D:/OI/template/template/math/行列式与矩阵树定理.cpp:21)、[25](D:/OI/template/template/math/行列式与矩阵树定理.cpp:25)、[53](D:/OI/template/template/math/行列式与矩阵树定理.cpp:53)、[58](D:/OI/template/template/math/行列式与矩阵树定理.cpp:58) |
| `template/math/杜教筛.cpp` | [53](D:/OI/template/template/math/杜教筛.cpp:53)、[65](D:/OI/template/template/math/杜教筛.cpp:65) |
| `template/math/扩展卢卡斯定理.cpp` | [70](D:/OI/template/template/math/扩展卢卡斯定理.cpp:70) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [43](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:43) |
| `template/math/高斯消元.cpp` | [50](D:/OI/template/template/math/高斯消元.cpp:50)、[52](D:/OI/template/template/math/高斯消元.cpp:52) |
| `template/math/矩阵求逆.cpp` | [20](D:/OI/template/template/math/矩阵求逆.cpp:20) |
| `template/math/积性函数求和.cpp` | [84](D:/OI/template/template/math/积性函数求和.cpp:84) |
| `template/math/二次剩余.cpp` | [16](D:/OI/template/template/math/二次剩余.cpp:16)、[32](D:/OI/template/template/math/二次剩余.cpp:32) |
| `template/math/多项式多点求值与快速插值.cpp` | [58](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:58)、[59](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:59)、[72](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:72)、[77](D:/OI/template/template/math/多项式多点求值与快速插值.cpp:77) |
| `template/math/快速数论变换与多项式乘法.cpp` | [45](D:/OI/template/template/math/快速数论变换与多项式乘法.cpp:45) |
| `template/math/素性测试与整数分解.cpp` | [49](D:/OI/template/template/math/素性测试与整数分解.cpp:49)、[73](D:/OI/template/template/math/素性测试与整数分解.cpp:73) |
| `template/math/原根.cpp` | [18](D:/OI/template/template/math/原根.cpp:18) |
| `template/math/子集卷积.cpp` | [24](D:/OI/template/template/math/子集卷积.cpp:24) |
| `template/math/异或线性基.cpp` | [36](D:/OI/template/template/math/异或线性基.cpp:36) |
| `template/others/分治三维偏序.cpp` | [29](D:/OI/template/template/others/分治三维偏序.cpp:29)、[32](D:/OI/template/template/others/分治三维偏序.cpp:32)、[35](D:/OI/template/template/others/分治三维偏序.cpp:35)、[41](D:/OI/template/template/others/分治三维偏序.cpp:41)、[60](D:/OI/template/template/others/分治三维偏序.cpp:60) |
| `template/others/分治动态规划与单调队列多重背包.cpp` | [17](D:/OI/template/template/others/分治动态规划与单调队列多重背包.cpp:17) |
| `template/others/莫队.cpp` | [22](D:/OI/template/template/others/莫队.cpp:22) |
| `template/others/带修改莫队.cpp` | [34](D:/OI/template/template/others/带修改莫队.cpp:34)、[44](D:/OI/template/template/others/带修改莫队.cpp:44)、[45](D:/OI/template/template/others/带修改莫队.cpp:45)、[50](D:/OI/template/template/others/带修改莫队.cpp:50) |
| `template/others/树上莫队.cpp` | [55](D:/OI/template/template/others/树上莫队.cpp:55)、[62](D:/OI/template/template/others/树上莫队.cpp:62)、[64](D:/OI/template/template/others/树上莫队.cpp:64) |
| `template/others/整体二分与动态区间第几小.cpp` | [65](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:65)、[78](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:78)、[94](D:/OI/template/template/others/整体二分与动态区间第几小.cpp:94) |
| `template/others/博弈状态函数.cpp` | [16](D:/OI/template/template/others/博弈状态函数.cpp:16) |
| `template/string/多模式匹配自动机.cpp` | [47](D:/OI/template/template/string/多模式匹配自动机.cpp:47) |
| `template/string/位集优化最长公共子序列.cpp` | [27](D:/OI/template/template/string/位集优化最长公共子序列.cpp:27) |
| `template/string/后缀自动机.cpp` | [66](D:/OI/template/template/string/后缀自动机.cpp:66)、[68](D:/OI/template/template/string/后缀自动机.cpp:68)、[80](D:/OI/template/template/string/后缀自动机.cpp:80) |

### bits/stdc++.h 以外的头文件

7 个命中行，7 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/无旋平衡树.cpp` | [2](D:/OI/template/template/ds/无旋平衡树.cpp:2) |
| `template/graph/上下界最小费用流.cpp` | [2](D:/OI/template/template/graph/上下界最小费用流.cpp:2) |
| `template/graph/一般图最大权匹配.cpp` | [2](D:/OI/template/template/graph/一般图最大权匹配.cpp:2) |
| `template/math/行列式与矩阵树定理.cpp` | [2](D:/OI/template/template/math/行列式与矩阵树定理.cpp:2) |
| `template/math/杜教筛.cpp` | [2](D:/OI/template/template/math/杜教筛.cpp:2) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [2](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:2) |
| `template/math/多项式基本运算.cpp` | [2](D:/OI/template/template/math/多项式基本运算.cpp:2) |

### 其他类型别名声明

8 个命中行，8 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/堆优化最短路.cpp` | [9](D:/OI/template/template/graph/堆优化最短路.cpp:9) |
| `template/graph/二分图最大权匹配.cpp` | [36](D:/OI/template/template/graph/二分图最大权匹配.cpp:36) |
| `template/graph/上下界最小费用流.cpp` | [20](D:/OI/template/template/graph/上下界最小费用流.cpp:20) |
| `template/graph/势能最短路费用流.cpp` | [38](D:/OI/template/template/graph/势能最短路费用流.cpp:38) |
| `template/graph/换根动态规划与距离和.cpp` | [14](D:/OI/template/template/graph/换根动态规划与距离和.cpp:14) |
| `template/graph/树的直径.cpp` | [13](D:/OI/template/template/graph/树的直径.cpp:13) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [12](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:12) |
| `template/math/矩阵快速幂.cpp` | [14](D:/OI/template/template/math/矩阵快速幂.cpp:14) |

### 数字分隔符

2 个命中行，1 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/容量缩放费用流.cpp` | [38](D:/OI/template/template/graph/容量缩放费用流.cpp:38)、[39](D:/OI/template/template/graph/容量缩放费用流.cpp:39) |

### 编译器位运算内建

5 个命中行，4 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/graph/长链剖分与祖先查询.cpp` | [58](D:/OI/template/template/graph/长链剖分与祖先查询.cpp:58) |
| `template/math/子集卷积.cpp` | [14](D:/OI/template/template/math/子集卷积.cpp:14)、[18](D:/OI/template/template/math/子集卷积.cpp:18) |
| `template/others/稀疏表.cpp` | [15](D:/OI/template/template/others/稀疏表.cpp:15) |
| `template/string/位集优化最长公共子序列.cpp` | [27](D:/OI/template/template/string/位集优化最长公共子序列.cpp:27) |

### 显式忽略未使用变量

1 个命中行，1 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/二维树状数组.cpp` | [18](D:/OI/template/template/ds/二维树状数组.cpp:18) |

### 默认 bool 开关或 mode 参数

11 个命中行，9 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/ds/二维树状数组.cpp` | [42](D:/OI/template/template/ds/二维树状数组.cpp:42) |
| `template/graph/重构树.cpp` | [34](D:/OI/template/template/graph/重构树.cpp:34) |
| `template/graph/上下界网络流.cpp` | [96](D:/OI/template/template/graph/上下界网络流.cpp:96) |
| `template/math/约数与倍数变换与卷积.cpp` | [15](D:/OI/template/template/math/约数与倍数变换与卷积.cpp:15)、[26](D:/OI/template/template/math/约数与倍数变换与卷积.cpp:26) |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | [22](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:22)、[69](D:/OI/template/template/math/快速傅里叶变换与任意模数卷积.cpp:69) |
| `template/math/多项式基本运算.cpp` | [30](D:/OI/template/template/math/多项式基本运算.cpp:30) |
| `template/math/快速沃尔什变换.cpp` | [53](D:/OI/template/template/math/快速沃尔什变换.cpp:53) |
| `template/math/快速数论变换与多项式乘法.cpp` | [23](D:/OI/template/template/math/快速数论变换与多项式乘法.cpp:23) |
| `template/others/最长上升子序列与方案.cpp` | [11](D:/OI/template/template/others/最长上升子序列与方案.cpp:11) |

### 运算符重载

11 个命中行，2 个文件。

| 文件 | 行号 |
| --- | --- |
| `template/geometry/三维凸包.cpp` | [19](D:/OI/template/template/geometry/三维凸包.cpp:19)、[20](D:/OI/template/template/geometry/三维凸包.cpp:20)、[21](D:/OI/template/template/geometry/三维凸包.cpp:21)、[22](D:/OI/template/template/geometry/三维凸包.cpp:22) |
| `template/geometry/点与向量.cpp` | [23](D:/OI/template/template/geometry/点与向量.cpp:23)、[24](D:/OI/template/template/geometry/点与向量.cpp:24)、[25](D:/OI/template/template/geometry/点与向量.cpp:25)、[26](D:/OI/template/template/geometry/点与向量.cpp:26)、[28](D:/OI/template/template/geometry/点与向量.cpp:28)、[33](D:/OI/template/template/geometry/点与向量.cpp:33)、[54](D:/OI/template/template/geometry/点与向量.cpp:54) |

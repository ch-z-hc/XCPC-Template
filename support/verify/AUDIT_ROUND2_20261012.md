# 第二轮全量审计

日期：2026-10-12。范围：`template/*/*.cpp` 的全部 145 个 C++17 模板。

后续版本说明：用户随后要求采用竞赛写法，差分约束已改为纯 `ll`，移除模板中的异常、`static_cast`、`optional` 和类型萃取。本文的接口、源码哈希、打印册哈希和全量结果保留为第二轮结束时的历史快照；当前调用域与新一轮复验见 [验证 README](README.md)，不将下表旧接口或旧数值域作为当前版本的保证。

本轮完成全部 145 项逐文件复核，发现并修复 2 处模板问题、1 处独立测试真值问题，补明 3 个模板的调用前提。最终 45 个标准验证程序、42 个新种子随机入口、20 题共 409 组官方生成数据全部通过；全量 Linux 流程包含的独立整数与对拍工具检查也完整退出 0。在声明输入域和本轮测试范围内，未再发现反例。

## 范围与方法

本轮重新阅读全部模板实现及接口前提，重点检查空输入、状态复用、整数中间量、浮点判停、图与几何退化情形。重新运行全量 Linux 检查、两组新种子的全部随机入口和固定版本的官方生成数据，不把上一轮的通过结论直接当作本轮结果。

保留 [第一轮审计](AUDIT_PER_TEMPLATE_20261012.md) 的历史结论和失败记录。本轮开始时工作区已有未提交修改；不回退、不提交、不清理这些修改。沿用用户此前“有问题的帮我改好”的要求，只修复已复现的问题，并补明直接相关的调用前提。

逐文件表中的“通过”表示重新复核实现且在本轮执行的合法输入域测试中未发现反例，不是无限输入的形式化证明。随机真值、特殊数据和限制分别列出；同一入口覆盖多个模板，不把入口数当作模板数。详细循环规模可查第一轮审计及测试源文件，本轮额外测试规模在下文单列。

## 新发现及处理

### 1. 闵可夫斯基和的空集合语义错误：已修复

`ConvexHull.cpp` 中，`MincowskySum({}, {{2,3}})` 和反向调用原来都返回单点 `{(2,3)}`。按点对和的定义，空集合与任何集合的和集都是空集合；这不是原点单点集合的单位元语义。

修改为任一集合为空就返回空。保留非空凸包的边归并算法、原接口及方向归一化。`geometry_random.cpp` 新增 1000 组规模含 0 的点集：先枚举原始点集的所有点对和，再用独立 Jarvis 包裹法得到真值，与模板结果比较；覆盖空 / 空、单侧空、单点、线段、重复点、多边形和两种方向。不是只对比两个共享空集合错误的闵和实现。

### 2. Simpson 的中点和加权和溢出：已修复

被积函数为 `1 / x`，区间分别为 `[1e-308, 1.1e-308]` 和 `[1e308, 1.1e308]`。端点、函数采样值和真实积分均有限；独立真值为 `log(r/l)`，约 `0.095310179804325`。原实现两例均返回 `NaN`，即使函数平滑且区间相对宽度仅为 10%。

第一例的 `f(a) + 4 * f(mid) + f(b)` 溢出；第二例的 `(a + b) / 2` 溢出。仅调整递归深度或误差阈值不能解决中间量问题。中点、区间宽度和求积加权和改用 `long double` 中间量，再转换到现有 `double` 接口；保留递归和上一轮新增的局部判停检查。

固定反例在深度 4、`eps = 1e-10` 下分别返回 `0.0953101798043251569` 和 `0.0953101798043251014`，误差均小于 `2e-16`。没有放宽阈值。默认深度也纳入正常回归。

`audit_gaps_random.cpp` 另新增 1000 组跨 `1e-308` 至 `1e308` 的随机正区间，用解析对数作真值，正向与反向各测一次；同时保留极端尺度固定例、上一轮多项式抵消反例及其 1000 组变换。

数值限制仍然存在：采样值与各积分近似须能用 `double` 表示；本修复在 Linux GCC 的宽范围 `long double` 上验证，不承诺 `long double` 与 `double` 同范围的平台也支持这些极端尺度。`eps` 仍是采样误差估计，不能保证任意尖峰、振荡或深度耗尽时的精度。

### 3. 三分与欧拉路的调用前提：已补明

- `TernarySearch.cpp`：极值区间外需要严格单调。非最优平台会使相等比较误删含最优点的一侧，例如 `min(1, 100*(x-1)^2)` 在 `[0,1]` 上有非最优平台。此类弱单峰函数不纳入三分模板保证域；未擅自把三分替换为别的算法。
- 两个欧拉路模板：声明 `n >= 1`，无边时返回 `{1}`。空边集与空顶点集不同；不把 `n = 0` 时的非法起点调用当作一般图算法必须支持的情形。
- `XorBasis` 只支持位 0..62、树构建需要合法非空树及根、静态值域结构需要可分配的值域等既有约束不变，不以声明域外输入制造错误报告。

### 4. 独立几何真值的重复点缺陷：已修正

新增退化数据使原 `brute_hull` 暴露问题：输入两个完全相同的点 `(-2,-1)` 时，按位置编号闭环的 Jarvis 枚举输出两个相同点，而正确凸包只应有一个点。原始种子在新增补测第 335 组触发，salt `2026101209` 也触发了这个断言。

在真值程序内按精确坐标排序去重，再执行原 Jarvis 算法，并加入固定重复点断言；没有修改模板来迎合错误真值，也没有删除重复点测试或放宽断言。最初失败的全量流程和换种子结果均保留，最终结果另存。

## 执行证据

证据目录为 `build/audit-20261012-round2/`，随机快照在 `build/audit-20261012/reseed-*/`。这些为本地构建证据，不提交二进制产物。修复前后日志分开，不覆盖原失败记录。

| 检查 | 当前状态 | 证据 |
| --- | --- | --- |
| 本轮修复前全量流程 | 退出 0，145 个模板语法检查，45 个无输入程序及独立整数 / 对拍流程通过 | `full-linux.log` |
| 本轮修复前两组新种子 | salt `2026101208`、`2026101209` 各 42 / 42 入口通过；仍未覆盖新反例，因此不能据此否定新发现 | `reseed-2026101208.log`、`random-before.json`、`reseed-2026101209.log` |
| 独立固定反例 | 修复前退出 1：空集合结果非空、两例积分为 NaN；修复后退出 0 | `boundaries-before.log`、`boundaries-after.log`；[复现源码](fixtures/audit_round2_boundaries.cpp) |
| 修复后专项换种子 | salt `2026101208` 的积分 / 数学补测和几何补测分别通过 ASan / UBSan | `gaps-after-1208.log`、`geometry-after-1208.log` |
| 真值修正后的专项复验 | 原始固定重复点、salt `2026101208`、`2026101209`、`2026101210` 的几何入口均通过 | `geometry-final-1208.log`、`geometry-final-1209.log`、`geometry-final-focus.log`；原始种子另在最终全量流程执行 |
| 第一次修复后复跑 | 全量流程止于几何补测；salt `2026101209` 为 41 / 42，通过独立最小用例确认是真值程序的重复点缺陷 | `full-linux-after.log`、`reseed-2026101209-after.log`、`geometry-diagnose.log` |
| 最终全部随机入口 | salt `2026101210` 的 42 / 42 入口全部通过 ASan / UBSan，145 个模板及 42 个测试 SHA-256 均与当前文件一致；不覆盖失败快照 | `reseed-2026101210-final.log`、`random-final.json` |
| 最终全量 Linux 流程 | 完整退出 0；145 个模板及验证 / 工具语法检查、45 个无输入程序通过 ASan / UBSan；256 MiB 栈，含深链与接口烟测 | `full-linux-final.log` |
| 独立整数与对拍工具 | 李超树 200 组在线解码 + 200 组大整数真值通过；6 个示例各 200 轮、C++ 各 20 轮及错误答案 / 崩溃 / 超时等故障路径通过 | `full-linux-final.log` |
| 官方生成数据 | 20 题、409 组通过；核对固定官方版本的数据哈希及官方 checker，树形图仅比较最优权，无新增在线提交 | `public.log`、`public-results.json` |
| 索引、逐文件表与源码 SHA-256 | 145 行无缺漏 / 重复，每行有直接验证入口；相对本轮起始快照仅 2 个实现修复和 3 个前提注释变化 | `source-index.json`、`random-before.json` |
| 正式打印册 | 147 项、231 页，全部源码行与字体 / 页边界检查通过；第 96、97、171、196、197、226 页渲染检查通过 | `print-book-check.log`、`output/pdf/template.pdf`、`output/pdf/template.layout.json`、`page-*.png` |

打印生成时根目录兼容副本一度写入被拒绝，正式输出未受影响。稍后重试已成功同步，根目录 `template.pdf` 与 `output/pdf/template.pdf` 的 SHA-256 均为 `ebef308f6c3798ba76a0f07248ad456616e716cd7470152cad2e5f8bd58e5d76`。没有强制关闭程序或移除旧文件。

本轮官方数据仓库固定提交为 `1814c4e5205517e368bb57a8d1127eb961cfeaae`。`random-before.json` 保存初始 salt `2026101208` 的快照，`random-oracle-failure.json` 保存真值修正前 salt `2026101209` 的失败快照，`random-final.json` 保存 salt `2026101210` 的最终快照。最早 salt `2026101209` 的通过记录保留在初次日志中；其通用 runner 目录在第二次同 salt 执行时被重用，不能将该目录当前的 JSON 误称为最早执行快照。

## 逐文件复核

入口缩写在文末链接至实际验证程序。所有行均重新阅读实现并关联本轮随机 / 固定测试；“澄清后通过”不表示算法被修改。“修复后通过”须结合最终全量执行状态读取。

### 数据结构：19 个

| 模板 | 本轮实现复核重点 | 随机真值与特殊数据 | 入口 | 结论 |
| --- | --- | --- | --- | --- |
| `template/ds/AffineSegTree.cpp` | 仿射标记复合顺序、取模与负数归一化 | 逐项数组更新；空树、赋值、模数 1、整数极值参数 | XDS | 通过 |
| `template/ds/DSU.cpp` | 根大小、路径压缩、重复 init | 显式集合标签；自合并、失败合并、单点与空初始化 | C | 通过 |
| `template/ds/Fenwick.cpp` | 前缀边界及 kth 的非负权前提 | 数组前缀扫描；零权、负增量、空树、非法 k | C、XDS | 通过 |
| `template/ds/Fenwick2D.cpp` | 离散坐标、严格前缀与矩形开闭边界 | map 枚举；重复坐标、倒置矩形、ll 极值、空集 | XDS | 通过 |
| `template/ds/FHQTreap.cpp` | split / merge 引用、重复键、翻转标记 | multiset / vector；空集、重复、删除缺失键、ll 极值 | ND | 通过 |
| `template/ds/IntervalSet.cpp` | 相邻合并、截断、最大端点的半开区间 | 布尔覆盖和 mex；相邻、切开、全部删除、ll 极值 | CS | 通过 |
| `template/ds/KDTree.cpp` | 包围盒剪枝、同距编号、距离整数范围 | 枚举点对；重复、空集、排除自身、10 万共线点 | KD | 通过 |
| `template/ds/LCT.cpp` | access / 翻转 / link / cut 的父子关系 | 显式森林找路径；非法连删、换根、同点、深链 | XDS、DEEP | 通过 |
| `template/ds/LeftistHeap.cpp` | 合并消耗根、距离维护与相等键排序 | 多个 multiset；空堆、同键、连续 pop、重新 init | P | 通过 |
| `template/ds/LiChaoTree.cpp` | 整数交叉比较、线段覆盖及值域大小前提 | Python 大整数和逐线枚举；同值、竖线、int 坐标极值 | LC、REG、PYLC | 通过 |
| `template/ds/MergeSegTree.cpp` | 破坏性合并 / 分裂、节点共享限制 | 频次数组；空根、根清空、非法 kth、INT_MAX 值域 | XDS、REG | 通过 |
| `template/ds/PersistentArray.cpp` | 路径复制、旧版本不变与 init 失效 | 完整快照；历史分叉、单点、空初始化、ll 极值 | MD | 通过 |
| `template/ds/PersistentSegTree.cpp` | 前缀根差及压缩值的秩 | 区间排序；重复值、空 build、单元素、ll 极值 | ND | 通过 |
| `template/ds/PersistentXorTrie.cpp` | 版本包含关系、计数相减与最高位 | 多重集快照；空差、全 1、重复、版本分叉 | PX | 通过 |
| `template/ds/RollbackDSU.cpp` | 失败合并栈长、恢复 siz、有效快照 | 每步全点对连通性；回滚中间状态、回滚到 0、init | B、GR | 通过 |
| `template/ds/SegmentTreeBeats.cpp` | 严格次值、双侧截断及加法标记 | 逐项数组；全等、双值、交替截断、合法大值、重建 | MD | 通过 |
| `template/ds/SegTree.cpp` | lazy 下推、位置搜索与 kth 前提 | 逐项数组与扫描；空区间、负权、零权、越界 k | C、CS | 通过 |
| `template/ds/WeightedDSU.cpp` | 差值方向、异或关系和矛盾判定 | 独立 BFS；自关系、未连通、重复约束、最高位 | ND | 通过 |
| `template/ds/XorTrie.cpp` | 位 63、删除计数与缺失值处理 | 多重集枚举；0、ULLONG_MAX、空集、重复 | ND | 通过 |

### 图论与树：47 个

| 模板 | 本轮实现复核重点 | 随机真值与特殊数据 | 入口 | 结论 |
| --- | --- | --- | --- | --- |
| `template/graph/BipartiteMatch.cpp` | 增广重置、空部及重复 solve | 枚举匹配；缺边、不完全匹配、空部、重复调用 | GR、REG | 通过 |
| `template/graph/BlockCutTree.cpp` | 圆方节点归属及分量森林 | 删点后连通性；孤点、重边、不连通、深链 | AT、DEEP | 通过 |
| `template/graph/Bridge.cpp` | 父边编号与重边 low 更新 | 逐边删除；平行边、重复 find、不连通、深链 | GR、DEEP、REG | 通过 |
| `template/graph/BridgeTree.cpp` | 桥连接分量与编号约定 | 连通性和森林性；孤立分量、重边、深链 | AT、DEEP | 通过 |
| `template/graph/Centroid.cpp` | 去重计数、非负距离与阈值减法 | 无序点对枚举；星形、零权、负阈值、LLONG_MAX 单边 | NT | 通过 |
| `template/graph/CentroidTree.cpp` | 点分层距离及子分支容斥 | 独立树距离；负点权、阈值等号、重建清空、深链 | MG | 通过 |
| `template/graph/CutVertex.cpp` | 根孩子数及平行父边对割点判据的影响 | 逐点删除；重边、不连通、重复 find、深链 | GR、DEEP、REG | 通过 |
| `template/graph/DiffConstraints.cpp` | 不等式方向、负环与整体平移 | 独立 i128 Floyd；极大负权、不可表示解、重复 solve | GR、REG | 通过 |
| `template/graph/Dijkstra.cpp` | 非负边及 inf 哨兵的可加范围 | 独立 Floyd；不可达、自环、重边、零权、大权 | C | 通过 |
| `template/graph/Dinic.cpp` | 自环反向编号、增流语义与残量维护 | 穷举流量 / 割；零容量、反向边、二次 flow、init | F、AT、REG | 通过 |
| `template/graph/DirectedEulerTrail.cpp` | 入出度差、起点与全部边使用数 | 按边编号枚举；无边、自环、重边、不连通、深链 | GR、DEEP | 澄清 n >= 1 后通过 |
| `template/graph/DirectedMST.cpp` | 环收缩、权差与根可达性 | 枚举入边；负环权、自环、重边、单点、极大单边 | XGR | 通过 |
| `template/graph/Dominator.cpp` | DFS 编号、半支配及不可达点 | 删点支配集合；所有根、自环、重边、长链 | DOM | 通过 |
| `template/graph/DSUOnTree.cpp` | 重儿子保留与外部回调恢复 | 枚举子树颜色；换根、重复 build、状态清理、深链 | DT、DEEP | 通过 |
| `template/graph/DynamicConnectivity.cpp` | 重边计数、时间区间和回滚 | 当前图遍历；自环、重复边、反向 erase、多次 solve | XDS | 通过 |
| `template/graph/EBCC.cpp` | 桥排除、分量归属及边号下标 | 独立删桥重连通；重边、孤点、不连通、深环 | GR、AT、DEEP | 通过 |
| `template/graph/EulerLCA.cpp` | 欧拉序首次出现、换根重建 | 父亲逐步跳；全根全点对、单点、深链 | GR、DEEP | 通过 |
| `template/graph/Floyd.cpp` | 对角初始化、不可达与负环标记 | i128 松弛；负自环、孤立负环、重边、空图 | XGR | 通过 |
| `template/graph/FunctionalGraph.cpp` | 入环树、环距离与大步跳转 | 逐步走图；空图、自环、长尾、ULLONG_MAX 步数 | CS、DEEP | 通过 |
| `template/graph/GeneralMatching.cpp` | 花缩环与增广路径恢复 | 子集匹配 DP；奇环、重边、自环忽略、重复 solve | XGR | 通过 |
| `template/graph/GlobalMinCut.cpp` | 收缩权值、返回一侧与总权上界 | 割子集枚举；空 / 单点、不连通、近 ll 上界单边 | XGR | 通过 |
| `template/graph/GomoryHu.cpp` | 父关系调整、割值与容量复制 | 全点对割枚举；不连通、自环、重边、重复 build | AT | 通过 |
| `template/graph/HLD.cpp` | 重链编号、子树连续与路径方向限制 | 父亲扫描和路径集合；换根、单点、深链 | NT | 通过 |
| `template/graph/HopcroftKarp.cpp` | 分层增广与最小点覆盖 | 子集 DP / 点覆盖；空部、重边、增边后重算、长结构 | NF、XGR | 通过 |
| `template/graph/Johnson.cpp` | 全局负环、重赋权与 inf 范围 | i128 Floyd；势能负边、孤立负环、空图、重复 solve | XGR | 通过 |
| `template/graph/KM.cpp` | 全左覆盖语义、松弛标签与失败清理 | 全匹配枚举；n > m、负权、缺边、ll 极值 | NF | 通过 |
| `template/graph/Kruskal.cpp` | 森林返回值、连通判定和累加 | 独立 Prim；负权、同权、不连通、单点 | C | 通过 |
| `template/graph/KruskalTree.cpp` | 升 / 降序重构与不连通查询 | Floyd minimax / maximin；同点、负权、重复建树 | AT、DEEP | 通过 |
| `template/graph/KShortestWalk.cpp` | 左偏堆偏移、零环与游走语义 | 独立多次出堆枚举；重边、空游走、不可达、近 inf | KSW | 通过 |
| `template/graph/KthAncestor.cpp` | 长链索引、深度外哨兵与换根 | 父亲逐步跳；k = 0、深度外、单点、50 万点链 | AT、DEEP | 通过 |
| `template/graph/LCA.cpp` | 根父哨兵与倍增边界 | 全根全点对父亲跳跃；单点、重复 build、深链 | GR、DEEP | 通过 |
| `template/graph/LowerBoundFlow.cpp` | 需求平衡和允许负源汇净流语义 | 穷举流量及方案；强制下界、无解、自环、大容量 | NF | 通过 |
| `template/graph/LowerBoundMinCostFlow.cpp` | 供需方向、负环消除与重算 | 全边流量枚举；孤立负环、自环、无解、重复 solve | LBC | 通过 |
| `template/graph/MaxClique.cpp` | 候选剪枝、补图与着色上界 | 子集枚举 / 5 点全图；空图、完全图、重复调用 | CLIQUE | 通过 |
| `template/graph/MinCostFlow.cpp` | 势能初始化、自环反向边与负环限制 | 枚举流量；负边 DAG、重建、重复增流 | F、LBC、REG | 通过 |
| `template/graph/Reroot.cpp` | 子树贡献、换根差及 ll 中间量 | 全点对距离后求和；零权、重复调用、深链封闭式 | GR、DEEP | 通过 |
| `template/graph/ScalingMinCostFlow.cpp` | 全局费用优化与残量重算语义 | 任意小图流量枚举；孤立负环、负自环、无通路 | F、REG | 通过 |
| `template/graph/SCC.cpp` | 活跃栈、low 与重复 build 重置 | 传递闭包；自环、重边、不连通、深链 | GR、DEEP | 通过 |
| `template/graph/SteinerTree.cpp` | 子集合并、最短路传播及终端去重 | 边子集枚举；空 / 重复终端、零权、不连通、近 inf | MG | 通过 |
| `template/graph/TopologicalSort.cpp` | 重边入度、空图与有环可选返回值 | 独立闭包判环；空图、自环、菱形 DAG、一般有环图 | A、C | 通过 |
| `template/graph/TreeDiameter.cpp` | 双扫的非负权前提与端点返回 | 全点对距离；单点、零权、深链 | GR、DEEP | 通过 |
| `template/graph/TwoSAT.cpp` | 文字映射、强制约束及方案方向 | 所有布尔赋值；空变量、矛盾、同变量、深链 | NT | 通过 |
| `template/graph/UndirectedEulerTrail.cpp` | 按边去重、自环度数及非孤点连通 | 边编号枚举；无边、重边、自环、不连通、深链 | GR、DEEP | 澄清 n >= 1 后通过 |
| `template/graph/VBCC.cpp` | 分量栈、单边 / 孤点与极大性 | 枚举点集及删点；重边、重复 build、不连通、深链 | A、GR、AT、DEEP | 通过 |
| `template/graph/VirtualTree.cpp` | LCA 闭包、最近祖先连接及清理 | 关键点对闭包；空 / 重复点、换根、重复 query | NT | 通过 |
| `template/graph/WeightedMatching.cpp` | 花状态、松弛标签、非正边及权上界 | 子集 DP / 5 点三态全图；奇花反例、ll 边界、500 点结构 | WM | 通过 |
| `template/graph/ZeroOneBFS.cpp` | 距离松弛与严格 0 / 1 边权前提 | 独立 Bellman-Ford 式松弛；零环、重边、不可达 | XGR | 通过 |

### 数学：34 个

| 模板 | 本轮实现复核重点 | 随机真值与特殊数据 | 入口 | 结论 |
| --- | --- | --- | --- | --- |
| `template/math/AdaptiveSimpson.cpp` | 误差抵消、有限极端尺度的中间量 | 解析多项式 / log；原反例、1000 组尺度区间、反向、深度 0 | A、R、FIX | 修复后通过 |
| `template/math/BerlekampMassey.cpp` | 零递推、特征式取模与初始项 | 逐项递推；零序列、空递推、初始 / 后续项 | R、REG | 通过 |
| `template/math/BostanMori.cpp` | 分母奇偶消去、大 k 和零阶递推 | 逐项展开 / Fibonacci 倍增；空分子、ULLONG_MAX、2048 阶 | CM | 通过 |
| `template/math/BSGS.cpp` | 最小指数、非互质约分与模数 1 | 逐幂首达；0 底数、无解、负数、ll 极值 | B、NM | 通过 |
| `template/math/Combination.cpp` | 阶乘可逆条件、越界 k 和重复 init | 独立 Pascal；n = 0、小素数、表外参数 | A、R | 通过 |
| `template/math/CRT.cpp` | 非互质合并、最小解与 lcm 溢出 | 枚举周期；空方程、冲突、模数 1、溢出异常 | NM | 通过 |
| `template/math/Determinant.cpp` | 消元符号、模数前提及有向矩阵树方向 | 排列展开 / 边子集；空、奇异、重边、自环、负元素 | XM | 通过 |
| `template/math/DivisorTransform.cpp` | 变换遍历方向、逆变换及上界截断 | 整除 / 数对枚举；n = 0、合数、模数 1 | CM | 通过 |
| `template/math/DuJiao.cpp` | 分块末端、缓存键、phi 的 i128 累加 | gcd / 试除前缀；表内外、n = 0、init 清缓存 | XM | 通过 |
| `template/math/EquidistantLagrange.cpp` | 采样点命中、前后缀乘积及归一化 | 独立 Horner；空样本、ll 横坐标极值、10 万采样点 | CM | 通过 |
| `template/math/ExLucas.cpp` | 质因子阶数、质数幂阶乘和预处理成本 | i128 Pascal；合数、模数 1、非法 k、极大 n 的边缘 k | XM | 通过 |
| `template/math/FFT.cpp` | 长度与 CRT 重建范围、浮点接口区别 | 双循环卷积 / DFT；空、负系数、极值系数、合数模数 | NP | 通过 |
| `template/math/FloorSum.cpp` | 负数数学下取整与 i128 中间量 | 逐项枚举 / 封闭式；n = 0、ll 极值、负 a / b | XM | 通过 |
| `template/math/FPS.cpp` | 常数项、截断、零前缀与幂移位 | 系数递推 / 朴素多项式；空、无平方根、ULLONG_MAX 幂 | NP、XM | 通过 |
| `template/math/FWT.cpp` | 三种蝶形及逆变换可逆条件 | 双循环卷积；长度 1、零掩码、正逆互验 | R | 通过 |
| `template/math/Gauss.cpp` | 主元、自由变量、模域与实数容差 | 模赋值枚举 / 小整数秩；无解、多解、零行列、近零主元 | NM | 通过 |
| `template/math/Lagrange.cpp` | 模意义互异横坐标与采样点直返 | 独立 Horner；单点、零系数、命中采样点 | R | 通过 |
| `template/math/LinearSieve.cpp` | 最小质因子递推、0 / 1 与重建 | 试除 / gcd 计数；质数幂、反复缩小和放大 | A、NM | 通过 |
| `template/math/Lucas.cpp` | 按 p 进位分解和 O(p) 内存条件 | Pascal；p = 2、跨 p、非法 k、极大边缘组合数 | A、NM | 通过 |
| `template/math/Matrix.cpp` | 维度、乘法中间量和指数 0 | i128 点积 / 逐次乘方；全 mod-1、单位阵大幂、mod = 2^31 | A、R | 通过 |
| `template/math/MatrixInverse.cpp` | 可逆判定、行交换及左右逆 | 排列行列式和左右乘积；空、奇异、负值、INT_MAX 模数 | P | 通过 |
| `template/math/Min25.cpp` | 商表索引、二次素数值和三次中间量 | 逐数试除 / phi 筛前缀；n = 0、负系数、合数模数 | BM | 通过 |
| `template/math/ModArithmetic.cpp` | 大模乘法、负底数和逆元前提 | 逐次乘法 / 验逆；模数 1、LLONG_MIN / MAX、空批量 | P | 通过 |
| `template/math/ModSqrt.cpp` | p = 2、非剩余与最小根 | 枚举根 / 大素数回平方；0、负数、INT_MAX 素数 | XM | 通过 |
| `template/math/Multipoint.cpp` | 余式树、零多项式与插值点互异 | Horner / 朴素插值；重复求值点、空、重建、4096 点 | AP | 通过 |
| `template/math/NTT.cpp` | 蝶形下标、逆归一化与 2^23 上限 | 双循环卷积；空、长度 1、各 2 的幂、4096 项应用 | R、CM | 通过 |
| `template/math/PollardRho.cpp` | ull 模乘、判素基底与随机重试 | 小数试除 / 分解乘积；伪素数、0 / 1、大半素数、ULLONG_MAX | B、NM | 通过 |
| `template/math/PrimeSieve.cpp` | 0 / 1、小规模边界与重复调用 | 逐数试除；小质数、稠密合数段、随机上界 | A、R | 通过 |
| `template/math/PrimitiveRoot.cpp` | p-1 因子检验与最小性 | 剩余类枚举；p = 2、大素数、ull 上界附近素数 | B、P | 通过 |
| `template/math/QuotientBlocks.cpp` | 最大右端点、终段与规模成本 | 逐元素核对商；0 / 1 / 2、10^12 回归、连续性 | A、R、REG | 通过 |
| `template/math/Simplex.cpp` | 双阶段可行性、退化循环及容差 | 顶点枚举；无解、无界、零维、负 RHS、Beale 例 | BM | 通过 |
| `template/math/Stirling.cpp` | 无符号第一类、递推 / 卷积模数区别 | 计数恒等式 / 小规模全枚举；n = 0、合数、4096 项 | CM | 通过 |
| `template/math/SubsetConvolution.cpp` | 秩分层、逆变换与内存上界 | 所有子掩码枚举；n = 0、负系数、极值、模数 1 / 合数 | AP | 通过 |
| `template/math/XorBasis.cpp` | 有序化、kth 与位 0..62 前提 | 可表示异或集合枚举；相关向量、空基、满 63 维、非法 k | R、XM | 通过 |

### 字符串：14 个

| 模板 | 本轮实现复核重点 | 随机真值与特殊数据 | 入口 | 结论 |
| --- | --- | --- | --- | --- |
| `template/string/ACAutomaton.cpp` | fail 累加、重复模式与新增后 build | 子串枚举；重叠、空文本、无模式、clear、多批 build | R、B、REG | 通过 |
| `template/string/BitsetLCS.cpp` | 多块借位及字节索引 | 普通 LCS DP；空串、256 字节、63 / 64 / 65 等块边界 | XO | 通过 |
| `template/string/Hash.cpp` | 子串开闭边界、空串和跨对象 | 直接多项式 / LCP；NUL、高字节、全等、重复 build | NS、A | 通过，有碰撞风险 |
| `template/string/Hash2D.cpp` | 二维容斥、形状和空矩形 | 直接逐行列多项式；空 / 零宽矩阵、任意字节、跨对象 | CS | 通过，有碰撞风险 |
| `template/string/KMP.cpp` | 失配跳转及空模式的 n+1 位置约定 | 逐起点 compare；空、重叠、周期、NUL / 高字节 | C、B | 通过 |
| `template/string/Lyndon.cpp` | Duval 分界与 unsigned char 顺序 | 三元串穷举 / 直接分解核验；空、全等、周期、长串 | BS | 通过 |
| `template/string/Manacher.cpp` | 奇偶半径和闭区间查询 | 逐子串回文枚举；空、全等、周期、NUL / 高字节 | C、B | 通过 |
| `template/string/MinRotation.cpp` | 相等旋转的最早起点和字节顺序 | 枚举旋转 / 二元串穷举；空、全等、周期、高字节 | NS | 通过 |
| `template/string/PAM.cpp` | 虚根、fail 计数与重复 count | 前缀回文频次枚举；空、全等、重复 build、继续插入 | NS | 通过 |
| `template/string/Runs.cpp` | LCE / LCS 边界、最小周期和极大性 | 三元串穷举 / 子串枚举；空、周期、任意字节、长全等 | BS | 通过 |
| `template/string/SAM.cpp` | clone、拓扑累计与重复 count | 所有子串频次；空、全等、全异、clone、重复 build | R、B | 通过 |
| `template/string/SuffixArray.cpp` | 倍增排序、LCP RMQ 与空后缀限制 | 后缀排序 / 直接 LCP；空、周期、NUL / 高字节、重建 | SA、B、BS | 通过 |
| `template/string/Trie.cpp` | 根计数、空串和 clear | 完整模式列表；重复插入、缺失、空前缀、空模式 | R、B | 通过 |
| `template/string/Z.cpp` | z[0] 约定和扩展匹配边界 | 直接 LCP / 二元串穷举；空模式、空文本、全等、高字节 | NS | 通过 |

### 其他算法：18 个

| 模板 | 本轮实现复核重点 | 随机真值与特殊数据 | 入口 | 结论 |
| --- | --- | --- | --- | --- |
| `template/others/CartesianTree.cpp` | 相等优先级及中序顺序 | 堆性质 / 中序 / 父子关系；空、全等、单调、ll 极值 | R、B | 通过 |
| `template/others/CDQ.cpp` | 重复点合并、回滚和非严格偏序 | 所有点对枚举；空、100 个全等、ll 极值坐标 | NO | 通过 |
| `template/others/Common.cpp` | 比较返回值、模加减归一化及类型上界 | 直接运算；相等、进位、借位、模数 1 | A、R | 通过 |
| `template/others/ConvexHullTrick.cpp` | 斜率 / 查询单调、同斜率及交叉积 | 逐线枚举 / 二次 DP；相等、负截距、大数、长 DP | P | 通过 |
| `template/others/Discretize.cpp` | 排名起点与不修改原数组 | 有序集合排名；空、重复、负数、ll 极值 | R | 通过 |
| `template/others/DLX.cpp` | 覆盖 / 恢复对称性与成功路径清理 | 行子集枚举及字段快照；零列、无解、重复 solve、追加行 | DLX | 通过 |
| `template/others/DPOpt.cpp` | 最左 argmin 单调前提、背包队列窗口 | 普通转移枚举；不可达、负价值、数量 0、精确 / 至多体积 | XO | 通过 |
| `template/others/LIS.cpp` | 严格 / 非严格二分与方案回溯 | 二次 DP；空、重复、负数、ll 极值、下标方案 | CS | 通过 |
| `template/others/Mo.cpp` | 初始空区间与回调下标语义 | 区间直接扫描；无查询、同点、重复区间、空操作 | R、B | 通过 |
| `template/others/MoModify.cpp` | 时间移动、旧值记录与外部状态重置 | 每次修改后 set 统计；同值修改、无查询、重复 solve | XO | 通过 |
| `template/others/MonotonicStack.cpp` | 严格不等关系与缺失哨兵 | 双向逐点扫描；空、重复、负值、ll 极值 | P | 通过 |
| `template/others/MoTree.cpp` | 欧拉奇偶、额外 LCA 与换根重建 | 独立 DFS 路径 / set；同点、祖先 / 非祖先、深链 | XO、DEEP | 通过 |
| `template/others/OverallBinary.cpp` | 时间顺序、修改贡献回滚与非法 k 前提 | 当前区间排序；空查询、重复修改、ll 极值、重复 solve | NO | 通过 |
| `template/others/SG.cpp` | mex 大小上界及环拒绝 | 双子游戏胜负 DP / 逐点 mex；终点、重边、自环、深链 | CS、DEEP | 通过 |
| `template/others/SlidingWindow.cpp` | 过期下标、相等元素与合法窗口 | 逐窗口扫描；全等、单调、单点、ll 极值、全部合法 k | R、B | 通过 |
| `template/others/SparseTable.cpp` | 对数边界与幂等重叠查询 | 枚举区间 max；单点、全等、单调、负数、重新 build | R、B | 通过 |
| `template/others/TernarySearch.cpp` | 相等比较及极值区间外严格单调前提 | 已知顶点二次函数；端点、零宽、常数、最小 / 最大版 | A、R | 澄清前提后通过 |
| `template/others/TreeHash.cpp` | 有根语义、子树顺序不变及随机盐 | 排序括号编码；单点、换编号、打乱邻接、深链 | R、DEEP | 通过，有碰撞风险 |

### 计算几何：13 个

| 模板 | 本轮实现复核重点 | 随机真值与特殊数据 | 入口 | 结论 |
| --- | --- | --- | --- | --- |
| `template/geometry/Circle.cpp` | 相切 / 包含分类、交点与切线条件 | 交点方程 / 距离；重合、零半径、共线三点、解析面积 | NG | 通过，受浮点限制 |
| `template/geometry/CircleUnion.cpp` | 角区间覆盖、重复圆及包含剔除 | 独立竖切片积分 / 圆周采样；洞、内外相切、近重合 | CU | 通过，数值证据 |
| `template/geometry/ClosestPair.cpp` | 分治条带、重复点和少于两点 | 所有点对枚举；空、单点、共线、竖线、乱序、重建 | XO | 通过 |
| `template/geometry/ConvexHull.cpp` | 排序严格性、直径方向与闵和空集合 | Jarvis / 点对枚举；新增 1000 组含空输入、退化、重复和方向变换 | GEO、FIX | 修复后通过 |
| `template/geometry/ConvexHull3D.cpp` | 初始维度、可见面、朝向和体积 | 整数支撑面枚举；重复、共面、共线、立方体、三种顺序 | H3 | 通过，受浮点限制 |
| `template/geometry/ConvexQuery.cpp` | 任意点集建包、扇区二分及边界 | 线性 contain / 边界验证；空、点、线段、共线、重复 build | XO | 通过 |
| `template/geometry/HalfPlane.cpp` | 平行方向排序、队列裁剪与空结果语义 | 独立逐面裁剪；冲突、平行、线 / 点退化、signed zero | NG | 通过，空结果含多种情况 |
| `template/geometry/Line.cpp` | 退化线段、平行前提及交点参数 | 独立整数谓词 / 最近点方程；相触、交叉、共线、点线段 | A、GEO | 通过 |
| `template/geometry/MinCircle.cpp` | 随机增量边界及共线候选 | 枚举 1 / 2 / 3 点候选圆；空、重复、共线、零半径 | NG | 通过，受浮点限制 |
| `template/geometry/MinRectangle.cpp` | 严格逆时针前提及多卡壳指针 | 逐边方向枚举；空、点、线、变换、循环起点、10 万点凸包 | MR | 通过 |
| `template/geometry/Point.cpp` | 精确排序与 EPS 判等分工、零向量 | 独立叉积 / 点积 / hypot；近 EPS、轴向、负数、旋转 | A、GEO | 通过 |
| `template/geometry/Polygon.cpp` | 平移后的面积、边界包含与凸裁切 | 矩形解析面积 / 点包含；空、首尾重复、凹多边形、大平移 | A、GEO | 通过 |
| `template/geometry/RectangleUnion.cpp` | 同横坐标事件、覆盖长度与边界条数 | 整数网格面积 / 边界；共边、重复、退化、乱序、极值平移 | XDS | 通过 |

## 验证入口索引

| 缩写 | 文件 |
| --- | --- |
| A | [audit_gaps_random.cpp](audit_gaps_random.cpp) |
| B | [audit_state_random.cpp](audit_state_random.cpp) |
| C | [common_templates_random.cpp](common_templates_random.cpp) |
| R | [remaining_templates_random.cpp](remaining_templates_random.cpp) |
| GR | [graph_random.cpp](graph_random.cpp) |
| F | [flow_random.cpp](flow_random.cpp) |
| XDS | [extra_ds_random.cpp](extra_ds_random.cpp) |
| XGR | [extra_graph_random.cpp](extra_graph_random.cpp) |
| XM | [extra_math_random.cpp](extra_math_random.cpp) |
| XO | [extra_offline_random.cpp](extra_offline_random.cpp) |
| ND | [new_ds_random.cpp](new_ds_random.cpp) |
| NF | [new_flow_random.cpp](new_flow_random.cpp) |
| NG | [new_geometry_random.cpp](new_geometry_random.cpp) |
| NM | [new_math_random.cpp](new_math_random.cpp) |
| NO | [new_offline_random.cpp](new_offline_random.cpp) |
| NP | [new_poly_random.cpp](new_poly_random.cpp) |
| NS | [new_string_random.cpp](new_string_random.cpp) |
| NT | [new_tree_random.cpp](new_tree_random.cpp) |
| AP | [advanced_poly_random.cpp](advanced_poly_random.cpp) |
| AT | [advanced_tree_random.cpp](advanced_tree_random.cpp) |
| MD | [more_ds_random.cpp](more_ds_random.cpp) |
| MG | [more_graph_random.cpp](more_graph_random.cpp) |
| CM | [completion_math_random.cpp](completion_math_random.cpp) |
| CS | [completion_structures_random.cpp](completion_structures_random.cpp) |
| BM | [batch_math_random.cpp](batch_math_random.cpp) |
| BS | [batch_string_random.cpp](batch_string_random.cpp) |
| P | [priority_templates_random.cpp](priority_templates_random.cpp) |
| LC | [li_chao_random.cpp](li_chao_random.cpp) |
| SA | [suffix_array_random.cpp](suffix_array_random.cpp) |
| DT | [dsu_on_tree_random.cpp](dsu_on_tree_random.cpp) |
| KD | [kdtree_random.cpp](kdtree_random.cpp) |
| PX | [persistent_xor_trie_random.cpp](persistent_xor_trie_random.cpp) |
| DOM | [dominator_random.cpp](dominator_random.cpp) |
| LBC | [lower_bound_cost_random.cpp](lower_bound_cost_random.cpp) |
| CLIQUE | [max_clique_random.cpp](max_clique_random.cpp) |
| KSW | [k_shortest_walk_random.cpp](k_shortest_walk_random.cpp) |
| WM | [weighted_matching_random.cpp](weighted_matching_random.cpp) |
| DLX | [dlx_random.cpp](dlx_random.cpp) |
| CU | [circle_union_random.cpp](circle_union_random.cpp) |
| H3 | [hull3d_random.cpp](hull3d_random.cpp) |
| GEO | [geometry_random.cpp](geometry_random.cpp) |
| MR | [min_rectangle_random.cpp](min_rectangle_random.cpp) |
| DEEP | [deep_stack.cpp](deep_stack.cpp) |
| REG | [template_regressions.cpp](template_regressions.cpp) |
| PYLC | [check_lichao_integer.py](check_lichao_integer.py) |
| FIX | [audit_round2_boundaries.cpp](fixtures/audit_round2_boundaries.cpp) |

## 保留的风险与复跑

- 随机、穷举小规模及官方生成数据不能穷尽输入；哈希不能由这些测试证明无碰撞，随机算法只提供期望复杂度。
- 图 / 树 / 字符集 / 模数 / 维度 / 有效下标等前提仍必须遵守。算术最终答案可表示，不自动意味着所有中间量可表示。
- Min25 的 10^12 上界、整除分块的 LLONG_MAX 规模、满规模子集卷积等未做端到端极限压力测试，不将小规模正确性写成最大规模性能保证。
- 浮点几何、积分、三分、FFT、实数消元和单纯形受尺度、容差和退化情形影响；表中已标明主要边界，第一轮审计保留更细的各接口限制。
- 递归保持原写法，本轮 Linux 栈为 256 MiB；通过不意味着 Windows 默认栈足够。

```sh
bash support/verify/run_linux.sh
python3 support/verify/audit_reseed.py --salt 2026101210
python3 support/verify/audit_reseed.py --salt 2026101208 --only audit_gaps_random
python3 support/verify/audit_reseed.py --salt 2026101208 --only geometry_random
python3 support/verify/check_sources.py
python3 support/verify/run_public.py
```

公开数据复跑使用本地固定版本的 `build/library-checker-problems/`，不需要重新联网，也不表示在线 AC。

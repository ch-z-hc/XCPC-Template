# 全模板逐文件审计：随机数据与特殊数据

日期：2026-10-12。范围：`template/*/*.cpp` 的 145 个模板，不包含比赛入口、工具及验证程序。

本文保留第一轮审计的历史结果及修复证据。后续重新全量复核又发现闵可夫斯基和空集合、积分极端尺度中间量的问题；当前版本的最新结论见 [第二轮全量审计](AUDIT_ROUND2_20261012.md)，不将本文的历史通过记录当作当前版本的完整结论。

## 本次结论与证据

145 项全部有逐文件审计。初次审计中发现 `自适应辛普森积分.cpp` 的确定精度反例，随后按用户要求修复；其余 144 项未发现需要修改的问题。修复后，全部 145 项在所列合法输入域的随机与特殊数据中未再发现反例；45 个标准验证程序和另一组种子的 42 个随机入口全部通过 ASan / UBSan。修复前的失败证据保留，修复后的结果另列，不把有限测试写成普适正确性证明。

审计以当前工作区为准，包括本次开始前已有的未提交修改。初次审计不修改算法；修复阶段只修改 `自适应辛普森积分.cpp` 及其验证、审计记录，同步生成打印册，不覆盖历史 `AUDIT.md`。没有新增在线提交；旧报告中的通过记录不充当本次运行证据。

“通过”只表示在文件声明的合法输入域内，本次测试没有发现反例或消毒器错误，不是对无限输入、浮点精度或概率算法的绝对保证。测试轮数是单次运行的循环规模，不把同一用例中的断言数算成数据组数。随机生成中强制安排的空、重复、零权等也属于特殊数据。

本次补充 [audit_gaps_random.cpp](audit_gaps_random.cpp) 和 [audit_state_random.cpp](audit_state_random.cpp)。补足矩阵、积分、三分、拓扑排序、组合数、筛法、整除分块、辅助函数的独立随机真值，补强点双极大性、回滚中间状态、几何基础运算、直接哈希，以及字符串与数组的固定特殊数据。

新增 [audit_reseed.py](audit_reseed.py)：只在 `build/` 生成测试副本，将测试入口中的随机引擎构造种子与指定 salt 异或，不改变原模板与测试断言。随机算法内部自带的引擎不强行替换。保存原文件 SHA-256、编译命令、修改后的测试副本、逐入口日志、退出状态和耗时；超时与编译失败单独记录。

## 修复前确认问题：自适应 Simpson 错误判停

对应 [自适应辛普森积分.cpp](../../template/math/自适应辛普森积分.cpp)，原误差判停条件为 `abs(x + y - s) <= 15 * err`。这是误差估计，不是严格误差上界。连续六次多项式也能使粗细公式恰好相等而积分不准确；增加 `max_dep` 无法修复已经提前判停的区间。

取以下函数与区间，调用 `simpson(f, -0.1, 0.3, 1e-10)`：

$$
f(x)=\frac{x^4}{5}-x^6,\qquad [l,r]=\left[-\frac{1}{10},\frac{3}{10}\right].
$$

用精确有理数计算，粗、细两级 Simpson 均为：

$$
S_1=S_2=\frac{493}{7500000}.
$$

解析积分及真实误差为：

$$
I=\left[\frac{x^5}{25}-\frac{x^7}{7}\right]_l^r
=\frac{1161}{17500000},\qquad
S_2-I=-\frac{1}{1640625}.
$$

Linux 实测返回 `0.0000657333333333333364`，真值约 `0.0000663428571428571429`，绝对误差约 `6.09523809523806e-7`，超过本例请求的 `1e-10`。原随机反例来自 salt `2026101204`、numeric 的第 `821` 轮，其区间为 `[-0.5, 0.3]`；去掉不影响 Simpson 差值的低次项并取发生误判的子区间，得到上述固定反例。

独立有理数证明保存在 [simpson_alias_exact.py](fixtures/simpson_alias_exact.py)，实际模板复现保存在 [simpson_alias.cpp](fixtures/simpson_alias.cpp)。后者修复前退出 `1`，明确表示精度检查失败。固定反例已加入 `audit_gaps_random.cpp`；修复前它先执行完其余补测，再因 `numeric_ok` 断言退出失败，不因一个积分反例跳过其他模板的测试。

### 修复措施与边界

保留 `simpson(f, l, r, eps, max_dep)` 接口与递归结构。原判据满足时，再分别细分左右子区间，只有两侧差值的绝对值之和也不超过 `15 * err` 才接受，并返回更细一级的 Richardson 校正值。没有放宽原反例的 `1e-10` 误差断言，没有仅靠提高 `max_dep` 或指定固定最小深度绕过反例。

本例下一层两侧绝对差之和精确为 `3/2000000`，大于 `15e-10`，新判据能拒绝原错误判停。实际修复后的返回值为 `0.0000663428571378191321`，绝对误差约 `5.04e-15`，小于 `1e-10`。

这仍是采样型数值积分，不能承诺任意连续函数的严格误差。源码明确 `eps > 0`、`max_dep >= 0`，并说明尖峰 / 高频振荡需按已知位置分段；达到深度上限仍返回近似值。有限采样可能遗漏局部结构，不能把本次修复写成普适的精度保证。

## 修复前实际运行记录

| 检查 | 实际结果 | 本地证据 |
| --- | --- | --- |
| 原有全量 Linux 流程 | 145 个模板及原验证 / 工具语法检查；原有 43 个无输入验证程序通过 ASan / UBSan，脚本退出 0 | `build/audit-20261012-baseline.log` |
| 新增状态与固定边界补测 | 1000 组回滚历史、1500 组独立数论真值及字符串 / 数组固定边界通过 | `build/audit-20261012-state.log` |
| 第一组换种子 | 当时的 41 个随机入口通过；此时尚未加入固定积分反例，不能用作当前积分模板通过证据 | `build/audit-20261012-reseed.log` |
| 第二组换种子 | 42 个入口中的 41 个通过，积分补测失败；其余 40 个原随机入口及状态补测均通过 | `build/audit-20261012-reseed2.log`、`build/audit-20261012/reseed-2026101204/results.json` |
| 最终补测，两组种子各重播一次 | 1000 组数学、2000 组数值、1500 组图、5000 组几何 / 哈希全部执行；均只有积分精度断言失败，日志保留随机与固定反例 | `build/audit-20261012/reseed-2026101202-audit_gaps_random/`、`build/audit-20261012/reseed-2026101204-audit_gaps_random/` |
| 精确反例证明 | Python Fraction 证明通过；调用实际 C++ 模板的复现退出 1 | `fixtures/simpson_alias_exact.py`、`fixtures/simpson_alias.cpp` |
| 独立整数与工具流程 | 李超树两类各 200 组；6 个示例各 200 轮，C++ 各 20 轮，错误答案 / 崩溃 / 超时等故障路径通过 | 全量 baseline 日志 |
| Library Checker 官方生成数据 | 固定已有版本，20 题、409 组通过，不是在线 AC | `build/audit-20261012-public.log`、`build/public-verify/results.json` |
| 逐文件清单及证据完整性 | 145 个模板恰好对应 145 个审计行，缩写均指向存在的测试，每行至少有一个直接引用该模板的验证入口 | 本报告、`build/source-audit/results.json` |

工作区共有 45 个无输入 C++ 验证程序，包含本次新增的 2 个。修复前新增积分回归会失败；原有 43 个通过的日志不是当前全部 45 个通过的记录。修复前日志及中断的长任务日志保留，不覆盖为成功结果。

## 修复后实际运行记录

| 检查 | 实际结果 | 本地证据 |
| --- | --- | --- |
| 原失败反例 | 修复前退出 1；修复后退出 0，原 `1e-10` 阈值不变，误差从约 `6.10e-7` 降至 `5.04e-15` | `build/simpson-fix/before.log`、`build/simpson-fix/after.log` |
| 独立精确证明 | Fraction 证明原判据错误收敛，并证明新局部误差检查会拒绝该区间 | `build/simpson-fix/exact.log` |
| 修复专项，ASan / UBSan | 原 2000 组解析多项式与固定反例通过；新增 1000 组平移 / 缩放 / 叠加三次项，逐组正向与反向积分均满足 `1e-10`；另测零区间、深度 0、exp 与 atan 解析积分 | `build/simpson-fix/focused.log`、`audit_gaps_random.cpp` |
| 原失败随机种子重播 | 使用保留的 salt `2026101204` 测试副本重新编译，原第 821 轮及固定反例全部通过；不覆盖原失败日志 | `build/simpson-fix/original-seed-replay.log` |
| 新增专项种子 | salt `2026101206`，包括新增 1000 组变换回归，通过 ASan / UBSan | `build/simpson-fix/reseed-2026101206.log`、`build/audit-20261012/reseed-2026101206-audit_gaps_random/results.json` |
| 全部随机测试换种子 | salt `2026101207`，42 / 42 个随机入口全部通过 ASan / UBSan，保存完整逐入口状态、耗时与源码 SHA-256 | `build/simpson-fix/reseed-2026101207.log`、`build/audit-20261012/reseed-2026101207/results.json` |
| 标准 Linux 全量算法验证 | 145 个模板及验证 / 工具语法检查通过；256 MiB 栈，45 / 45 个无输入验证程序通过 ASan / UBSan，包含 snippets_smoke、固定回归及深链 | `build/simpson-fix/full-linux-final.log` |
| 李超树独立整数校验 | 在线解码 200 组、大整数端点 200 组，全部通过 Python 精确整数真值核对 | `build/simpson-fix/full-linux-final.log` |
| 对拍工具及完整流程退出状态 | 6 个示例各 200 轮、C++ 各 20 轮，错误答案 / 崩溃 / 缺失程序 / 超时等故障路径全部通过；`run_linux.sh` 完整执行结束，退出 0 | `build/simpson-fix/full-linux-final.log` |
| 改动范围 | 相对初次审计的 145 个模板 SHA-256，仅 `自适应辛普森积分.cpp` 变化，其他 144 个保持不变；重新检查 145 项索引及测试入口 | `build/simpson-fix/source-audit-before.json`、`build/source-audit/results.json`、`build/simpson-fix/source-index.log` |
| 打印册同步 | 147 个条目、231 页，全部源码行与字体 / 页边界检查通过；渲染检查第 171 页，未发现裁切或乱码 | `output/pdf/template.pdf`、`output/pdf/template.layout.json`、`build/simpson-fix/simpson-page.png` |

## 共同检查边界

- 图与树默认 1-based，字符串默认 0-based；遵守各文件例外。有效位置、维度、树结构等属于调用前提，不把越界调用当作必须支持的输入。
- ASan、UBSan 和 `_GLIBCXX_ASSERTIONS` 检查内存及未定义行为；不关闭 assert，不忽略失败，不用浮点近似替代整数真值。
- 递归仍保留。Linux 栈为 256 MiB，相关树算法另有 20 万点深链，第 k 级祖先有 50 万点深链。通过不能推出默认 Windows 栈足够。
- `ll` 的和、差、积必须遵守源码的中间量限制；不是“最终答案放得下就一定安全”。下面各项的更严格前提仍然有效。
- 浮点几何、FFT、积分、三分、实数消元及单纯形只能在已测尺度和容差下给出数值证据；哈希测试不能排除碰撞。

## 数据结构：19 个

每行均为独立模板审计。入口缩写在文末链接到实际验证源文件。

| 模板 | 随机真值与规模 | 特殊数据 | 前提 / 剩余风险 | 入口 |
| --- | --- | --- | --- | --- |
| `template/ds/仿射线段树.cpp` | 2200 组逐项仿射更新与求和 | 空数组、赋值、负参数、模数 1、INT_MAX、LLONG_MIN / MAX | 1-based 闭区间，模数不超过 INT_MAX | XDS |
| `template/ds/并查集.cpp` | 200 组、每组 300 次操作，对照显式集合标签 | 重复合并、自合并、空初始化、单点、反复 init | siz 只对根有效；集合大小用 size | C |
| `template/ds/树状数组.cpp` | 5000 次增量 / 查询；2200 组 kth 对照前缀扫描 | 空结构、零权、负增量、k 越界与非正值、重新初始化 | add 下标至少为 1；kth 要求各权非负 | C、XDS |
| `template/ds/二维树状数组.cpp` | 2200 组，对照坐标到权值的 map 枚举 | 重复点、空集、严格 / 非严格前缀、倒置矩形、ll 坐标极值 | 所有更新坐标必须预先加入 build | XDS |
| `template/ds/无旋平衡树.cpp` | 键值与序列各 60 组 × 1000 次，对照 multiset / vector | 重复键、删除不存在值、空集、严格前驱后继、翻转、ll 极值 | 序列位置合法；随机优先级只有期望复杂度，删除不回收节点 | ND |
| `template/ds/区间集合与最小未出现数.cpp` | 2000 组 × 60 次，对照布尔覆盖与逐点 mex | 相邻合并、区间切开、空区间、LLONG_MIN / MAX、全部删除 | 半开区间，不能覆盖 LLONG_MAX 本身 | CS |
| `template/ds/二维最近邻.cpp` | 500 点集 × 200 对查询，对照直接枚举 | 重复点、同距取小编号、排除自身、空集、坐标 ±10^9、10 万共线点 | 静态二维，坐标绝对值不超过 10^9；最坏查询可线性 | KD |
| `template/ds/动态树.cpp` | 2200 组 × 85 次，对照显式森林的路径与连通性 | 非法 link / cut 返回值、换根、负点权、同点查询、20 万点链 | query 前须连通；只维护路径点权和，不是子树和 | XDS、DEEP |
| `template/ds/左偏树与可并堆.cpp` | 3000 组 × 200 次，对照 8 个 multiset 堆 | 同值编号顺序、空堆合并、连续 pop、init 重建 | 合并堆必须不相交；merge / pop 消耗旧根 | P |
| `template/ds/李超线段树.cpp` | 整数及浮点直线枚举；Python 大整数另测 200 组在线解码、200 组大端点 | P4097 同值编号回归、竖线、重复线、越出线段、int 极值横坐标、10^18 系数 | 整数入口精确判等；浮点或混合入口不能承诺数学同值 | LC、REG、PYLC |
| `template/ds/动态开点线段树合并与分裂.cpp` | 2200 组 × 65 次，6 棵树对照频次数组 | 空根、零频次、合并后原根清空、分裂、非法 kth、INT_MAX 值域回归 | 频次非负；根不能共享非空节点；merge / split 是破坏性操作 | XDS、REG |
| `template/ds/可持久化数组.cpp` | 2000 组 × 50 次历史分叉，对照完整版本快照 | 旧根不变、单点、空初始化、任意 ll 值、反复 init | 调用者保存根；init 后旧版本失效，非空查询下标合法 | MD |
| `template/ds/主席树.cpp` | 120 组 × 200 次，对照区间排序与 upper_bound | 重复值、LLONG_MIN / MAX、单元素区间、空 build | 静态区间第 k 小，k 必须合法，不是通用可修改主席树 | ND |
| `template/ds/可持久化二进制字典树.cpp` | 300 组 × 180 次分叉，对照多重集快照 | 全部闭区间、重复、最高位、全 1、空版本差 | 左版本多重集须包含于右版本；不能对任意两根直接相减 | PX |
| `template/ds/可撤销并查集.cpp` | 1000 组 × 100 次随机合并 / 回滚，逐步核对全点对连通性与根 siz | 失败合并也计栈长、回滚中间快照、回滚到 0、单点、init 清空 | 仅回滚到仍有效的历史快照；无路径压缩 | B、GR |
| `template/ds/吉司机线段树.cpp` | 2000 组 × 100 次，对照数组 add / chmin / chmax，核对所有区间 | 全等、双值、单点 / 全区间、交替截断与加法、±2^59 量级、重建 | 元素与截断值严格在 ±2^60 内，中间量及 lazy 不溢出 | MD |
| `template/ds/线段树与树上二分.cpp` | 200 组区间操作及 2000 组搜索，对照数组扫描 | 空区间、空树、全区间、负权、同点、ll 极值、k 越界及零权 | kth 要求各元素非负；first_ge / last_ge 搜值而非前缀和 | C、CS |
| `template/ds/带权与异或并查集.cpp` | 80 组 × 500 次，差值 / 异或关系对照独立 BFS | 矛盾约束、重复一致约束、自关系、未连通、最高异或位 | 差值语义为 value[v] - value[u]；差值累加不能溢出 ll | ND |
| `template/ds/二进制字典树.cpp` | 60 组 × 1000 次，对照多重集全枚举 | 空集、重复、删除不存在值、bit 63、0、ULLONG_MAX | 无符号 64 位；删除不回收节点 | ND |

## 图论与树：47 个

| 模板 | 随机真值与规模 | 特殊数据 | 前提 / 剩余风险 | 入口 |
| --- | --- | --- | --- | --- |
| `template/graph/二分图匹配.cpp` | 300 组，对照所有左点匹配选择 | 单点、缺边、不完全匹配、重复 solve；空部回归 | 左右部各自 1-based；返回最大匹配边数 | GR、REG |
| `template/graph/圆方树.cpp` | 2000 组，删去每个原点核对两图连通性与森林性 | 孤立点、重边、不连通、多次构建、20 万点链 | 必须提供完整点双顶点列表；保留非割圆点和孤立方点 | AT、DEEP |
| `template/graph/桥.cpp` | 300 组逐边删除，对照连通块数变化 | 重边、不连通、重复 find、20 万点链 | 边编号从 1 开始，编号对应返回结果 | GR、DEEP、REG |
| `template/graph/边双缩点树.cpp` | 2000 组，核对桥编号、连通性与森林无环 | 孤立分量、重边、不连通、20 万点链 | 输入须为完整边双分解，bel 为 1-based 分量编号 | AT、DEEP |
| `template/graph/点分治.cpp` | 600 组枚举无序点对距离 | 零权星形、负阈值、LLONG_MAX 阈值 / 单边、20 万点链 | 连通非负权树，不计同点对；路径与计数放入 ll | NT |
| `template/graph/点分树.cpp` | 2000 组 × 100 次，对照独立 DFS 距离与点权扫描 | 链 / 星、零边、负点权、阈值恰好等于距离、重建清权、20 万点链 | 静态连通非负权树；modify 是增量而非赋值 | MG |
| `template/graph/割点.cpp` | 300 组逐点删除，对照连通块数变化 | 根有多个子树、重边、不连通、重复 find、20 万点链 | 无向图，结果为顶点编号 | GR、DEEP、REG |
| `template/graph/差分约束.cpp` | 300 小权图及 500 大权图，对照独立 Floyd / i128 Floyd | 负环、极大负权、必须平移的可行解、ll 无法表示解、重复 solve | add 表示 x[u] - x[v] <= w；无法装入 ll 的可行解抛异常 | GR、REG |
| `template/graph/堆优化最短路.cpp` | 250 组多源逐个核对 Floyd | 不可达、零权、自环、重边、大权值固定例 | 边非负；有限路径严格小于 inf，路径加法不溢出 | C |
| `template/graph/最大流.cpp` | 600 组，穷举每边合法流量；最小割树另做割枚举 | 0 容量、自环、重边、反向边索引、二次 flow 只返回增流、init | 容量非负，流量与残量计算不溢出；flow 消耗残量网络 | F、AT、REG |
| `template/graph/有向图欧拉路.cpp` | 300 组小图，按边编号枚举可行欧拉路并验证输出 | 空边集、不连通、自环、重边、20 万边链与自环 | 返回顶点序列，不是边编号；失败为空 | GR、DEEP |
| `template/graph/最小树形图.cpp` | 2000 组、各根枚举入边方案 | 负权收缩环、自环、重边、不可达、单点、LLONG_MAX 单边、重复 solve | 根向外树形图，只返回最优权值；权差与和不溢出 | XGR |
| `template/graph/支配树.cpp` | 1500 组、所有根，逐点删除后核对支配集合及直接支配点 | 不可达、自环、重边、换根、长链 | root 的 idom 为自身，不可达点为 0；深链需较大栈 | DOM |
| `template/graph/树上启发式合并.cpp` | 500 组独立枚举每个子树的不同颜色数 | 随机换根、重复 build、回调状态清空、20 万点链 | add / del 必须互逆，外部统计状态由调用者维护 | DT、DEEP |
| `template/graph/离线动态连通性.cpp` | 2200 组 × 65 次，对照当前重边计数图遍历 | 反向 erase、重复边、自环、多次 solve、无操作 | 离线；erase 的边必须存在，每次删一份 | XDS |
| `template/graph/边双连通分量.cpp` | 300 组删桥后独立重连通分量，另 2000 组桥森林 | 重边、孤立点、不连通、顶点 / 边恰好归属一次、20 万点环 | bcc.e 只含非桥边，bel 与 is_bridge 的下标不同 | GR、AT、DEEP |
| `template/graph/欧拉序最近公共祖先.cpp` | 300 组每个根、全点对，逐步跳父亲作真值 | 单点、反复换根、20 万点链 | 连通树；欧拉序 0-based，顶点 1-based | GR、DEEP |
| `template/graph/全源最短路.cpp` | 2000 组对照 i128 三重循环最短路真值 | 空图、负边、负自环、不可达负环、重边、重复调用 | 初始化对角 min(0, 自环)，inf 范围按源码；负环时距离不可用 | XGR |
| `template/graph/函数图分解与跳跃.cpp` | 2000 组逐步走图，核对环、入环、全点对距离及跳跃 | 空图、单点自环、ULLONG_MAX 步数、20 万点长尾 | 每点恰有一条合法出边；不是一般有向图 | CS、DEEP |
| `template/graph/一般图匹配.cpp` | 2000 组 n <= 12，对照子集匹配枚举并验证匹配方案 | 奇环、空图、单点、重边、自环忽略、重复 solve | 一般无向图，无权最大匹配，O(n^3) | XGR |
| `template/graph/全局最小割.cpp` | 2000 组全部割子集枚举，同时核对返回的一侧 | 空 / 单点、不连通、重边、自环、LLONG_MAX - 1 单边 | 非负边权，总边权严格小于 LLONG_MAX | XGR |
| `template/graph/最小割树.cpp` | 2000 组全点对、每对穷举所有割 | 自环、重边、不连通、同点为 0、重复 build、LLONG_MAX 单边 | 无向非负容量，复制 Dinic；容量总量能放入 ll | AT |
| `template/graph/树链剖分.cpp` | 400 组、3 次换根，父亲扫描核对 LCA、子树区间与路径点集 | 单点、重复 build、20 万点链 | 路径分段不保证方向，不能直接拼非交换操作 | NT |
| `template/graph/分层增广二分图匹配.cpp` | 6000 组子集 DP；另 2000 组核对最小点覆盖 | 空左右部、重边、增边后重算、20 万点长增广结构 | cover 须在 solve 后调用，点覆盖大小等于最大匹配 | NF、XGR |
| `template/graph/重赋权全源最短路.cpp` | 2000 组对照 i128 Floyd | 势能构造负边无负环、任意负权、孤立负环、空图、重复 solve | 允许负边，任何位置负环均返回无解；有限距离遵守 inf | XGR |
| `template/graph/二分图最大权匹配.cpp` | 6000 组枚举左侧全匹配并核对双向方案 | n > m、缺边、全负权、重边取优、ll 极值、失败后清空匹配 | 求覆盖全部左点的最大权匹配，不是可任意留空的带权匹配 | NF |
| `template/graph/最小生成树.cpp` | 300 组对照独立 Prim 的连通性与最优权 | 单点、不连通、负边权、相同权值 | 不连通时返回森林且 ok 为假；累加中间量不溢出 ll | C |
| `template/graph/重构树.cpp` | 2000 组对照 Floyd minimax / maximin | 递增 / 递减重复建树、负权、不连通、自环、重边、20 万点链 | 同点 query 约定为 0；不连通返回 nullopt | AT、DEEP |
| `template/graph/第几短游走.cpp` | 1200 组所有源汇，对照每点最多 k 次出堆的独立枚举 | 零权环、重边、空游走、不可达、近 inf、1 万层双边 DAG | 求游走而不是简单路径；边权非负 | KSW |
| `template/graph/长链剖分与祖先查询.cpp` | 2000 组、更换根，对照逐步跳父亲 | k = 0、深度外返回 0、单点、50 万点链两端换根 | 静态树，k 为合法非负参数 | AT、DEEP |
| `template/graph/倍增最近公共祖先.cpp` | 300 组所有根及全点对，对照父亲跳跃 | 单点、重复 build、20 万点链、多根时哨兵父亲重置 | 连通树，查询前 build | GR、DEEP |
| `template/graph/上下界网络流.cpp` | 1600 组穷举所有合法流量，核对循环流、最小 / 最大流及方案 | 负的源汇净流、强制下界、不可能平衡、重边、自环、大容量 | lo <= hi 且非负；与通常只允许非负源汇流的实现语义不同 | NF |
| `template/graph/上下界最小费用流.cpp` | 25000 组穷举每边流量并核对供需与最优费用 | 强制下界、孤立负环、负自环、重边、无解、重复 solve、ll 大值 | 先复制 MinCostFlow；供需和容量、费用中间量按源码限制 | LBC |
| `template/graph/最大团与最大独立集.cpp` | 1200 组 n <= 16 子集枚举；穷举 1024 个 5 点图 | 最大独立集同时核对、100 点完全图与空图、重复求解 | 最坏指数时间，不能凭小图测试推断任意图性能 | CLIQUE |
| `template/graph/势能最短路费用流.cpp` | 600 组中的非负图 / DAG 穷举流量；上下界费用流补强 | 负边无负环、自环反向索引、重复 flow、重建 | 常规增广入口不支持可达负环；循环费用的语义见源码 | F、LBC、REG |
| `template/graph/换根动态规划与距离和.cpp` | 300 组，对照 Floyd 距离后逐点求和 | 零权、重复调用、20 万点链的封闭公式 | 连通树；距离、子树贡献、换根中间量须在 ll 内 | GR、DEEP |
| `template/graph/容量缩放费用流.cpp` | 600 组任意有向小图，枚举全部流量 | 孤立负环、负自环、无源汇通路、重复 flow、重建 | 返回重新求解的全局最大流最小费用，非普通残量增流语义 | F、REG |
| `template/graph/强连通分量.cpp` | 300 组传递闭包核对双向可达等价类 | 自环、重边、不连通、重复 build、20 万点链 | bel 为强连通分量编号，不把编号当原顶点 | GR、DEEP |
| `template/graph/最小斯坦纳树.cpp` | 2000 组枚举全部边子集 | 空 / 重复终端、零权、不连通、自环、重边、近 inf 权值 | 无向非负权；终端数指数复杂度，返回权值不恢复边集 | MG |
| `template/graph/拓扑排序.cpp` | 1500 组独立传递闭包判环，再验证排列及每条边方向 | 空图、单点自环、重边、DAG、一般有环图、固定菱形例 | 图须保留下标 0；有环返回 nullopt | A、C |
| `template/graph/树的直径.cpp` | 300 组所有点对距离求最大，另核对返回端点 | 单点、零权、20 万点链 | 双扫方法仅用于非负权连通树，不支持一般负权最长路 | GR、DEEP |
| `template/graph/二元可满足性.cpp` | 3500 组穷举所有布尔赋值并验证返回方案 | 空变量、force、同变量子句、矛盾、重复 solve、20 万蕴含点链 | add 的两个文字取值按源码语义，不是直接传图节点 | NT |
| `template/graph/无向图欧拉路.cpp` | 300 组按边编号穷举并验证每边恰用一次 | 自环、重边、空边集、不连通、20 万边链与自环 | 返回顶点序列，递归深度可到边数 | GR、DEEP |
| `template/graph/点双连通分量.cpp` | 1500 组枚举所有点集的连通性、删点性质及极大性；另检查边归属 | 孤立点、单边、重边、不连通、重复 build、20 万点链 | 不含自环；单边与孤立点按源码作为点双 | A、GR、AT、DEEP |
| `template/graph/虚树.cpp` | 400 组闭包枚举所有关键点对 LCA，核对最近祖先边及深度差 | 空 / 重复关键点、换根后清理、重复 query、20 万点链 | 压缩边权为深度差，不自动提供原路径最小边权或应用 DP | NT |
| `template/graph/一般图最大权匹配.cpp` | 20000 组 n <= 12 子集 DP；穷举 59049 个 5 点三态边图 | 奇花历史反例、非正权、重边、ll 边界、500 点结构 | 求最大权匹配，可留空；正权须 <= LLONG_MAX / (8 max(1,n)) | WM |
| `template/graph/零一最短路.cpp` | 2000 组对照独立 Bellman-Ford 式松弛 | 零权环、自环、重边、不可达、单点、重复调用 | 边权必须严格为 0 或 1 | XGR |

## 数学：34 个

| 模板 | 随机真值与规模 | 特殊数据 | 前提 / 剩余风险 | 入口 |
| --- | --- | --- | --- | --- |
| `template/math/自适应辛普森积分.cpp` | **已修复并通过专项复验**：2000 组解析多项式；新增 1000 组仿射变换并叠加三次项，正反向分别对照解析积分 | 原固定 x^4/5 - x^6 误差约 5.04e-15 < eps=1e-10；原失败种子重播；零区间、深度 0、exp、atan | 新增下一层局部误差检查；eps 仍为估计，任意连续函数 / 未解析尖峰振荡及深度耗尽不保证严格精度 | A、R |
| `template/math/最短线性递推.cpp` | 300 组随机递推，直接生成 60 项核对 BM 与 kth | 零序列、空递推、初始项与后续项、重复调用回归 | 质数模数，前缀须充分长才可恢复原递推；k 非负 | R、REG |
| `template/math/线性递推快速求项.cpp` | 2000 组有理生成函数及递推，对照逐项展开 | 空分子、零阶、ULLONG_MAX 项与独立 Fibonacci 倍增、2048 阶 | 复制 FPS；固定 998244353，分母常数非零 | CM |
| `template/math/离散对数.cpp` | 全枚举 mod <= 100；1500 组随机小模数用逐次幂首达位置 | 不互质、a = 0、mod = 1、无解、负数及 ll 极值归一化 | bsgs 要互质，exbsgs 不要求；返回最小非负指数 | B、NM |
| `template/math/组合数.cpp` | 1000 组随机质数 / 上界，独立 Pascal 三角核对 | n = 0、小模数、k 越界、表外 n、重复 init | 质数模数且预处理 n < mod | A、R |
| `template/math/扩展欧几里得与中国剩余定理.cpp` | 2000 组枚举整个 lcm 周期求最小解 | 非互质、冲突、空方程、模数 1、ll 极值、lcm 溢出异常 | 正模数；扩欧 a、b 非负；lcm 须可表示 | NM |
| `template/math/行列式与矩阵树定理.cpp` | 2500 组排列展开行列式；生成树穷举边子集 | 空矩阵、奇异、负值、有向根向外、重边、自环、LLONG_MIN | 质数模数；有向矩阵树计算根向外，不是向根汇聚 | XM |
| `template/math/约数与倍数变换与卷积.cpp` | 2000 组整除枚举、GCD / LCM 数对枚举 | n = 0、合数模数、mod = 1、变换后逆变换、上界截断 | 正整数下标，a[0] = 0；LCM 超过 n 的项不返回 | CM |
| `template/math/杜教筛.cpp` | 3000 次随机询问及随机预处理界，对照 gcd / 试除前缀 | n = 0、limit = 1、重复询问、init 清缓存、表内 / 外 | phi 前缀返回 i128；最大声明规模没有穷尽暴力验证 | XM |
| `template/math/等距拉格朗日插值.cpp` | 2000 组，对照独立 Horner | 采样点命中、空采样、LLONG_MIN / MAX 横坐标、10 万采样点 | 采样为 0..n-1，质数模数且 n < mod | CM |
| `template/math/扩展卢卡斯定理.cpp` | 3000 组，i128 Pascal 三角求组合数再取模 | mod = 1、质数幂 / 合数、非法 k、LLONG_MAX 的 k = 0 / 1 / 2 | 每个质数幂完整预处理，不能承诺任意大 int 模数都省内存 | XM |
| `template/math/快速傅里叶变换与任意模数卷积.cpp` | 300 组整数卷积对照二重循环；DFT 枚举；180 组任意模卷积 | 空、负系数、2 的幂边界、整数极值系数、合数 / 1 模数 | fft_mul 是近似；严格模卷积用 conv_mod，长度及数值范围遵守源码 | NP |
| `template/math/类欧几里得.cpp` | 3000 组逐项 i128 数学下取整 | n = 0、负 a / b、LLONG_MIN / MAX、LLONG_MAX n 的封闭公式 | n >= 0、m > 0，答案和中间量均在 i128 内 | XM |
| `template/math/多项式基本运算.cpp` | 350 组 inv / ln / exp 系数递推；2500 组 pow / sqrt / divmod 朴素运算 | 空、零截断、前导零、无平方根、尾零、2 的幂附近、ULLONG_MAX 幂 | 固定模数；inv / ln / exp 的常数项前提与 NTT 长度限制必须满足 | NP、XM |
| `template/math/快速沃尔什变换.cpp` | 300 组 OR / AND / XOR 双重循环卷积 | 长度 1、零掩码、正逆变换一致 | 长度为 2 的幂；XOR 逆变换需奇质数且长度可逆 | R |
| `template/math/高斯消元.cpp` | 2000 组，模方程枚举全部赋值；实数秩与模秩互查；异或版互查 | 无 / 唯一 / 多解、零行列、近零主元、自由变量、负数与模数上界 | 实数与模秩互查仅适合已测小整数矩阵，不是病态矩阵证明 | NM |
| `template/math/拉格朗日插值.cpp` | 300 组随机多项式、随机求值点，对照 Horner | 单采样点、命中采样点、零系数 | x 在模意义下两两不同；质数模数，系数与输入先归一化 | R |
| `template/math/线性筛积性函数.cpp` | 1000 组随机上界，对照试除、gcd 计数和平方因子分解 | 0、1、反复缩小 / 放大 build、质数与质数幂 | n 非负且表能放入内存 | A、NM |
| `template/math/卢卡斯定理.cpp` | 1000 组随机质数和 n / k，对照 Pascal 三角 | p = 2、n 跨越 p、非法 k、LLONG_MAX 的边缘组合数 | 质数 p，O(p) 表需要内存；不适合拿大质数直接预处理 | A、NM |
| `template/math/矩阵快速幂.cpp` | 1000 组矩形乘法对照 i128 点积，随机方阵幂对照逐次乘法 | 单元素、全 mod-1、指数 0、单位阵 LLONG_MAX 幂、mod = 2^31 | 非空且维度匹配；底数归一化、指数非负 | A、R |
| `template/math/矩阵求逆.cpp` | 2000 组排列行列式判可逆，同时核对左右乘积 | 空方阵、奇异、负元素、小素数、INT_MAX 模数 | 质数模数，输入必须为方阵 | P |
| `template/math/积性函数求和.cpp` | 1600 组逐数试除积性函数；百万 φ 筛表前缀 | n = 0、负系数、二次素数值、合数 / INT_MAX 模数、多次 sum | f(p) 限二次多项式，回调 f(p^e) 必须一致；未端到端测试 10^12 上界 | BM |
| `template/math/快速幂与逆元.cpp` | 随机幂对照逐次乘法；随机批量逆元逐项验乘积 | mod = 1、负底数、LLONG_MIN、LLONG_MAX 模数、空批量 | 幂指数非负；线性 / 批量逆元仍要求质数及非零元素 | P |
| `template/math/二次剩余.cpp` | 3000 组枚举全部根；大质数随机平方回推 | p = 2、a = 0、非剩余、负 a、LLONG_MIN、INT_MAX p | 仅质数；返回较小非负根，不检测合数前提 | XM |
| `template/math/多项式多点求值与快速插值.cpp` | 求值与插值各 2000 组，对照 Horner / 朴素拉格朗日系数 | 重复求值点、空数组、零多项式、重复 build、4096 点 | 复制 FPS；插值点必须互异，固定 998244353 | AP |
| `template/math/快速数论变换与多项式乘法.cpp` | 300 组，对照逐系数二重循环卷积 | 空乘法、长度 1、多种 2 的幂长度；斯特林另触发 4096 项规模 | 固定 998244353；直接 ntt 长度为非零 2 的幂，至多 2^23 | R、CM |
| `template/math/素性测试与整数分解.cpp` | 1500 组 < 10^6 对照独立试除；另 50 个随机 ull 核对分解乘积 | 0 / 1、Carmichael / 伪素数、2^63、ULLONG_MAX、大素数 / 半素数 | 小数试除是真独立真值；全 ull 随机乘积检查还依赖确定性判素 | B、NM |
| `template/math/线性筛素数.cpp` | 1000 组随机上界，对照逐数试除生成质数表 | 0、1、单质数、重复调用、稠密合数段 | n 非负，表规模受内存限制 | A、R |
| `template/math/原根.cpp` | 1500 组随机小素数，枚举非零剩余类；另穷举小素数最小性 | p = 2、四个大素数至 ull 上界附近 | p 必须为质数，依赖 PollardRho；大素数验证依赖因子分解结果 | B、P |
| `template/math/整除分块.cpp` | 1000 组随机 n，逐元素核对区间商及最大右端点 | 0 / 1 / 2、连续性、末段、10^12 回归 | 返回整个区间数组；LLONG_MAX 规模没有完整运行，可能需数十亿段 | A、R、REG |
| `template/math/单纯形与线性规划.cpp` | 1200 组有界 LP，枚举顶点并用独立 Gauss 核对目标与可行性 | 无解、无界、零变量 / 约束、负 RHS、Beale 退化循环例 | 实数容差、病态尺度仍有风险；随机主体为有界 LP，无界主要为固定例 | BM |
| `template/math/两类斯特林数.cpp` | 2000 组计数多项式恒等式；n <= 8 穷举排列环 / 集合划分 | n = 0、mod = 1 / 合数、4096 项卷积行 | 第一类为无符号；递推支持合数，卷积行固定 998244353 | CM |
| `template/math/子集卷积.cpp` | 2000 组枚举所有子掩码 | n = 0、负系数、INT_MIN / MAX、合数、mod = 1 | 长度为 2^n，n <= 20；满规模内存约 180 MiB 未逐项压力验证 | AP |
| `template/math/异或线性基.cpp` | 300 + 2500 组枚举可表示异或集合，核对 contains / max / kth | 零与相关向量、空基、bit 62、满 63 维、k 越界 | 仅处理 bit 0..62，不能当作支持 bit 63 的 ull 线性基 | R、XM |

## 字符串：14 个

| 模板 | 随机真值与规模 | 特殊数据 | 前提 / 剩余风险 | 入口 |
| --- | --- | --- | --- | --- |
| `template/string/多模式匹配自动机.cpp` | 300 组、多批模式，对照逐模式子串枚举 | 重复模式、重叠、空文本、无模式、insert 后重复 build、clear | 只支持小写字母；模式串非空，新增后必须 build | R、B、REG |
| `template/string/位集优化最长公共子序列.cpp` | 2000 组，对照普通 O(nm) DP | 空串、全部 256 字节、63 / 64 / 65 / 127 / 128 / 129 块边界 | 只返回长度；长串时间与内存仍按源码估计 | XO |
| `template/string/字符串哈希.cpp` | 500 任意字节串与直接 LCP；新增 5000 次直接多项式哈希 | 空查询、NUL / 高字节、全等串、跨对象、重复 build | 双模仍可能碰撞；相等时同时核对子串长度 | NS、A |
| `template/string/二维哈希.cpp` | 2000 组 × 30 次，对照逐行列直接多项式哈希 | 空矩阵、零宽矩阵、任意字节、空矩形、跨对象 | 输入等宽；跨对象比较还要核对尺寸，存在碰撞 | CS |
| `template/string/字符串匹配.cpp` | 1000 组逐起点 compare 作真值 | 空文本 / 模式、重叠、周期串、全异、NUL / 高字节固定例 | 空模式返回所有 n + 1 个位置，调用者须认同该约定 | C、B |
| `template/string/林登分解.cpp` | 1500 任意字节串；长度 <= 9 的所有三元串 | 空、全等、周期、10 万全等 | 因子是字典序不增，按 unsigned char 比较 | BS |
| `template/string/最长回文子串.cpp` | 500 组、全子串逐字符反转判回文 | 空、单字符、全等、奇偶回文、周期、NUL / 高字节、空区间 | query 闭区间；longest 返回起点和长度 | C、B |
| `template/string/最小表示法.cpp` | 穷举二元串至长度 8；500 任意字节串枚举全部旋转 | 空串、相同最优位置取最早、全等、周期、NUL / 高字节 | 返回起点不是旋转后的字符串；字节按无符号顺序 | NS |
| `template/string/回文自动机.cpp` | 穷举二元串至长度 8；500 小写串，逐前缀枚举回文频次 | 虚根计数、重复 count、2000 全等串、build 清空、再插入 | 仅小写字母；两个虚根不是实际回文种类 | NS |
| `template/string/极大周期子串.cpp` | 全三元串至长度 9；1500 任意字节串，逐子串枚举最小周期和极大性 | 空、全等、周期、10 万全等串 | 复制 SuffixArray，输出半开区间，period 是最小周期 | BS |
| `template/string/后缀自动机.cpp` | 300 组枚举所有子串及频次，核对状态 count 与 query | 空串、全等、全异、重复 count / build、clone 场景 | 小写字母；query 是重复子串的最大长度乘出现次数 | R、B |
| `template/string/后缀数组.cpp` | 500 组后缀排序与全点对直接 LCP；runs 间接覆盖任意字节 | 空串、全等、周期、NUL / 高字节固定排序、重复 build | 后缀位置合法；没有空后缀索引 n | SA、B、BS |
| `template/string/字典树.cpp` | 300 组，对照完整模式列表的精确计数与前缀计数 | 空串、重复插入、缺失串、空前缀、clear | 小写字母；此模板不提供删除操作 | R、B |
| `template/string/扩展字符串匹配.cpp` | 穷举二元串至长度 8；500 任意字节串直接 LCP | 空模式、空文本、2000 全等、NUL / 高字节 | z[0] = n；exkmp 的输出长度等于文本长度 | NS |

## 其他算法：18 个

| 模板 | 随机真值与规模 | 特殊数据 | 前提 / 剩余风险 | 入口 |
| --- | --- | --- | --- | --- |
| `template/others/笛卡尔树.cpp` | 300 随机数组，核对中序为原顺序、堆性质和父子关系 | 空、单点、全等、递增 / 递减、ll 极值、重复 build | 小根堆，相等值保留较早节点；1-based 占位数组 | R、B |
| `template/others/分治三维偏序.cpp` | 1800 组三维点，二重枚举所有其他支配点 | 重复点全相互计数、空集、100 个全等、ll 坐标极值 | 非严格 <= 偏序，排除自己；输入不变 | NO |
| `template/others/常用定义.cpp` | 1000 组直接 min / max / 模运算核对返回值 | 相等不更新、加法进位、减法借位、模数 1 | add / sub 已归一化且不能溢出 T；本次测试为有符号整数 | A、R |
| `template/others/单调凸包与斜率优化.cpp` | 3000 组逐线枚举；P3195 型 DP 对照 O(n²) | 同斜率、同查询点、负截距、10^18 量级、5 万次 DP | 斜率不增、x 不减；交叉积须在 i128 内 | P |
| `template/others/离散化.cpp` | 2000 组，对照有序集合排名 | 空、重复、负数、LLONG_MIN / MAX、不修改输入 | 返回编号 1-based，数组位置 0-based | R |
| `template/others/舞蹈链精确覆盖.cpp` | 2500 组矩阵，枚举全部行子集，另逐字段快照检查恢复 | 0 列、空行、无解、Knuth 例、成功 / 失败后重复 solve、追加行 | 行 id 唯一；行内列不重复且合法；指数搜索 | DLX |
| `template/others/分治动态规划与单调队列多重背包.cpp` | 分治 DP 与多重背包各 2000 组，对照普通转移枚举 | 不可达层、零容量、物品超容、数量 0、负价值、精确 / 至多体积 | 分治要求最左最优决策单调；背包 w > 0、count >= 0 | XO |
| `template/others/最长上升子序列与方案.cpp` | 2000 组 O(n²) DP，核对长度与返回下标方案 | 空、重复、严格 / 非严格、负数、ll 极值 | 返回一组方案，不保证特定字典序 | CS |
| `template/others/莫队.cpp` | 300 组 × 50 查询，区间和直接扫描 | 同点 / 重复区间、无查询、n = 0 的空操作 | 回调接收下标，需自备初始空状态 | R、B |
| `template/others/带修改莫队.cpp` | 2000 组混合操作，对照每次修改后的 set 区间种类数 | 无操作、无查询、同值修改、重复 solve | 回调接收值，不是下标；每次 solve 前重置外部状态 | XO |
| `template/others/单调栈与左右最近更小与更大.cpp` | 3000 组对照双向逐点扫描 | 空、重复值、负数、LLONG_MIN / MAX、不存在返回哨兵 | 严格更大 / 更小，相等不算 | P |
| `template/others/树上莫队.cpp` | 2000 组独立 DFS 找路径并用 set 统计 | 同点、祖先与非祖先路径、换根后重建、重复 solve、20 万点链 | 静态树点统计，不支持带修改树上莫队 | XO、DEEP |
| `template/others/整体二分与动态区间第几小.cpp` | 1600 组带修改操作，对照当前区间排序 | 空查询、全等、重复修改、ll 极值、查询后继续追加、重复 solve | 离线，k 必须在区间元素个数内 | NO |
| `template/others/博弈状态函数.cpp` | 2000 组 DAG，对照双子游戏胜负 DP 和逐点 mex | 终止点、重复边、自环 / 非平凡环拒绝、20 万点链 | 有限无环正常规则公平游戏，不适用 misère 或有环博弈 | CS、DEEP |
| `template/others/滑动窗口最值.cpp` | 300 随机数组和窗口，直接扫描最值 | 固定全等 / 单调 / 单点 / ll 极值数组枚举全部合法 k | 1 <= k <= n；无合法窗口的空数组不作为允许调用域 | R、B |
| `template/others/稀疏表.cpp` | 300 随机数组枚举全部闭区间，对照 max_element | 单点、全等、递增 / 递减、负数与 ll 极值、重复 build | 静态最大值，不支持更新或一般非幂等运算 | R、B |
| `template/others/三分搜索.cpp` | 2000 组已知顶点的随机二次函数，极值为顶点投影到区间 | 极值在端点、零宽区间、最小 / 最大版、常数函数固定例 | 只用于对应单峰 / 单谷；返回位置近似，不保证任意精度 | A、R |
| `template/others/树哈希.cpp` | 2000 组，对照排序括号编码并验证换编号 / 打乱邻接不变性 | 单点、不同根、重复结构、20 万点链两端对称 | 有根树哈希，不能直接用于无根树判同构；仍有碰撞 | R、DEEP |

## 计算几何：13 个

| 模板 | 随机真值与规模 | 特殊数据 | 前提 / 剩余风险 | 入口 |
| --- | --- | --- | --- | --- |
| `template/geometry/圆.cpp` | 800 组核对交点方程、切线垂直 / 距离条件及对称性 | 相离、相切、包含、重合、零半径、共线三点、两圆交面积解析式 | 随机部分主要验证输出合法性，完整性还靠解析边界；浮点 EPS | NG |
| `template/geometry/圆面积并与周长并.cpp` | 两原始种子共 200 组，对照独立竖切片积分与圆周采样；再扰动种子 | 重复、包含、内外相切、洞、小圆、近重合、打乱顺序 / 平移 | 数值采样不是严格误差证明，近退化拓扑仍受浮点限制 | CU |
| `template/geometry/平面最近点对.cpp` | 2000 组直接枚举所有点对 | 空、单点、重复、共线、竖线、打乱顺序、重复 build | 少于两点返回 {-1,-1}，d2 为无穷；坐标及平方需可表示 | XO |
| `template/geometry/凸包与直径与闵可夫斯基和.cpp` | Jarvis 独立包裹法；500 组直径点对枚举；闵和枚举所有点对和 | 全共线、重复、近 EPS、顺 / 逆时针、循环起点、顺时针直径旧反例 | 直径输入凸边界无重复邻点；checkConvexHull 不检测自交 | GEO |
| `template/geometry/三维凸包.cpp` | 1500 整数点集 × 3 顺序，对照整数支撑面枚举及面内凸包 | 立方体、四面体、重复、共面、共线、闭合性与朝向 | dim < 3 返回空面，area = volume = 0；近退化数据无精确拓扑保证 | H3 |
| `template/geometry/凸多边形包含查询.cpp` | 2000 组 × 40 查询，对照线性 contain，边界另直接验 | 空、单点、线段、重复、共线、顶点 / 边中点、重复 build | build 接收任意点集后取凸包；与 Polygon 共用浮点基础 | XO |
| `template/geometry/半平面交.cpp` | 1500 组，对照独立逐半平面裁剪及面积 / 可行性 | 重复平行边、冲突、线 / 点退化、signed zero、0 / 2π 附近方向 | 无界 / 无解 / 零面积均返回空，不能据空结果区分原因 | NG |
| `template/geometry/直线与线段.cpp` | 5000 组整数方向谓词；投影方程、独立参数最近点及交点方程 | 端点相触、点线段、共线、交叉、平行、退化线段最近距离 | isLL 要非平行，proj / Line 构造要求不同端点；浮点域不保证精确 | A、GEO |
| `template/geometry/最小圆覆盖.cpp` | 700 组枚举所有单点、两点及三点候选圆 | 空、重复、共线、单点、零半径 | 随机增量期望 O(n)，数值覆盖性用容差检查 | NG |
| `template/geometry/最小面积包围矩形.cpp` | 两原始种子共 6000 组及变换，对照逐边方向枚举 | 空、点、线段、旋转 / 缩放 / 平移、循环起点、10 万点规则凸包 | 必须严格逆时针凸包，不支持任意点序；浮点精度依尺度 | MR |
| `template/geometry/点与向量.cpp` | 5000 组独立整数叉积 / 点积 / hypot；排序三元组性质检查 | 零向量、轴向、近 EPS 排序反例、负坐标、90° 旋转 | unit 不接受零向量；默认构造不初始化坐标；EPS 相等不是严格等价关系 | A、GEO |
| `template/geometry/多边形.cpp` | 5000 组随机矩形及裁切解析面积；凸包点包含关系 | 空 / 点 / 线、有向面积、顺逆时针、首尾重复、1e9 平移小方形、凹多边形 | contain 输入简单边界多边形，convexCut 需凸；大坐标不能恢复已丢失输入精度 | A、GEO |
| `template/geometry/矩形面积并与周长并.cpp` | 2500 组，对照整数网格面积和边界计数 | 共享边、重复、退化、乱序端点、ll 极值平移、10^9 边长 | 坐标差与面积 / 周长及中间量不溢出 ll | XDS |

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

`snippets_smoke.cpp` 另作接口集成烟测，不能替代上面的独立随机真值。

## 复跑

在 Linux 下执行：

```sh
bash support/verify/run_linux.sh
python3 support/verify/audit_reseed.py --salt 2026101202
python3 support/verify/audit_reseed.py --salt 2026101204
python3 support/verify/check_sources.py
python3 support/verify/run_public.py
```

公开数据复跑依赖已有固定版本 `build/library-checker-problems/`，无需重新联网取题面。单独重跑补测也必须保留 ASan / UBSan、标准库断言及大栈设置。

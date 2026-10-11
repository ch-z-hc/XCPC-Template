# Linux 对拍

## C++ 对拍程序

将本目录的 `stress.cpp` 复制到专用题目目录，另外准备三个完整程序：

- `gen.cpp`：生成器，从 `argv[1]` 读取随机种子，向 `cout` 输出一组合法输入。
- `brute.cpp`：小数据暴力。
- `a.cpp`：待测代码。

```sh
g++ -std=c++17 -O2 gen.cpp -o gen
g++ -std=c++17 -O2 brute.cpp -o brute
g++ -std=c++17 -O2 a.cpp -o a
g++ -std=c++17 -O2 stress.cpp -o stress
ulimit -s 262144
./stress
```

对拍程序持续运行，直到答案不同、程序异常退出或手动中断。程序没有内置限时；某个程序死循环时需手动停止。深递归前可在当前终端用 `ulimit -s 262144` 将栈设为 256 MiB。

通过 `system("diff -wB brute.out a.out")` 比较答案，差异直接显示在终端。忽略行内空白和空行，不忽略非空行的分行差异；浮点题、多解题需要另写判定。答案不同或比较命令失败返回 1，生成器、暴力或待测程序异常退出返回 2。

终端中的 `Test t` 表示当轮种子，失败后 `input.txt` 是输入，`brute.out` / `a.out` 是两份答案。异常退出时可能没有完整输出。修复后重新编译 `a.cpp`，先用原输入复测：

```sh
g++ -std=c++17 -O2 a.cpp -o a
./a < input.txt > a.out
diff -wB brute.out a.out
```

若两份输出分行方式不同，`diff` 可能报告差异，此时按内容逐项核对。也可用 `./gen 123 > input.txt` 单独复现种子 123，再运行暴力和待测程序。再次运行对拍会覆盖原输入和输出，先保存失败数据。

下面的 6 组示例均可配合 C++ 版使用：将对应的 `generator.cpp`、`brute.cpp` 分别复制为 `gen.cpp`、`brute.cpp`，自己的完整解题代码保存为 `a.cpp`。各组的 `standard.cpp` 用于仓库验证，包含相对路径，不要单独复制到其他目录。

打印册直接收录 `stress.cpp`，不再使用 Bash 循环。

## Python 版

在仓库根目录执行，需要 g++ 和 Python 3：

```sh
# 最大子段和，种子从 1 开始，跑 1000 轮。
python3 support/tools/stress/run.py --tests 1000 --seed 1

# 区间加、区间和。
python3 support/tools/stress/run.py --case range_sum --tests 1000

# 用自己的 a.cpp 替换区间操作示例中的待测程序。
python3 support/tools/stress/run.py --case range_sum --standard a.cpp

# 持续对拍，Ctrl+C 停止；每个程序每轮最多 2 秒。
python3 support/tools/stress/run.py --tests 0 --timeout 2

# 检查越界、非法内存访问和未定义行为；调试版放宽时限。
python3 support/tools/stress/run.py --case tree_path --sanitize --timeout 10
```

脚本自动编译，每轮依次运行生成器、暴力和待测程序。Linux 下将子进程的栈设为 256 MiB；若系统硬限制更小，则遵守硬限制。Windows 普通编译也可使用同一脚本，将命令中的 python3 换成 python。

默认按空白分隔后的 token 精确比较，忽略空格、空行和换行方式，但不把 1 与 1.0 当成同一个 token。调试信息写到 cerr，不要写进答案。

## 已准备的示例

每组都有 generator.cpp、brute.cpp、standard.cpp。最大子段和在当前目录，其余放在 cases/ 对应子目录。standard.cpp 是可运行的参考程序，新增示例直接调用仓库算法模板；使用时可用 --standard 换成自己的完整代码。

| --case | 内容 | 暴力方式 | 重点边界 |
| --- | --- | --- | --- |
| max_subarray | 非空最大子段和 | 枚举左右端点，O(n^2) | 单点、正负数 |
| range_sum | 区间加、区间和 | 数组逐项修改 / 求和，O(nq) | 单点、全区间、负数、全零 |
| inversions | 1..n 排列的逆序对数 | 枚举下标对，O(n^2) | 有序、逆序、交换一对 |
| tree_path | 无权树两点距离 | 每个询问 BFS，O(nq) | 链、菊花树、同一点、随机重编号和根 |
| shortest_path | 有向非负权单源最短路 | Bellman-Ford，O(nm) | 不连通、重边、自环、零权、大权 |
| kmp | 所有匹配起点 | 每个起点直接比较 | 空串、空模式串、重叠匹配、空格 |

例如：

```sh
python3 support/tools/stress/run.py --case inversions --tests 3000
python3 support/tools/stress/run.py --case tree_path --tests 3000
python3 support/tools/stress/run.py --case shortest_path --tests 3000
python3 support/tools/stress/run.py --case kmp --tests 3000
```

这些是小数据对拍，不是大数据性能测试。范围和特殊形态在生成器里直接修改；生成器接收命令行种子，用 mt19937_64 生成随机数，相同可执行文件和种子得到相同输入，不用当前时间重新播种。不同标准库的随机分布实现可能不同，跨环境复现以保存的输入为准。

输入输出约定：

- range_sum：n q，数组；0 l r v 表示区间加，1 l r 表示求和，均为 1-based 闭区间。
- inversions：n 和 1..n 排列，输出逆序对数；普通数组需要先离散化再改参考程序。
- tree_path：n q root，n-1 条无向边，q 组 u v，输出路径边数。
- shortest_path：n m s，m 条 u v w 有向边，输出各点距离，不可达为 -1；不生成负权。
- kmp：两行字符串，用 getline 读取；输出匹配数及所有 0-based 起点，空模式串匹配每个边界。

## 给自己的题对拍

准备三个完整程序：

- gen.cpp：向 stdout 输出一组合法输入；随机种子从 argv[1] 读取。
- brute.cpp：只求小数据正确，优先枚举、搜索或普通 DP。
- a.cpp：待测代码。

```sh
python3 support/tools/stress/run.py \
    --generator gen.cpp --brute brute.cpp --standard a.cpp \
    --tests 10000 --seed 1 --timeout 2
```

路径可以是绝对路径；路径有空格时加引号。也可以只覆盖某一个程序，其余仍使用 --case 指定的示例。

三个程序必须采用相同的输入输出格式。生成器要保证题目约束，例如树连通、删除的元素存在、图边权满足算法要求。先用很小的范围跑暴力，确认流程后再扩大；多测题也要生成并读取相同的 t。

## 错误现场与复现

一旦答案不同、程序异常退出或超时，脚本立即停止。每次失败使用独立目录，不覆盖之前的失败数据：

```text
build/stress/<case>/failure-<seed>-<随机后缀>/
    failure.in       当轮输入；生成器失败时可能只是部分输入
    generator.out    生成器输出
    generator.err    生成器错误日志
    brute.out        暴力输出
    brute.err        暴力错误日志
    standard.out     待测程序输出
    standard.err     待测程序错误日志
    failure.json     种子、原因、返回码、源码路径和复现命令
```

未运行的程序没有对应输出文件。终端会打印现场路径和复现命令；应在原工作目录运行，并保持源码不变。例如只重跑种子 123：

```sh
python3 support/tools/stress/run.py --case range_sum --seed 123 --tests 1

# 不再依赖生成器，直接用保存的输入检查。
g++ -std=c++17 -O2 -Wall -Wextra a.cpp -o a
./a < build/stress/range_sum/failure-123-xxxx/failure.in > a.out
```

将 xxxx 换成实际目录后缀。失败后修复代码，先复跑该组输入，再恢复连续随机测试。

退出码：0 全部通过，1 答案不同，2 编译 / 运行错误或超时，130 手动中断。不要把暴力超时误认为待测程序有错，也不要把错误程序和暴力碰巧输出一致当成通过。

## 不用 Python 的 Bash 版本

run.sh 使用 Bash、g++、GNU timeout 和 diff。默认运行最大子段和，持续到失败或 Ctrl+C；每个程序限时 2 秒，超时后最多再等 1 秒强制终止：

```sh
bash support/tools/stress/run.sh

# 有限轮数；TESTS=0 表示持续运行。
TESTS=1000 bash support/tools/stress/run.sh

# 传入自己的三个源文件；在独立目录编译，不覆盖现有 a 等文件。
bash support/tools/stress/run.sh gen.cpp brute.cpp a.cpp
```

工作目录会打印在终端，输入、两份输出、stderr、差异和 seed.txt 都留在其中。Bash 版本使用 diff -wB，忽略行内空白和空行，但仍要求非空行的分行方式一致；需要忽略换行差异时用 Python 版本。Bash 版没有浮点判定。

只想临时手写对拍循环，可以在题目目录中编译后运行：

```sh
g++ -std=c++17 -O2 gen.cpp -o gen
g++ -std=c++17 -O2 brute.cpp -o brute
g++ -std=c++17 -O2 a.cpp -o a
ulimit -s 262144

for ((seed=1; ; ++seed)); do
    timeout -k 1s 2s ./gen "$seed" > input.txt || break
    timeout -k 1s 2s ./brute < input.txt > brute.out || break
    timeout -k 1s 2s ./a < input.txt > a.out || break
    diff -wB brute.out a.out || break
    echo "OK $seed"
done
echo "最后种子：$seed"
```

这段短循环会覆盖当前目录的 gen、brute、a、input.txt 和两份输出，只在专用题目目录中使用；要保留多次失败现场，用上面的脚本。

## 浮点与多解题

```sh
python3 support/tools/stress/run.py \
    --generator gen.cpp --brute brute.cpp --standard a.cpp --eps 1e-6
```

浮点模式按 token 对齐，对数值使用绝对 / 相对误差 1e-6，其他文字仍精确比较；NaN、无穷大视为失败。整数、大整数题保持默认精确比较，不使用 --eps。

构造题、输出路径 / 匹配方案等多解题，合法答案可能不同，不能直接 diff。应写检查器分别验证合法性及目标值，按题意修改脚本中的 same_output；单独放宽浮点误差不能解决多解问题。

## 检查对拍工具本身

```sh
python3 support/verify/stress_tools_check.py

# 仅检查 C++ 对拍程序（Linux）。
python3 support/verify/stress_tools_check.py --cpp-only
```

检查 6 组示例各 200 轮、种子复现、输出比较、错误答案 / 崩溃 / 超时的现场保存，以及 Bash 版的成功和失败路径。Linux 下另测 C++ 版：6 组示例各 20 轮，以及答案不同、生成器 / 暴力 / 待测程序异常退出和缺失可执行文件。

# 纯 C++17 对拍

将本目录的 `gen.cpp`、`sol.cpp`、`bf.cpp`、`stress.cpp` 复制到专用题目目录，在该目录编译、运行。示例生成长度为 1..10、元素为 1..100 的数组，两份程序均求数组和。比赛时只替换前三份源码，对拍器尽量不改；全部使用正常的 `cin` / `cout`，不使用 `freopen`。

## Linux

```sh
g++ -std=c++17 -O2 gen.cpp -o gen
g++ -std=c++17 -O2 sol.cpp -o sol
g++ -std=c++17 -O2 bf.cpp -o bf
g++ -std=c++17 -O2 stress.cpp -o stress
./stress
```

## Windows（MinGW g++）

PowerShell / CMD 编译命令相同：

```text
g++ -std=c++17 -O2 gen.cpp -o gen.exe
g++ -std=c++17 -O2 sol.cpp -o sol.exe
g++ -std=c++17 -O2 bf.cpp -o bf.exe
g++ -std=c++17 -O2 stress.cpp -o stress.exe
```

PowerShell 运行 `./stress.exe` 或 `.\stress.exe`；CMD 运行 `stress.exe`。需要 g++ 在 PATH 中，不依赖 Python、`diff`、`fc` 或第三方库。

## 失败与复现

第 i 轮依次运行 `gen i`、`sol`、`bf`，用系统重定向传递输入输出。每 100 轮显示 `AC 100`、`AC 200` 等。答案不同显示 `WA seed=<i>` 并返回 1；程序返回非零或文件错误显示 `RE`、程序名或文件名及种子，并返回 2。

- `in.txt`：生成器的输入数据。
- `out.txt`：待测程序 `sol` 的输出。
- `ans.txt`：暴力 / 标准程序 `bf` 的输出。

失败时不清理文件；异常退出时对应输出可能不完整，未运行程序的输出可能仍是上一轮内容。再次运行或复现会覆盖文件，先保存有用的失败现场。

复现种子 347：

```sh
# Linux
./gen 347 > in.txt
./sol < in.txt > out.txt
./bf < in.txt > ans.txt
```

```powershell
# PowerShell
.\gen.exe 347 > in.txt
# 旧版 PowerShell 重定向可能改变编码；用 CMD 保留原始字节：
cmd /c "gen.exe 347 > in.txt"
cmd /c "sol.exe < in.txt > out.txt"
cmd /c "bf.exe < in.txt > ans.txt"
```

CMD 可直接执行 `gen.exe 347 > in.txt`，以及 `sol.exe < in.txt > out.txt`、`bf.exe < in.txt > ans.txt`。对拍器内部的 `system()` 在 Windows 下使用 CMD 重定向，不经过 PowerShell 的编码转换。

生成器从命令行读取整数种子，以 `mt19937 rng(seed)` 初始化；相同可执行文件、相同种子生成相同数据。不同标准库的 `uniform_int_distribution` 实现可能不同，跨环境复现以保存的 `in.txt` 为准。

## 比较与限制

比较器按空白分隔逐 token 精确比较，忽略空格、空行和换行差异，但不会漏掉任一边多出的 token；`1` 与 `1.0` 不相同。默认仅适合精确答案，需要浮点容差或允许多解时修改比较逻辑。

简单的 `system()` 版本无法可靠处理超时；死循环需手动中断（Ctrl+C）。深递归在 Linux 运行前可执行 `ulimit -s 262144`，将栈设为 256 MiB。

原有 `generator.cpp` / `standard.cpp` / `brute.cpp`、`cases/` 中的 6 组题型示例及 `run.py` / `run.sh` 保留，不参与这套纯 C++ 流程。若使用旧示例，将对应源码分别编译成 `gen`、`sol`、`bf`（Windows 加 `.exe`）；引用仓库模板的 `standard.cpp` 需在原位置编译。

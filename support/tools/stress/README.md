# 场上对拍

把这道题的三份程序放在同一目录。用 `sol.cpp` 写待提交的正解，用 `bf.cpp` 按题意写小数据暴力。`gen.cpp` 每次运行生成一组合法输入。例如拍逆序对时，取数组长度 1 到 8、元素值 1 到 3，很容易造出能手算的重复值反例。

以下命令适用于 Linux / WSL 的 Bash 和 Windows 的 Git Bash。

```bash
g++ -std=c++17 -O2 gen.cpp -o gen
g++ -std=c++17 -O2 sol.cpp -o sol
g++ -std=c++17 -O2 bf.cpp -o bf
```

编译成功后，在[对拍条目](https://chzhc.cn/xcpc/#support-tools-stress-pai)复制当前终端对应的一行。Windows 的 PowerShell 命令调用默认安装位置的 Git Bash，运行 MinGW 生成的 `.exe`。Linux、WSL 和已打开的 Git Bash 都可以直接用 Bash 那一行。

每组开始前显示编号，从 1 开始。输出一致时继续，发现差异时 `diff` 显示暴力答案（`<`）和正解输出（`>`）并停止。用 `cat in` 看反例，修正并重新编译后用 `./sol < in` 重跑这组数据，再开始下一轮对拍。

`diff -Z` 按行比较，忽略行尾空白，适合输出格式一致、答案唯一且可精确比较的题。浮点误差和多解题需要按题意判定。程序异常退出时脚本也会停止，后续步骤的文件可能还是上一轮的。遇到死循环可按 Ctrl+C 中断。重新启动会覆盖 `in`、`ans` 和 `out`。

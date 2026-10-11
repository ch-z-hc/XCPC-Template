---
name: chzhc-xcpc-style
description: 按 chzhc 的个人码风编写、重写或检查 C++17 算法竞赛代码。用于 CCPC、ICPC、XCPC、OI 模板和竞赛解题，或用户要求“按我的码风”“按 chzhc 风格”时；不将这套竞赛习惯强加给业务代码、其他语言或仅需算法讲解的任务。
---

# chzhc 的 XCPC 码风

保留用户熟悉的竞赛写法：短命名、适度压行、直接可改。代码应像本人手写的板子，不改成 ACL、jiangly 或工程库风格。新要求和所在项目约定优先；技能只决定写法，不扩大修改范围。

## 基础写法

- 使用 C++17、`#include <bits/stdc++.h>` 和 `using namespace std;`。
- 使用 `#define V vector`、`typedef long long ll;`，需要浮点时使用 `typedef long double ld;`。
- 比赛起手式可保留 `fi`、`se`、`pii`、`chkmin`、`chkmax`；独立算法片段只带实际需要的定义。默认不添加 `debug` 宏。
- `pii` 固定表示 `pair < int, int >`；其他组合显式写类型，不在不同文件里悄悄改变同一个宏的含义。
- 不换成全套 `std::` 前缀或 `i64`，不使用 `#define int long long`。
- 模数按题目明确指定，不因为示例中出现过某个模数就固定所有算法的模数。

## 排版

- 4 空格缩进，左大括号不另起一行，使用 `} else {`。
- 模板类型两侧留空格：`V < int >`、`V < V < int > >`、`pair < int, ll >`、`template < class T >`。
- 循环和一元运算写成 `++ i`、`-- i`、`ans ++`、`q --`、`! op[x]`。
- 二元运算符两侧留空格；保持 `p << 1 | 1`、`i & -i` 这类常用写法。
- 简单分支可以省略大括号。简短辅助函数可以写成一行。
- 相关的简单语句可以适度压行，例如 `int n; cin >> n;`、`u = find(u); v = find(v);`、`auto [d, u] = q.top(); q.pop();`。
- 多步骤初始化、整段循环和复杂条件必须展开；不要把整个算法压成几个长行。
- 注释和说明用简洁中文，中文与英文、数字之间留空格。

## 命名与状态

- 结构体采用常见算法名：`DSU`、`Fenwick`、`SegTree`、`Dinic`、`LCA`、`SCC`、`SAM`、`Trie`。
- 函数采用简短明确的名称：`init`、`find`、`merge`、`add_edge`、`build`、`modify`、`query`；需要区分语义时保留明确的小写下划线名称。
- 内部采用熟悉的竞赛命名：`fa`、`siz`、`dep`、`dis`、`g`、`q`、`cnt`、`ans`、`u`、`v`、`l`、`r`、`mid`。不要机械缩短所有变量，避免不同概念重名。
- 有状态的数据结构、复杂图算法用 `struct`；简单算法用函数。不添加无用的 `private`、访问器、继承或策略层。
- 默认用按规模分配的 `vector`；固定数组、内存池有实际需要时可用。不要从样例推导“禁止全局数组”。
- 多测通过 `init`、`clear` 或可重复 `build` 重置状态。
- 结构体内部递归用成员函数；题目局部状态适合时使用递归 lambda。不要强制所有 DFS 都改成 lambda。

## 下标与线段树

- 图、树和普通数组默认 1-based，区间默认闭区间 `[l, r]`。
- 字符串默认 0-based；其他例外在文件头说明，不强制转换 STL 容器的自然下标。
- 线段树明确采用用户选择的 A 写法：直接维护 `sum`、`lazy` 或具体节点字段。
- 不引入 `Info` / `Tag` 泛型骨架。复杂维护直接调整节点信息、`pull`、`apply`、`push`，不为“通用”增加抽象。
- 常用线段树对外采用 `init(a)`、`modify(l, r, v)`、`query(l, r)`。

## 区分解题代码与算法片段

- 完整解题代码允许 `solve()`、`main()` 和题目输入输出，通常使用 `ios::sync_with_stdio(0); cin.tie(0);`，输出换行用 `'\n'`。
- 算法库片段不带 `main()`、题目专属 `solve()` 或输入输出，不依赖自定义头文件。
- 比赛起手式、验证程序和工具程序可以有 `main()`，按项目规则单独存放。
- 文件头简要注明适用条件、复杂度、下标、必要的模数与数值限制；文件尾附最小调用示例。
- 不因风格改写而偷偷改变算法能力、边界行为或复杂度；公共接口变化同步修改调用、测试和索引。

## 写法示例

```cpp
struct DSU {
    V < int > fa, siz;

    DSU(int n = 0) { init(n); }

    void init(int n) {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int u) {
        if (fa[u] == u) return u;
        return fa[u] = find(fa[u]);
    }

    bool merge(int u, int v) {
        u = find(u); v = find(v);
        if (u == v) return 0;
        if (siz[u] < siz[v]) swap(u, v);
        fa[v] = u;
        siz[u] += siz[v];
        return 1;
    }
};
```

## 交付前检查

对新增或修改的片段执行 `g++ -std=c++17 -Wall -Wextra -pedantic -fsyntax-only`。重要算法用样例、随机测试或对拍验证，接口调整运行相关集成测试。最后检查是否保留用户码风、是否加入无关抽象，以及是否只修改了任务范围内的文件。

#include <bits/stdc++.h>

#define V vector

using namespace std;

// 普通莫队；查询区间 [l, r] 和数组下标均为 1-based。
// 使用前补全 add / del / answer 的定义及题目状态；add / del 接收数组下标。
// answer(id) 保存答案，solve 前清空统计状态；块长约 n / sqrt(q)。
// 排序 O(q log q)，区间移动 O(n sqrt(q) + q)，另乘单次维护复杂度；空间 O(q)。

struct Mo {
    struct Query { int l, r, id; };

    // 示例维护：将下面 3 个声明替换为此段成员，再按题意改。
    // V < int > a, cnt, ans; int kinds = 0;
    // void add(int p) { if (cnt[a[p]] ++ == 0) ++ kinds; }
    // void del(int p) { if (-- cnt[a[p]] == 0) -- kinds; }
    // void answer(int id) { ans[id] = kinds; }
    void add(int p);
    void del(int p);
    void answer(int id);

    void solve(int n, V < Query > qs) {
        int B = max(1, int(n / max(1.0, sqrt(double(max(1, int(qs.size())))))));
        sort(qs.begin(), qs.end(), [&](const Query &a, const Query &b) {
            int ba = a.l / B, bb = b.l / B;
            if (ba != bb) return ba < bb;
            return (ba & 1) ? a.r < b.r : a.r > b.r;
        });
        int l = 1, r = 0;
        for (auto &q : qs) {
            while (l > q.l) add(-- l);
            while (r < q.r) add(++ r);
            while (l < q.l) del(l ++);
            while (r > q.r) del(r --);
            answer(q.id);
        }
    }
};

// 调用示例：
// 以上示例求不同值个数，a 的值先离散到 [1, m]，a 的下标为 1-based。
// Mo mo; mo.a = a; mo.cnt.assign(m + 1, 0); mo.kinds = 0;
// mo.ans.resize(qs.size()); mo.solve(n, qs); // 答案在 mo.ans。
// qs 的类型为 V < Mo::Query >，每个查询的 id 为答案下标。
// 重跑前清零 cnt 和 kinds；不同维护逻辑的重置方式也按题意修改。

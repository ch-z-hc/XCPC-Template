#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
离线处理 1-based 闭区间查询，增删按数组下标。每次求解前清空统计量。
块长约 n / sqrt(q)，q 次查询排序 O(q log q)，移动 O(n sqrt(q) + q) 次。
*/

struct Mo {
    struct Query { int l, r, id; };

    /*
    // 区间不同值个数
    V < int > a, cnt, ans; int kinds = 0;
    void add(int p) { if (cnt[a[p]] ++ == 0) ++ kinds; }
    void del(int p) { if (-- cnt[a[p]] == 0) -- kinds; }
    void answer(int id) { ans[id] = kinds; }
    */
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

/*
Mo mo; mo.a = a; // 统计不同值个数，值已离散到 1..m
mo.cnt.assign(m + 1, 0); mo.kinds = 0;
mo.ans.resize(qs.size()); mo.solve(n, qs); // qs 中每项为 {l, r, 答案下标}
*/

/*
离线处理 1-based 闭区间查询，增删按数组下标。每次求解前清空统计量。
块长约 n / sqrt(q)，q 次查询排序 O(q log q)，移动 O(n sqrt(q) + q) 次。
文件尾第一段替换结构体中的 3 个成员声明，示例统计区间不同值个数。值先离散到 1..m。
qs 每项为 {l, r, 答案下标}。
*/

#include <bits/stdc++.h>

struct Mo {
    struct Query {
        int l, r, id;
    };

    void add(int p);
    void del(int p);
    void answer(int id);

    void solve(int n, std::vector<Query> qs) {
        int B =
            std::max(1, int(n / std::max(1.0, std::sqrt(double(std::max(1, int(qs.size())))))));
        std::sort(qs.begin(), qs.end(), [&](const Query &a, const Query &b) {
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
std::vector<int> a, cnt, ans; int kinds = 0;
void add(int p) { if (cnt[a[p]] ++ == 0) ++ kinds; }
void del(int p) { if (-- cnt[a[p]] == 0) -- kinds; }
void answer(int id) { ans[id] = kinds; }
*/

/*
Mo mo; mo.a = a;
mo.cnt.assign(m + 1, 0); mo.kinds = 0;
mo.ans.resize(qs.size()); mo.solve(n, qs);
*/

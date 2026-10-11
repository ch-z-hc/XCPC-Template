#include <bits/stdc++.h>

#define V vector

using namespace std;

// 普通莫队；查询区间 [l, r] 和数组下标均为 1-based。
// add / remove 接收数组下标，query(id) 保存答案；块长约 n / sqrt(q)。
// 排序 O(q log q)，区间移动 O(n sqrt(q) + q)，另乘回调复杂度；空间 O(q)。

struct Query { int l, r, id; };

template < class Add, class Remove, class Answer >
void mo(int n, V < Query > qs, Add add, Remove remove, Answer query) {
    int B = max(1, int(n / max(1.0, sqrt(double(max(1, int(qs.size())))))));
    sort(qs.begin(), qs.end(), [&](const Query &a, const Query &b) {
        int ba = a.l / B, bb = b.l / B;
        if (ba != bb) return ba < bb;
        return (ba & 1) ? a.r < b.r : a.r > b.r;
    });
    int l = 1, r = 0;
    for (const auto &q : qs) {
        while (l > q.l) add(-- l);
        while (r < q.r) add(++ r);
        while (l < q.l) remove(l ++);
        while (r > q.r) remove(r --);
        query(q.id);
    }
}

// 调用示例：
// mo(n, qs, add, del, query); // 回调分别添加下标、删除下标、保存答案。

#include <bits/stdc++.h>
using namespace std;

struct MoQuery { int left, right, id; };

// 普通莫队；查询区间 [left, right] 和数组下标均为 1-based。
// Add/Remove 接收数组下标，Answer 写入 result[id]；复杂度 O((n + q) sqrt(n)) 次移动。
template <class Add, class Remove, class Answer>
void run_mo(int n, vector<MoQuery> queries, Add add, Remove remove, Answer answer) {
    int block = max(1, int(n / max(1.0, sqrt(double(max(1, int(queries.size())))))));
    sort(queries.begin(), queries.end(), [&](const MoQuery &a, const MoQuery &b) {
        int ba = a.left / block, bb = b.left / block;
        if (ba != bb) return ba < bb;
        return (ba & 1) ? a.right < b.right : a.right > b.right;
    });
    int left = 1, right = 0;
    for (const auto &q : queries) {
        while (left > q.left) add(--left);
        while (right < q.right) add(++right);
        while (left < q.left) remove(left++);
        while (right > q.right) remove(right--);
        answer(q.id);
    }
}

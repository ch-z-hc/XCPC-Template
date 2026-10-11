#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

// 先复制 FPS.cpp；固定模数 998244353，系数归一化，数组 0-based。
// bostan_mori 求 [x^k] P(x)/Q(x)，要求 Q 非空且 Q[0] != 0；P 可为空。
// 输入长度至多 2^22；D = max(|P|,|Q|)，O(D log D log(k+2)) 时间、O(D) 空间。
// linear_kth 求 d 阶递推第 k 项：a[n] = sum(rec[i] * a[n-1-i])。
// init 与 rec 长度同为 d，d < 2^22；d = 0 视作零序列，k 支持整个 ull 范围。
int bostan_mori(V < int > p, V < int > q, ull k) {
    assert(! q.empty() && q[0]);
    while (k && ! p.empty()) {
        V < int > neg = q;
        for (int i = 1; i < int(neg.size()); i += 2)
            if (neg[i]) neg[i] = FPS::mod - neg[i];
        auto a = FPS::mul(p, neg), b = FPS::mul(q, neg);
        p.clear(); q.clear();
        for (size_t i = k & 1; i < a.size(); i += 2) p.push_back(a[i]);
        for (size_t i = 0; i < b.size(); i += 2) q.push_back(b[i]);
        k >>= 1;
    }
    return p.empty() ? 0 : 1ll * p[0] * FPS::power(q[0], FPS::mod - 2) % FPS::mod;
}

int linear_kth(const V < int > &init, const V < int > &rec, ull k) {
    int d = int(rec.size());
    if (k < init.size()) return init[size_t(k)];
    if (! d) return 0;
    V < int > q(d + 1, 1);
    for (int i = 0; i < d; ++ i) q[i + 1] = rec[i] ? FPS::mod - rec[i] : 0;
    auto p = FPS::mul(init, q); p.resize(d);
    return bostan_mori(p, q, k);
}

// int ans = linear_kth({0, 1}, {1, 1}, 1000000000000000000ull);
// int coef = bostan_mori({1}, {1, FPS::mod - 1}, k); // 恒为 1。

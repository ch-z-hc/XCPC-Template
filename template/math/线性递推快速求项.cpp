/*
先复制「多项式基本运算」。求有理生成函数系数或线性递推第 k 项，k 从 0 数，可取任意 u64。
固定模 998244353，低次系数在前且已取模。P、Q 长度 <= 2^22，Q[0] 非零。
递推初值和系数等长，阶数 d < 2^22。多项式最大长度为 D 时耗时 O(D log(D + 1) log(k + 2))。
*/

#include <bits/stdc++.h>

using u64 = unsigned long long;

int bostan_mori(std::vector<int> p, std::vector<int> q, u64 k) {
    assert(!q.empty() && q[0]);
    while (k && !p.empty()) {
        std::vector<int> neg = q;
        for (int i = 1; i < int(neg.size()); i += 2)
            if (neg[i]) neg[i] = FPS::mod - neg[i];
        auto a = FPS::mul(p, neg), b = FPS::mul(q, neg);
        p.clear();
        q.clear();
        for (std::size_t i = k & 1; i < a.size(); i += 2) p.push_back(a[i]);
        for (std::size_t i = 0; i < b.size(); i += 2) q.push_back(b[i]);
        k >>= 1;
    }
    return p.empty() ? 0 : 1ll * p[0] * FPS::power(q[0], FPS::mod - 2) % FPS::mod;
}

int linear_kth(const std::vector<int> &init, const std::vector<int> &rec, u64 k) {
    int d = int(rec.size());
    if (k < init.size()) return init[std::size_t(k)];
    if (!d) return 0;
    std::vector<int> q(d + 1, 1);
    for (int i = 0; i < d; i ++) q[i + 1] = rec[i] ? FPS::mod - rec[i] : 0;
    auto p = FPS::mul(init, q);
    p.resize(d);
    return bostan_mori(p, q, k);
}

/*
int ans = linear_kth({0, 1}, {1, 1}, 1000000000000000000ull);
int coef = bostan_mori({1}, {1, FPS::mod - 1}, k);
*/

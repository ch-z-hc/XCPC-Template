#include <bits/stdc++.h>
using namespace std;

static long long bm_norm(long long x, long long mod) {
    x %= mod;
    return x < 0 ? x + mod : x;
}

static long long bm_pow(long long a, long long e, long long mod) {
    long long r = 1 % mod;
    for (; e; e >>= 1, a = a * a % mod)
        if (e & 1) r = r * a % mod;
    return r;
}

// 根据序列前缀求最短线性递推式；s[n] = rec[0]s[n-1] + ...。
vector<long long> berlekamp_massey(const vector<long long> &s, long long mod) {
    vector<long long> c{1}, b{1};
    int length = 0, shift = 1;
    long long last_delta = 1;

    for (int n = 0; n < int(s.size()); ++n) {
        long long delta = s[n] % mod;
        for (int i = 1; i <= length; ++i)
            delta = (delta + c[i] * s[n - i]) % mod;
        if (delta == 0) {
            ++shift;
            continue;
        }

        vector<long long> old = c;
        long long factor = delta * bm_pow(last_delta, mod - 2, mod) % mod;
        if (int(c.size()) < int(b.size()) + shift) c.resize(b.size() + shift, 0);
        for (int i = 0; i < int(b.size()); ++i)
            c[i + shift] = bm_norm(c[i + shift] - factor * b[i], mod);

        if (2 * length <= n) {
            length = n + 1 - length;
            b = move(old);
            last_delta = delta;
            shift = 1;
        } else {
            ++shift;
        }
    }

    c.erase(c.begin());
    for (long long &x : c) x = bm_norm(-x, mod);
    return c;
}

static vector<long long> bm_combine(const vector<long long> &a,
                                    const vector<long long> &b,
                                    const vector<long long> &rec,
                                    long long mod) {
    int k = int(rec.size());
    vector<long long> result(2 * k, 0);
    for (int i = 0; i < k; ++i)
        for (int j = 0; j < k; ++j)
            result[i + j] = (result[i + j] + a[i] * b[j]) % mod;
    for (int i = 2 * k - 2; i >= k; --i) if (result[i])
        for (int j = 1; j <= k; ++j)
            result[i - j] = (result[i - j] + result[i] * rec[k - j]) % mod;
    result.resize(k);
    return result;
}

// 求 0-based 的第 k 项；初始序列长度应至少等于递推阶数。
long long linear_recurrence_kth(const vector<long long> &init,
                                const vector<long long> &rec,
                                long long k, long long mod) {
    int n = int(rec.size());
    if (k < int(init.size())) return bm_norm(init[int(k)], mod);
    if (n == 0) return 0;
    vector<long long> pol(n, 0), e(n, 0);
    pol[0] = e[1 % n] = 1;
    while (k > 0) {
        if (k & 1) pol = bm_combine(pol, e, rec, mod);
        e = bm_combine(e, e, rec, mod);
        k >>= 1;
    }
    long long answer = 0;
    for (int i = 0; i < n; ++i) answer = (answer + pol[i] * init[i]) % mod;
    return answer;
}

// Usage:
// vector<long long> rec = berlekamp_massey(sequence, MOD);
// long long ans = linear_recurrence_kth(init, rec, k, MOD);

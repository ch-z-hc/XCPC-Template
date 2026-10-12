#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 二维双模哈希：输入为等宽字符串矩阵，支持任意字节，坐标 0-based。
// query(x1, y1, x2, y2) 查询闭矩形；空矩形返回 {0,0}。
// build 为 O(nm)，query 为 O(1)，空间 O(nm)，支持空矩阵与重复 build。
// 固定两个质数模数与行列基数；跨对象可比较，但必须同时核对矩形尺寸。
// 哈希有碰撞风险，不是严格判等；所有非空查询坐标必须合法。
struct Hash2D {
    struct Table {
        static constexpr ll bx = 911382323, by = 972663749;
        ll mod = 1;
        V < V < ll > > h;
        V < ll > x, y;

        void build(const V < string > &a, ll p) {
            mod = p;
            int n = int(a.size()), m = n ? int(a[0].size()) : 0;
            h.assign(n + 1, V < ll > (m + 1));
            x.assign(n + 1, 1); y.assign(m + 1, 1);
            for (int i = 1; i <= n; ++ i) x[i] = x[i - 1] * bx % mod;
            for (int j = 1; j <= m; ++ j) y[j] = y[j - 1] * by % mod;
            for (int i = 0; i < n; ++ i) for (int j = 0; j < m; ++ j) {
                ll v = (unsigned char)a[i][j] + 1;
                v += h[i][j + 1] * bx % mod + h[i + 1][j] * by % mod;
                v -= h[i][j] * bx % mod * by % mod;
                h[i + 1][j + 1] = (v % mod + mod) % mod;
            }
        }

        ll query(int x1, int y1, int x2, int y2) const {
            int n = x2 - x1 + 1, m = y2 - y1 + 1;
            ll v = h[x2 + 1][y2 + 1] - h[x1][y2 + 1] * x[n] % mod;
            v -= h[x2 + 1][y1] * y[m] % mod;
            v += h[x1][y1] * x[n] % mod * y[m] % mod;
            return (v % mod + mod) % mod;
        }
    };

    Table a, b;

    void build(const V < string > &s) {
        a.build(s, 1000000007); b.build(s, 1000000009);
    }

    pair < ll, ll > query(int x1, int y1, int x2, int y2) const {
        if (x1 > x2 || y1 > y2) return {0, 0};
        return {a.query(x1, y1, x2, y2), b.query(x1, y1, x2, y2)};
    }
};

// Hash2D hash; hash.build({"abc", "def"});
// auto value = hash.query(0, 1, 1, 2);

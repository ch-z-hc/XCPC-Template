#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 每行须等长。query(x1, y1, x2, y2) 取闭矩形的双模哈希，行列都从 0 数。
// 空矩形返回 {0, 0}。比较两个矩形时先比尺寸，再比哈希值，仍有碰撞的可能。
// 建表时间和空间 O(nm)，查询 O(1)。
struct Hash2D {
    struct Table {
        static const ll bx = 911382323, by = 972663749;
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
            for (int i = 0; i < n; ++ i) {
                for (int j = 0; j < m; ++ j) {
                    ll v = (unsigned char)a[i][j] + 1;
                    v += h[i][j + 1] * bx % mod + h[i + 1][j] * by % mod;
                    v -= h[i][j] * bx % mod * by % mod;
                    h[i + 1][j + 1] = (v % mod + mod) % mod;
                }
            }
        }

        ll query(int x1, int y1, int x2, int y2) {
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

    pair < ll, ll > query(int x1, int y1, int x2, int y2) {
        if (x1 > x2 || y1 > y2) return {0, 0};
        return {a.query(x1, y1, x2, y2), b.query(x1, y1, x2, y2)};
    }
};
const ll Hash2D::Table::bx, Hash2D::Table::by;

// Hash2D hash; hash.build({"abc", "def"});
// auto value = hash.query(0, 1, 1, 2);

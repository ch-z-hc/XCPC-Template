#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << ' '
#define V vector
#define fi first
#define se second
#define pii pair < int, int >

typedef long long ll;

const int mod = 1e9 + 7;

using namespace std;

template < class T > bool chkmin(T & a, const T & b) { return b < a ? a = b, 1 : 0; }
template < class T > bool chkmax(T & a, const T & b) { return a < b ? a = b, 1 : 0; } 
template < class T > void add(T &x, T y) { x += y; if (x >= mod) x -= mod; }
template < class T > void sub(T &x, T y) { x -= y; if (x < 0) x += mod; }

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m, k, blk; cin >> n >> m >> k;
    V < int > a(n + 1); blk = ceil(n / sqrt(m));
    struct ask { int l, r, id; };
    V < ask > q(m + 1);
    for (int i = 1; i <= n; ++ i) cin >> a[i];
    for (int i = 1; i <= m; ++ i) {
        cin >> q[i].l >> q[i].r; q[i].id = i;
    }
    auto pos = [&](int x) { return x / blk; }; 
    sort(q.begin() + 1, q.end(), [&](ask a, ask b) {
        if (pos(a.l) != pos(b.l)) return a.l < b.l;
        if (pos(a.l) & 1) return a.r < b.r;
        return a.r > b.r;
    });

    V < int > ans(m + 1), cnt(k + 1);

    int l = 1, r = 0, nowans = 0;

    auto add = [&](int x) { 
        nowans -= cnt[a[x]] * cnt[a[x]];
        cnt[a[x]] ++;
        nowans += cnt[a[x]] * cnt[a[x]];
    };
    auto del = [&](int x) { 
        nowans -= cnt[a[x]] * cnt[a[x]];
        cnt[a[x]] --;
        nowans += cnt[a[x]] * cnt[a[x]];
    };

    for (int i = 1; i <= m; ++ i) {
        while (l > q[i].l) { l --; add(l); }
        while (r < q[i].r) { r ++; add(r); }
        while (l < q[i].l) { del(l); l ++; }
        while (r > q[i].r) { del(r); r --; }
        ans[q[i].id] = nowans;
    }

    for (int i = 1; i <= m; ++ i) cout << ans[i] << '\n';

    return 0;
}
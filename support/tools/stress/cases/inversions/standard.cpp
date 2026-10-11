#include "../../../../../template/ds/Fenwick.cpp"

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n; Fenwick bit(n);
    ll ans = 0;
    for (int i = 1; i <= n; ++ i) {
        int x; cin >> x;
        ans += i - 1 - bit.query(x); bit.add(x, 1);
    }
    cout << ans << '\n';
}

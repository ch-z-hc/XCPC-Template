#include "../../../../../template/ds/树状数组.cpp"

int main() {
    std::ios::sync_with_stdio(0); std::cin.tie(0);
    int n; std::cin >> n; Fenwick bit(n);
    i64 ans = 0;
    for (int i = 1; i <= n; ++ i) {
        int x; std::cin >> x;
        ans += i - 1 - bit.query(x); bit.add(x, 1);
    }
    std::cout << ans << '\n';
}

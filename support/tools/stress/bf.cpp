#include <iostream>
using namespace std;

// 示例：数组求和；实际使用时换成小数据暴力。
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    long long ans = 0;
    for (int i = 1, x; i <= n; ++ i) {
        cin >> x;
        ans += x;
    }
    cout << ans << '\n';
}

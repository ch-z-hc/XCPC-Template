#include "../../../../../template/string/字符串匹配.cpp"

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    string s, t; getline(cin, s); getline(cin, t);
    auto ans = kmp(s, t); cout << ans.size() << '\n';
    for (int x : ans) cout << x << ' ';
    cout << '\n';
}

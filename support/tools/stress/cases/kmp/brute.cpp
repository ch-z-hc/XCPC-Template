#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 枚举所有起点直接比较，O(|s| |t|)；空模式串匹配每个边界。
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    string s, t; getline(cin, s); getline(cin, t);
    V < int > ans;
    for (int i = 0; i + int(t.size()) <= int(s.size()); ++ i)
        if (s.compare(i, t.size(), t) == 0) ans.push_back(i);
    cout << ans.size() << '\n';
    for (int x : ans) cout << x << ' ';
    cout << '\n';
}

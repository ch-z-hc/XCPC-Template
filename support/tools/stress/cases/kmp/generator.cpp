#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// 两行字符串，允许空串和空格；包含重叠匹配、空模式串和模式长于文本。
int main(int argc, char **argv) {
    uint64_t seed = argc > 1 ? stoull(argv[1]) : 1;
    mt19937_64 rng(seed);
    auto rnd = [&](int l, int r) { return uniform_int_distribution < int > (l, r)(rng); };
    int mode = seed % 8;
    int n = mode == 0 ? 0 : rnd(1, 100), m = mode == 1 ? 0 : rnd(1, 30);
    if (mode == 5) m = n + rnd(1, 10);
    string s, t, chars = "ab c";
    for (int i = 0; i < n; ++ i) s += chars[rnd(0, 3)];
    for (int i = 0; i < m; ++ i) t += chars[rnd(0, 3)];
    if (mode == 2) t = s.substr(rnd(0, n - 1), rnd(1, n));
    if (mode == 3) s.assign(n, 'a'), t.assign(m, 'a');
    if (mode == 4) {
        for (int i = 0; i < n; ++ i) s[i] = "ab"[i & 1];
        t = "aba";
    }
    cout << s << '\n' << t << '\n';
}

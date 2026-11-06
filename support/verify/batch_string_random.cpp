#include "../../template/string/后缀数组.cpp"
#include "../../template/string/林登分解.cpp"
#include "../../template/string/极大周期子串.cpp"

#include <bits/stdc++.h>

bool byte_less(const string &a, const string &b) {
    return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
        [](unsigned char x, unsigned char y) { return x < y; });
}

bool is_lyndon(const string &s) {
    for (int i = 1; i < int(s.size()); ++ i)
        if (! byte_less(s, s.substr(i))) return false;
    return ! s.empty();
}

V < array < int, 3 > > brute_runs(const string &s) {
    int n = int(s.size());
    V < array < int, 3 > > ans;
    for (int l = 0; l < n; ++ l) for (int r = l + 2; r <= n; ++ r) {
        for (int p = 1; p <= r - l; ++ p) {
            bool ok = true;
            for (int i = l + p; i < r; ++ i) if (s[i] != s[i - p]) ok = false;
            if (! ok) continue;
            if (r - l >= 2 * p && (l == 0 || s[l - 1] != s[l - 1 + p]) &&
                (r == n || s[r] != s[r - p])) ans.push_back({p, l, r});
            break;
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}

void check(const string &s) {
    auto pos = duval(s);
    assert(pos.front() == 0 && pos.back() == int(s.size()));
    string prev;
    for (int i = 1; i < int(pos.size()); ++ i) {
        assert(pos[i] > pos[i - 1]);
        string cur = s.substr(pos[i - 1], pos[i] - pos[i - 1]);
        assert(is_lyndon(cur));
        if (i > 1) assert(! byte_less(prev, cur));
        prev = cur;
    }
    assert(runs(s) == brute_runs(s));
}

int main() {
    for (int n = 0; n <= 9; ++ n) {
        int total = 1;
        for (int i = 0; i < n; ++ i) total *= 3;
        for (int mask = 0; mask < total; ++ mask) {
            string s(n, 'a'); int x = mask;
            for (char &c : s) c += char(x % 3), x /= 3;
            check(s);
        }
    }
    mt19937 rng(621731);
    for (int rep = 0; rep < 1500; ++ rep) {
        string s(rng() % 35, '\0');
        for (char &c : s) c = char(rng() % 256);
        check(s);
    }
    string s(100000, 'a');
    auto res = runs(s);
    assert((res == V < array < int, 3 > > {{1, 0, 100000}}));
    assert(duval(s).size() == 100001);
    cout << "PASS: Lyndon and runs exhaustive/randomized checks\n";
}

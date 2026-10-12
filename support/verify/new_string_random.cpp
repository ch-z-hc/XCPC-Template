#include <bits/stdc++.h>

#include "../../template/string/扩展字符串匹配.cpp"
#include "../../template/string/字符串哈希.cpp"
#include "../../template/string/回文自动机.cpp"
#include "../../template/string/最小表示法.cpp"

using namespace std;

mt19937 rng(94827163);

int direct_lcp(const string &a, int i, const string &b, int j) {
    int n = 0;
    while (i + n < int(a.size()) && j + n < int(b.size()) && a[i + n] == b[j + n])
        n ++;
    return n;
}

void check_z(const string &s, const string &t) {
    auto z = get_z(s), e = exkmp(s, t);
    assert(z.size() == s.size() && e.size() == s.size());
    for (int i = 0; i < int(s.size()); ++ i) {
        assert(z[i] == direct_lcp(s, 0, s, i));
        assert(e[i] == direct_lcp(s, i, t, 0));
    }
    assert(exkmp(s, "") == vector<int>(s.size()));
    assert(exkmp("", t).empty());
}

void check_hash(const string &s) {
    Hash h(s), other;
    other.build("old"); other.build(s);
    int n = int(s.size());
    assert(h.query(0, -1) == make_pair(0ll, 0ll));
    assert(h.query(n, n - 1) == make_pair(0ll, 0ll));
    for (int i = 0; i <= n; ++ i)
        for (int j = 0; j <= n; ++ j)
            assert(h.lcp(i, j) == direct_lcp(s, i, s, j));
    for (int l = 0; l < n; ++ l)
        for (int r = l; r < n; ++ r) {
            string t = s.substr(l, r - l + 1);
            Hash part(t);
            assert(h.query(l, r) == part.query(0, int(t.size()) - 1));
            assert(h.query(l, r) == other.query(l, r));
            int p = int(rng() % (n - int(t.size()) + 1));
            bool same = t == s.substr(p, t.size());
            assert((h.query(l, r) == h.query(p, p + int(t.size()) - 1)) == same);
        }
    other.build("");
    assert(other.lcp(0, 0) == 0 && other.query(0, -1) == make_pair(0ll, 0ll));
}

void check_rotation(const string &s) {
    int ans = 0;
    string best = s;
    for (int i = 1; i < int(s.size()); ++ i) {
        string t = s.substr(i) + s.substr(0, i);
        bool less = lexicographical_compare(t.begin(), t.end(), best.begin(), best.end(),
            [](char a, char b) { return (unsigned char)a < (unsigned char)b; });
        if (less) best = t, ans = i;
    }
    assert(min_rotation(s) == ans);
}

map < string, ll > palindromes(const string &s) {
    map < string, ll > ans;
    for (int l = 0; l < int(s.size()); ++ l)
        for (int r = l; r < int(s.size()); ++ r) {
            bool ok = 1;
            for (int i = l, j = r; i < j; ++ i, -- j)
                if (s[i] != s[j]) ok = 0;
            if (ok) ans[s.substr(l, r - l + 1)] ++;
        }
    return ans;
}

void check_pam(const string &s) {
    PAM p;
    vector < string > rep(2);
    for (int i = 0; i < int(s.size()); ++ i) {
        int u = p.insert(s[i]);
        rep.resize(p.tr.size());
        string t = s.substr(i - p.tr[u].len + 1, p.tr[u].len);
        if (rep[u].empty()) rep[u] = t;
        assert(rep[u] == t);
        auto expected = palindromes(s.substr(0, i + 1));
        auto cnt = p.count();
        assert(cnt == p.count());
        assert(p.distinct() == int(expected.size()));
        assert(cnt[0] == 0 && cnt[1] == 0);
        set < string > seen;
        for (int v = 2; v < int(rep.size()); ++ v) {
            assert(seen.insert(rep[v]).second);
            assert(expected.count(rep[v]) && cnt[v] == expected.at(rep[v]));
        }
    }
    auto cnt = p.count();
    p.build("abacaba"); p.build(s);
    assert(p.count() == cnt && p.distinct() == int(rep.size()) - 2);
    p.build("");
    assert(p.distinct() == 0 && p.count() == vector<ll>({0, 0}));
    p.insert('z'); assert(p.distinct() == 1 && p.count()[2] == 1);
}

int main() {
    for (int n = 0; n <= 8; ++ n)
        for (int mask = 0; mask < (1 << n); ++ mask) {
            string s(n, 'a');
            for (int i = 0; i < n; ++ i) s[i] += (mask >> i & 1);
            check_z(s, s.substr(n / 2));
            check_hash(s); check_rotation(s); check_pam(s);
        }
    for (int z = 0; z < 500; ++ z) {
        string s(rng() % 30, '\0'), t(rng() % 30, '\0');
        for (char &c : s) c = char(rng() % 256);
        for (char &c : t) c = char(rng() % 256);
        check_z(s, t); check_hash(s); check_rotation(s);
        string a(rng() % 15, 'a');
        for (char &c : a) c += rng() % 3;
        check_pam(a);
    }
    string s(2000, 'a');
    check_z(s, string(1700, 'a')); check_rotation(s);
    Hash h(s);
    for (int q = 0; q < 1000; ++ q) {
        int u = rng() % 2001, v = rng() % 2001;
        assert(h.lcp(u, v) == 2000 - max(u, v));
    }
    PAM p;
    for (int i = 1; i <= 2000; ++ i) {
        int u = p.insert('a');
        assert(p.tr[u].len == i && p.distinct() == i);
    }
    auto cnt = p.count();
    for (int u = 2; u < int(p.tr.size()); ++ u)
        assert(cnt[u] == 2001 - p.tr[u].len);
    p.build(s); assert(p.count() == cnt);
    string bytes{char(0), char(128), char(255), char(0), char(128)};
    check_z(bytes, bytes); check_hash(bytes); check_rotation(bytes);
    cout << "PASS: Z, extended KMP, Hash, PAM and minimum rotation\n";
}

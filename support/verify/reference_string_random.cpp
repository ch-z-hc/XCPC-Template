#include "../../template/ds/可持久化二进制字典树.cpp"
#include "../../template/string/后缀自动机.cpp"
#include "../../template/string/回文自动机.cpp"
#include "../../template/ds/整体加一与集合异或和.cpp"
#include "../../template/string/大字符集多模式匹配.cpp"
#include "../../template/string/广义后缀自动机.cpp"
#include "../../template/string/后缀自动机双向扩展.cpp"
#include "../../template/string/回文半长后缀与最少分割.cpp"

int main() {
    mt19937 rng(2701);
    auto word = [&](int n) {
        string s;
        while (n --) s += char('a' + rng() % 3);
        return s;
    };
    for (int t = 0; t < 1500; t ++) {
        V < string > words;
        set < string > parts{""};
        GSAM online, offline;
        for (int k = 0; k < 5; k ++) {
            auto s = word(rng() % 10);
            for (int i = 0; i < int(s.size()); i ++)
                for (int j = i + 1; j <= int(s.size()); j ++) parts.insert(s.substr(i, j - i));
            words.push_back(s);
            online.insert(s);
            assert(online.distinct() == ll(parts.size()) - 1);
        }
        offline.build(words); offline.build(words);
        assert(offline.distinct() == online.distinct());
        for (auto &s : parts) assert(online.contains(s) && offline.contains(s));
        for (int j = 0; j < 10; j ++) {
            auto s = word(4); assert(offline.contains(s) == bool(parts.count(s)));
        }

        string s = word(rng() % 15);
        BiSAM bi; bi.build(s);
        for (int j = 0; j < 30; j ++) {
            pii state{0, 0};
            string q;
            for (int k = 0; k < 7; k ++) {
                char c = char('a' + rng() % 3);
                if (rng() & 1) state = bi.prepend(state, c), q = c + q;
                else state = bi.append(state, c), q += c;
                bool ok = s.find(q) != string::npos;
                assert((state.fi >= 0) == ok && state.se == (ok ? int(q.size()) : 0));
            }
            int best = 0;
            for (int a = 0; a < int(q.size()); a ++)
                for (int b = a + 1; b <= int(q.size()); b ++)
                    if (s.find(q.substr(a, b - a)) != string::npos) best = max(best, b - a);
            assert(best == bi.lcs(q));
        }
        PalPartition pal;
        pal.build(s); pal.build(s);
        V < int > dp(s.size() + 1);
        for (int i = 1; i <= int(s.size()); i ++) {
            dp[i] = i;
            for (int j = 0; j < i; j ++) {
                auto w = s.substr(j, i - j);
                if (equal(w.begin(), w.end(), w.rbegin())) dp[i] = min(dp[i], dp[j] + 1);
            }
        }
        assert(dp == pal.cut);
        auto &tr = pal.pam.tr;
        for (int u = 2; u < int(tr.size()); u ++) {
            int v = tr[u].fail;
            while (tr[v].len > tr[u].len / 2) v = tr[v].fail;
            assert(pal.half[u] == v);
        }

        SparseAC ac(1000000000);
        V < V < int > > patterns;
        V < int > id, text;
        auto sequence = [&](int len) {
            V < int > a;
            while (len --) a.push_back((rng() % 3) * 400000000);
            return a;
        };
        for (int k = 0; k < 8; k ++) {
            patterns.push_back(sequence(rng() % 5 + 1));
            id.push_back(ac.insert(patterns.back()));
            if (k == 3) ac.build();
        }
        ac.build();
        text = sequence(20);
        auto cnt = ac.count(text);
        for (int k = 0; k < 8; k ++) {
            int hits = 0, len = int(patterns[k].size());
            for (int j = 0; j + len <= 20; j ++)
                hits += equal(patterns[k].begin(), patterns[k].end(), text.begin() + j);
            assert(cnt[id[k]] == hits);
        }
    }
    PXorTrie versions;
    int a = versions.insert(0, 5), b = versions.insert(0, 9);
    int both = versions.merge(a, b), doubled = versions.merge(both, both);
    ull ans;
    assert(versions.max_xor(a, both, 0, ans) && ans == 9);
    assert(versions.max_xor(b, both, 0, ans) && ans == 5);
    assert(versions.max_xor(both, doubled, 0, ans) && ans == 9);
    assert(versions.tr[a].cnt == 1 && versions.tr[doubled].cnt == 4);
    XorSet trie;
    V < uint32_t > values;
    for (int i = 0; i < 15000; i ++) {
        switch (rng() % 4) {
            case 0:
                trie.add_one();
                for (auto &x : values) x ++;
                break;
            case 1:
                if (!values.empty()) {
                    int j = rng() % values.size();
                    trie.add(values[j], -1);
                    swap(values[j], values.back()); values.pop_back();
                }
                break;
            default: {
                uint32_t x = (rng() % 2) ? rng() : UINT32_MAX;
                trie.add(x); values.push_back(x);
            }
        }
        uint32_t xr = 0;
        for (auto x : values) xr ^= x;
        assert(trie.query() == xr);
    }
}

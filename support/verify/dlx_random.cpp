#include <cassert>
#include "../../template/others/舞蹈链精确覆盖.cpp"

bool brute(int cols, const V < V < int > > &rows) {
    V < unsigned > mask(rows.size());
    for (int i = 0; i < int(rows.size()); ++ i)
        for (int c : rows[i]) mask[i] |= 1u << (c - 1);
    for (unsigned s = 0; s < (1u << rows.size()); ++ s) {
        unsigned have = 0; bool ok = 1;
        for (int i = 0; i < int(rows.size()); ++ i) if (s >> i & 1) {
            if (have & mask[i]) { ok = 0; break; }
            have |= mask[i];
        }
        if (ok && have == (1u << cols) - 1) return 1;
    }
    return 0;
}

V < array < int, 6 > > snapshot(const DLX &dlx) {
    V < array < int, 6 > > ans;
    for (auto node : dlx.tr) ans.push_back({node.l, node.r, node.u, node.d, node.col, node.row});
    return ans;
}

void check(DLX &dlx, const V < V < int > > &rows) {
    auto before = snapshot(dlx); auto siz = dlx.siz;
    bool exists = brute(dlx.cols, rows);
    for (int repeat = 0; repeat < 3; ++ repeat) {
        V < int > ans; assert(dlx.solve(ans) == exists);
        assert(snapshot(dlx) == before && dlx.siz == siz && dlx.cur.empty());
        if (! exists) continue;
        V < int > cnt(dlx.cols + 1);
        set < int > seen;
        for (int id : ans) {
            int i = id - 100;
            assert(0 <= i && i < int(rows.size()) && seen.insert(id).second);
            assert(! rows[i].empty());
            for (int c : rows[i]) cnt[c] ++;
        }
        for (int c = 1; c <= dlx.cols; ++ c) assert(cnt[c] == 1);
    }
}

int main() {
    mt19937 rng(20261015);
    for (int test = 0; test < 2500; ++ test) {
        int cols = rng() % 9, n = rng() % 12;
        DLX dlx(cols);
        V < V < int > > rows(n);
        for (int i = 0; i < n; ++ i) {
            for (int c = 1; c <= cols; ++ c) if (rng() % 3 == 0) rows[i].push_back(c);
            shuffle(rows[i].begin(), rows[i].end(), rng);
            dlx.add_row(i + 100, rows[i]);
        }
        check(dlx, rows);
        for (int c = 1; c <= cols; ++ c) {
            dlx.add_row(int(rows.size()) + 100, {c}); rows.push_back({c});
        }
        // 加入单列行后必有解，不再对最多 19 行重复穷举。
        V < int > ans; assert(dlx.solve(ans));
        dlx.init(1); check(dlx, {});
        dlx.add_row(100, {1}); check(dlx, {{1}});
    }
    DLX dlx(7);
    V < V < int > > rows{{1, 4, 7}, {1, 4}, {4, 5, 7}, {3, 5, 6}, {2, 3, 6, 7}, {2, 7}};
    for (int i = 0; i < int(rows.size()); ++ i) dlx.add_row(i + 100, rows[i]);
    check(dlx, rows);
    DLX empty; V < int > ans{1};
    assert(empty.solve(ans) && ans.empty());
    ans.push_back(2); assert(empty.solve(ans) && ans.empty());
    cout << "dlx random tests passed\n";
}

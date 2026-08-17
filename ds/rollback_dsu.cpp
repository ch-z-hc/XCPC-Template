#include <bits/stdc++.h>
using namespace std;

// 可撤销并查集；不使用路径压缩，摊销合并复杂度 O(log n)。
struct RollbackDSU {
    vector<int> parent, size;
    vector<pair<int, int>> history;

    explicit RollbackDSU(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n + 1);
        size.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
        history.clear();
    }

    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }

    int snapshot() const { return int(history.size()); }

    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            history.push_back({-1, -1});
            return false;
        }
        if (size[x] < size[y]) swap(x, y);
        history.push_back({y, size[x]});
        parent[y] = x;
        size[x] += size[y];
        return true;
    }

    void rollback(int snap) {
        while (int(history.size()) > snap) {
            auto [y, old_size] = history.back();
            history.pop_back();
            if (y == -1) continue;
            int x = parent[y];
            parent[y] = y;
            size[x] = old_size;
        }
    }
};

// Usage:
// RollbackDSU dsu(n);
// int snap = dsu.snapshot();
// dsu.unite(x, y);
// dsu.rollback(snap);

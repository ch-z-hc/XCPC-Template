#include <bits/stdc++.h>
using namespace std;

// 普通并查集；元素编号 1..n，使用路径压缩和按大小合并。
// 初始化 O(n)，单次操作摊销 O(alpha(n))，空间 O(n)。
struct DisjointSetUnion {
    int components = 0;
    vector<int> parent, size;

    explicit DisjointSetUnion(int n = 0) { init(n); }

    void init(int n) {
        components = n;
        parent.resize(n + 1);
        size.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int first, int second) {
        first = find(first);
        second = find(second);
        if (first == second) return false;
        if (size[first] < size[second]) swap(first, second);
        parent[second] = first;
        size[first] += size[second];
        --components;
        return true;
    }

    bool same(int first, int second) { return find(first) == find(second); }
    int component_size(int x) { return size[find(x)]; }
};

// Usage:
// DisjointSetUnion dsu(n);
// dsu.unite(u, v);
// bool connected = dsu.same(u, v);

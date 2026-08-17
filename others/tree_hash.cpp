#include <bits/stdc++.h>
using namespace std;

static uint64_t tree_hash_mix(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

uint64_t rooted_tree_hash(int u, int parent, const vector<vector<int>> &g) {
    vector<uint64_t> child;
    for (int v : g[u]) if (v != parent)
        child.push_back(rooted_tree_hash(v, u, g));
    sort(child.begin(), child.end());
    uint64_t h = tree_hash_mix(child.size() + 1);
    for (uint64_t x : child) h = tree_hash_mix(h ^ x);
    return h;
}

// Usage:
// uint64_t h = rooted_tree_hash(root, 0, graph);
// 这是随机化哈希，存在极小概率碰撞；需要严格判定时应使用同构算法。

#include <bits/stdc++.h>

#define V vector

using namespace std;

// 输入必须为树；顶点编号 1-based，默认以 1 为根，fa 为递归用的父节点。
// 每个节点排序子树哈希，复杂度 O(sum(deg(u) log deg(u)))，递归空间 O(n)。

static uint64_t hash_mix(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

uint64_t tree_hash(const V < V < int > > &g, int u = 1, int fa = 0) {
    V < uint64_t > a;
    for (int v : g[u]) if (v != fa)
        a.push_back(tree_hash(g, v, u));
    sort(a.begin(), a.end());
    uint64_t h = hash_mix(a.size() + 1);
    for (uint64_t x : a) h = hash_mix(h ^ x);
    return h;
}

// 调用示例：
// uint64_t h = tree_hash(g, root);
// 这是随机化哈希，存在极小概率碰撞；需要严格判定时应使用同构算法。

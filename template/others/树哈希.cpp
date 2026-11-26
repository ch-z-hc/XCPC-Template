/*
有根无序树哈希，儿子顺序不影响结果，相同哈希可能碰撞。点从 1 编号。
时间 O(n + Σ deg(u) log(deg(u) + 1))。
*/

#include <bits/stdc++.h>

#define V vector

using namespace std;

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

// uint64_t h = tree_hash(g, root);

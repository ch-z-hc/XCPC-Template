#include <bits/stdc++.h>
using namespace std;

// DSU on Tree：离线处理每个子树的统计问题。
// 适用于加点、删点和读取答案均可由回调维护的状态；顶点编号 1-based。
// add_vertex 与 remove_vertex 必须互为逆操作，answer_subtree(u) 调用时状态恰为 u 的子树。
// 复杂度 O(n log n * T_update + n * T_answer)，空间 O(n)；递归深度最坏为 O(n)。
struct DSUOnTree {
    int n = 0;
    vector<vector<int>> graph;
    vector<int> subtree_size, heavy_child, enter, leave, euler;

    explicit DSUOnTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        graph.assign(n + 1, {});
        subtree_size.assign(n + 1, 0);
        heavy_child.assign(n + 1, 0);
        enter.assign(n + 1, 0);
        leave.assign(n + 1, 0);
        euler.clear();
        euler.reserve(n);
    }

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    template<class AddVertex, class RemoveVertex, class AnswerSubtree>
    void build(int root, AddVertex add_vertex, RemoveVertex remove_vertex,
               AnswerSubtree answer_subtree) {
        euler.clear();
        fill(subtree_size.begin(), subtree_size.end(), 0);
        fill(heavy_child.begin(), heavy_child.end(), 0);
        prepare(root, 0);
        solve(root, 0, false, add_vertex, remove_vertex, answer_subtree);
    }

private:
    void prepare(int u, int parent) {
        enter[u] = int(euler.size());
        euler.push_back(u);
        subtree_size[u] = 1;
        for (int v : graph[u]) if (v != parent) {
            prepare(v, u);
            subtree_size[u] += subtree_size[v];
            if (!heavy_child[u] || subtree_size[v] > subtree_size[heavy_child[u]])
                heavy_child[u] = v;
        }
        leave[u] = int(euler.size());
    }

    template<class AddVertex, class RemoveVertex, class AnswerSubtree>
    void solve(int u, int parent, bool keep, AddVertex &add_vertex,
               RemoveVertex &remove_vertex, AnswerSubtree &answer_subtree) {
        for (int v : graph[u])
            if (v != parent && v != heavy_child[u])
                solve(v, u, false, add_vertex, remove_vertex, answer_subtree);

        if (heavy_child[u])
            solve(heavy_child[u], u, true, add_vertex, remove_vertex, answer_subtree);

        for (int v : graph[u]) if (v != parent && v != heavy_child[u])
            for (int i = enter[v]; i < leave[v]; ++i) add_vertex(euler[i]);
        add_vertex(u);
        answer_subtree(u);

        if (!keep)
            for (int i = enter[u]; i < leave[u]; ++i) remove_vertex(euler[i]);
    }
};

// Usage:
// DSUOnTree dsu(n);
// dsu.add_edge(u, v);
// dsu.build(1, add_vertex, remove_vertex, answer_subtree);

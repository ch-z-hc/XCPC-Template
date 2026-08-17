#include <bits/stdc++.h>
using namespace std;

// 二分图最大匹配（Kuhn）；左部点 1..n_left，右部点 1..n_right。
struct BipartiteMatcher {
    int n_left = 0, n_right = 0;
    vector<vector<int>> graph;
    vector<int> match_left, match_right;

    BipartiteMatcher(int left = 0, int right = 0) { init(left, right); }

    void init(int left, int right) {
        n_left = left; n_right = right;
        graph.assign(n_left + 1, {});
        match_left.assign(n_left + 1, 0);
        match_right.assign(n_right + 1, 0);
    }

    void add_edge(int u, int v) { graph[u].push_back(v); }

    bool augment(int u, vector<int> &vis) {
        for (int v : graph[u]) if (!vis[v]) {
            vis[v] = 1;
            if (!match_right[v] || augment(match_right[v], vis)) {
                match_left[u] = v;
                match_right[v] = u;
                return true;
            }
        }
        return false;
    }

    int max_matching() {
        fill(match_left.begin(), match_left.end(), 0);
        fill(match_right.begin(), match_right.end(), 0);
        int answer = 0;
        for (int u = 1; u <= n_left; ++u) {
            vector<int> vis(n_right + 1, 0);
            answer += augment(u, vis);
        }
        return answer;
    }
};

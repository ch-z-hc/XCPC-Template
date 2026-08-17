#include <bits/stdc++.h>
using namespace std;

// 有向图欧拉路。edges 按输入顺序编号，返回顶点序列；不存在时返回空。
vector<int> directed_euler_trail(int n, const vector<pair<int, int>> &edges) {
    vector<vector<int>> graph(n + 1);
    vector<int> in(n + 1), out(n + 1), ptr(n + 1);
    for (int i = 0; i < int(edges.size()); ++i) {
        auto [u, v] = edges[i]; graph[u].push_back(v);
        ++out[u]; ++in[v];
    }
    int start = 1, plus = 0, minus = 0;
    for (int u = 1; u <= n; ++u) {
        if (out[u] - in[u] == 1) { start = u; ++plus; }
        else if (in[u] - out[u] == 1) ++minus;
        else if (in[u] != out[u]) return {};
    }
    if (!((plus == 1 && minus == 1) || (plus == 0 && minus == 0))) return {};
    if (plus == 0) {
        while (start <= n && graph[start].empty()) ++start;
        if (start > n) return edges.empty() ? vector<int>{1} : vector<int>{};
    }
    vector<int> answer;
    vector<int> st{start};
    while (!st.empty()) {
        int u = st.back();
        if (ptr[u] < int(graph[u].size())) st.push_back(graph[u][ptr[u]++]);
        else answer.push_back(u), st.pop_back();
    }
    if (int(answer.size()) != int(edges.size()) + 1) return {};
    reverse(answer.begin(), answer.end());
    return answer;
}

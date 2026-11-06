#include <bits/stdc++.h>

#include "../../template/ds/李超线段树.cpp"
#include "../../template/ds/动态开点线段树合并与分裂.cpp"
#include "../../template/graph/桥.cpp"
#include "../../template/graph/割点.cpp"
#include "../../template/graph/最大流.cpp"
#include "../../template/graph/势能最短路费用流.cpp"
#include "../../template/graph/二分图匹配.cpp"
#include "../../template/graph/容量缩放费用流.cpp"
#include "../../template/graph/差分约束.cpp"
#include "../../template/math/最短线性递推.cpp"
#include "../../template/math/整除分块.cpp"
#include "../../template/string/多模式匹配自动机.cpp"

int main() {
    {
        MergeSegTree st(INT_MAX);
        int a = 0, b = 0;
        st.add(a, 1, 2); st.add(a, INT_MAX - 1, 3); st.add(a, INT_MAX, 4);
        st.add(b, INT_MAX - 1, 5); st.add(b, INT_MAX, 6);
        assert(st.query(a, 1, INT_MAX) == 9);
        assert(st.query(a, INT_MAX - 1, INT_MAX) == 7);
        assert(st.kth(a, 1) == 1 && st.kth(a, 3) == INT_MAX - 1);
        assert(st.kth(a, 9) == INT_MAX && st.kth(a, 10) == -1);
        st.merge(a, b);
        assert(b == 0 && st.query(a, 1, INT_MAX) == 20);
        assert(st.query(a, INT_MAX - 1, INT_MAX - 1) == 8);
        b = st.split(a, INT_MAX - 1, INT_MAX - 1);
        assert(st.query(a, 1, INT_MAX) == 12 && st.query(b, 1, INT_MAX) == 8);
        assert(st.kth(a, 3) == INT_MAX && st.kth(b, 8) == INT_MAX - 1);
        st.add(b, INT_MAX - 1, -3);
        st.merge(a, b);
        assert(b == 0 && st.query(a, INT_MAX - 1, INT_MAX) == 15);
        b = st.split(a, 1, INT_MAX);
        assert(a == 0 && st.query(b, 1, INT_MAX) == 17);
        st.init(1);
        a = 0; st.add(a, 1, 7);
        assert(st.query(a, 1, 1) == 7 && st.kth(a, 7) == 1);
    }
    {
        assert(div_blocks(0).empty());
        for (ll n : {1LL, 2LL, 1000000000000LL}) {
            auto blocks = div_blocks(n);
            ll next = 1;
            for (auto [l, r] : blocks) {
                assert(l == next && l <= r && r <= n);
                assert(n / l == n / r && r == n / (n / l));
                if (r != n) assert(n / (r + 1) != n / l);
                next = r == n ? 0 : r + 1;
            }
            assert(next == 0 && blocks.back().second == n);
        }
    }
    {
        Diff dc(2);
        V < ll > dis;
        dc.add(1, 1, -4000000000000000000LL);
        assert(! dc.solve(dis));
        dc.init(2);
        dc.add(2, 1, LLONG_MIN / 4);
        dc.add(1, 2, LLONG_MAX / 4);
        assert(! dc.solve(dis));
        dc.init(2);
        dc.add(2, 1, LLONG_MIN);
        assert(dc.solve(dis) && dis[2] == LLONG_MIN);
        assert(dc.solve(dis) && dis[2] == LLONG_MIN);
        dc.init(2);
        dc.add(2, 1, -5000000000000000000LL);
        dc.add(1, 2, 5000000000000000000LL);
        assert(dc.solve(dis));
        for (auto [u, v, w] : dc.edges)
            assert(dis[u] <= dis[v] + w);
        dc.init(3);
        dc.add(2, 1, -3000000000000000000LL);
        dc.add(3, 2, -3000000000000000000LL);
        assert(dc.solve(dis) && dis[3] == -6000000000000000000LL);
        for (auto [u, v, w] : dc.edges)
            assert(dis[u] <= dis[v] + w);
        dc.init(3);
        dc.add(2, 1, -LLONG_MAX / 3);
        dc.add(3, 2, -LLONG_MAX / 3);
        assert(dc.solve(dis));
        assert(dis[1] == 0 && dis[2] == -LLONG_MAX / 3);
        assert(dis[3] == 2 * (-LLONG_MAX / 3));
        dc.init(3);
        dc.add(2, 3, -1);
        dc.add(3, 2, 0);
        assert(! dc.solve(dis));
        dc.init(0);
        assert(dc.solve(dis) && dis == V < ll >{0});
    }
    {
        MCMF mcf(2);
        mcf.add_edge(1, 2, 1, 1LL << 62);
        assert(mcf.flow(1, 2) == std::make_pair(1LL, 1LL << 62));
        assert(mcf.flow(1, 2) == std::make_pair(0LL, 0LL));
        mcf.init(3);
        mcf.add_edge(1, 2, 1, -3000000000000000000LL);
        mcf.add_edge(2, 3, 1, 0);
        mcf.add_edge(1, 3, 1, 3000000000000000000LL);
        assert(mcf.flow(1, 3) == std::make_pair(2LL, 0LL));
        const ll lim = std::numeric_limits<ll>::max();
        mcf.init(2);
        mcf.add_edge(1, 2, lim, 1);
        assert(mcf.flow(1, 2) == std::make_pair(lim, lim));
        mcf.init(2);
        mcf.add_edge(1, 2, 1, lim);
        assert(mcf.flow(1, 2) == std::make_pair(1LL, lim));
        mcf.init(4);
        // 两条路径费用抵消，但中间路径费用也必须能放入 ll。
        mcf.add_edge(1, 2, 1, -lim / 8);
        mcf.add_edge(2, 4, 1, -lim / 8);
        mcf.add_edge(1, 3, 1, lim / 8);
        mcf.add_edge(3, 4, 1, lim / 8);
        assert(mcf.flow(1, 4) == std::make_pair(2LL, 0LL));
    }
    {
        Kuhn match(40000, 40000);
        match.add_edge(1, 40000);
        assert(match.solve() == 1);
        assert(match.solve() == 1);
        match.init(2, 2);
        match.add_edge(1, 1); match.add_edge(1, 2); match.add_edge(2, 1);
        assert(match.solve() == 2);
    }
    {
        LiChaoTree tree(4);
        int id = tree.add_line_real(0, -1e20L);
        assert(tree.query_id(2) == id);
        assert(tree.query(2) == -1e20L);
    }
    {
        const long long mod = 1000000007;
        assert(kth({3}, {2}, 5, mod) == 96);
        assert(kth({0, 1}, {2, 3}, 2, mod) == 2);
    }
    {
        AC ac;
        ac.insert("a");
        ac.build();
        assert((ac.query("aa") == std::vector<long long>{2}));
        ac.build();
        ac.insert("aa");
        ac.build();
        assert((ac.query("aa") == std::vector<long long>{2, 1}));
    }
    {
        Bridge finder(2);
        finder.add_edge(1, 1, 2);
        assert((finder.find() == std::vector<int>{1}));
        finder.add_edge(2, 1, 2);
        assert(finder.find().empty());
    }
    {
        CutVertex finder(3);
        finder.add_edge(1, 2);
        finder.add_edge(2, 3);
        assert((finder.find() == std::vector<int>{2}));
        finder.add_edge(1, 3);
        assert(finder.find().empty());
    }
    {
        Dinic flow(1);
        assert(flow.flow(1, 1) == 0);
        ScalingMCMF cost_flow(1);
        assert(cost_flow.flow(1, 1) == std::make_pair(0LL, 0LL));
    }
    return 0;
}

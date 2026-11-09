#include "../../template/others/决策单调性.cpp"

const ll inf = 1ll << 62;
mt19937 rng(20261110);

int rnd(int l, int r) { return l + rng() % (r - l + 1); }

template < class Cost >
void check(int n, Cost cost, ll base) {
    auto checked_cost = [&](int l, int r) {
        assert(1 <= l && l <= r && r <= n);
        return cost(l, r);
    };
    V < ll > truth(n + 1, inf);
    truth[0] = base;
    for (int i = 1; i <= n; ++ i)
        for (int j = 0; j < i; ++ j)
            truth[i] = min(truth[i], truth[j] + cost(j + 1, i));
    assert(binary_stack_dp(n, checked_cost, base) == truth);
    assert(binary_stack_dp(n, checked_cost, base) == truth);

    V < ll > prev(n + 1, inf);
    prev[0] = base;
    for (int t = 0; t < min(n + 2, 8); ++ t) {
        truth.assign(n + 1, inf);
        for (int i = 1; i <= n; ++ i)
            for (int j = 0; j < i; ++ j) if (prev[j] != inf)
                truth[i] = min(truth[i], prev[j] + cost(j + 1, i));
        assert(dc_dp(prev, checked_cost) == truth);
        prev = truth;
    }
    // 任意上一层值与不可达位置；加入行常数仍保持 Monge 性质。
    for (ll &x : prev) x = rnd(0, 3) ? rnd(-100, 100) : inf;
    truth.assign(n + 1, inf);
    for (int i = 1; i <= n; ++ i)
        for (int j = 0; j < i; ++ j) if (prev[j] != inf)
            truth[i] = min(truth[i], prev[j] + cost(j + 1, i));
    assert(dc_dp(prev, checked_cost) == truth);
    assert(dc_dp(V < ll >(n + 1, inf), checked_cost) == V < ll >(n + 1, inf));
}

int main() {
    for (int n = 0; n <= 20; ++ n) {
        check(n, [](int, int) { return 0ll; }, 0);
        check(n, [](int, int) { return -7ll; }, 13);
        check(n, [](int, int) { return 7ll; }, -13);
        check(n, [](int l, int r) { return 1000000000000ll * (r - l + 1); }, 0);
    }
    for (int t = 0; t < 2000; ++ t) {
        int n = rnd(0, 60);
        V < ll > sum(n + 1);
        for (int i = 1; i <= n; ++ i) sum[i] = sum[i - 1] + rnd(0, 10);
        ll penalty = rnd(-200, 200);
        check(n, [&](int l, int r) {
            ll s = sum[r] - sum[l - 1];
            return s * s + penalty;
        }, rnd(-100, 100));

        // 相邻混合差分非正，再加任意行列常数，构造一般 Monge 矩阵。
        V < V < ll > > a(n + 1, V < ll >(n + 1));
        V < ll > row(n + 1), col(n + 1);
        for (int i = 0; i <= n; ++ i) row[i] = rnd(-200, 200), col[i] = rnd(-200, 200);
        for (int j = 1; j <= n; ++ j)
            for (int i = 1; i <= n; ++ i)
                a[j][i] = a[j - 1][i] + a[j][i - 1] - a[j - 1][i - 1] - rnd(0, 4);
        check(n, [&](int l, int r) { return a[l - 1][r] + row[l - 1] + col[r]; }, 0);
    }
    // 单调递推、大段覆盖及长候选栈；调用次数排除退化成平方枚举。
    int n = 200000;
    for (ll penalty : {0ll, 1000000000000ll}) {
        ll calls = 0;
        auto cost = [&](int l, int r) {
            assert(1 <= l && l <= r && r <= n);
            ++ calls;
            ll s = r - l + 1;
            return s * s + penalty;
        };
        auto f = binary_stack_dp(n, cost);
        for (int i = 1; i <= n; ++ i)
            assert(f[i] == (penalty ? 1ll * i * i + penalty : i));
        assert(calls < 100ll * n);
    }
    ll calls = 0;
    auto plateau = [&](int l, int r) {
        ++ calls;
        ll s = max(0, r - l + 1 - n / 2);
        return s * s;
    };
    assert(binary_stack_dp(n, plateau) == V < ll >(n + 1));
    assert(calls < 100ll * n);
    cout << "PASS: monotone DP brute force, ties, unreachable states and scale\n";
}

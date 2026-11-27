/*
用指定种子生成随机数。sample 从 [0, n) 均匀取 k 个不同整数并随机排列，期望 O(k)。
tree 用均匀 Prüfer 序列生成 1-based 有标号树，n >= 1，时间 O(n log n)。mix 是带掩码的异或移位。
*/

#include <bits/stdc++.h>

#define V vector
#define se second
#define pii pair < int, int >

typedef unsigned long long ull;

using namespace std;

struct Sampler {
    mt19937_64 rng;
    explicit Sampler(ull seed) : rng(seed) {}

    int integer(int l, int r) {
        return uniform_int_distribution < int >(l, r)(rng);
    }
    double real(double l, double r) {
        return uniform_real_distribution < double >(l, r)(rng);
    }
    static ull mix(ull x, ull mask) {
        x ^= mask;
        x ^= x << 13;
        x ^= x >> 7;
        return (x ^ (x << 17)) ^ mask;
    }
    V < int > sample(int n, int k) {
        unordered_set < int > seen;
        V < int > a;
        for (int i = n - k; i < n; i ++) {
            int x = integer(0, i);
            if (!seen.insert(x).se) seen.insert(x = i);
            a.push_back(x);
        }
        shuffle(a.begin(), a.end(), rng);
        return a;
    }
    V < pii > tree(int n) {
        V < pii > edges;
        if (n == 1) return edges;
        V < int > deg(n + 1, 1), code(n - 2);
        for (int &x : code) deg[x = integer(1, n)] ++;
        priority_queue < int, V < int >, greater < int > > q;
        for (int u = 1; u <= n; u ++) if (deg[u] == 1) q.push(u);
        for (int x : code) {
            edges.emplace_back(q.top(), x);
            q.pop();
            if (-- deg[x] == 1) q.push(x);
        }
        int u = q.top(); q.pop();
        edges.emplace_back(u, q.top());
        return edges;
    }
};

/*
Sampler gen(42);
int x = gen.integer(1, 6); double y = gen.real(0, 1);
auto a = gen.sample(100, 12); auto edges = gen.tree(20);
ull h = Sampler::mix(17, 42);
*/

/*
维护低 63 位的异或线性基，0 <= x < 2^63。插入、判可表示和最大异或 O(63)，第 k 小 O(63²)。
kth 按不同异或值从小到大编号，k 从 0 数，包含空集的 0。无此项返回 false，成功时写入 ans。
*/

#include <bits/stdc++.h>

typedef unsigned long long ull;

using namespace std;

struct Basis {
    array < ull, 63 > p{};

    void clear() { p.fill(0); }
    bool insert(ull x) {
        for (int i = 62; i >= 0; i --)
            if (x >> i & 1ULL) {
                if (p[i])
                    x ^= p[i];
                else {
                    p[i] = x;
                    return true;
                }
            }
        return false;
    }
    bool contains(ull x) {
        for (int i = 62; i >= 0; i --)
            if (x >> i & 1ULL) {
                if (!p[i]) return false;
                x ^= p[i];
            }
        return true;
    }
    ull max_xor(ull x = 0) {
        for (int i = 62; i >= 0; i --) x = max(x, x ^ p[i]);
        return x;
    }

    bool kth(ull k, ull &ans) {
        auto a = p;
        int n = 0;
        for (int i = 0; i < 63; i ++)
            if (a[i]) {
                for (int j = 0; j < i; j ++)
                    if (a[i] >> j & 1ULL) a[i] ^= a[j];
                n ++;
            }
        if (k >= (1ULL << n)) return false;
        ans = 0;
        for (int i = 0, j = 0; i < 63; i ++)
            if (a[i]) {
                if (k >> j & 1ULL) ans ^= a[i];
                j ++;
            }
        return true;
    }
};

/*
Basis base; bool independent = base.insert(5); base.insert(3);
bool has = base.contains(6); ull largest = base.max_xor(0);
ull x; if (base.kth(1, x)) cout << x;
base.clear();
*/

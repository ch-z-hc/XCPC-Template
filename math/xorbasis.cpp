#include <bits/stdc++.h>
using namespace std;

// 64 位线性基；basis 下标对应 bit 0..62，默认处理非负 long long。
// 单次插入、表示判断和最大异或查询均为 O(63)，空间 O(63)。
struct XorBasis {
    array<unsigned long long, 63> basis{};

    void clear() { basis.fill(0); }
    bool insert(unsigned long long x) {
        for (int i = 62; i >= 0; --i) if (x >> i & 1ULL) {
            if (basis[i]) x ^= basis[i];
            else { basis[i] = x; return true; }
        }
        return false;
    }
    bool can_represent(unsigned long long x) const {
        for (int i = 62; i >= 0; --i) if (x >> i & 1ULL) {
            if (!basis[i]) return false;
            x ^= basis[i];
        }
        return true;
    }
    unsigned long long max_xor(unsigned long long seed = 0) const {
        for (int i = 62; i >= 0; --i) seed = max(seed, seed ^ basis[i]);
        return seed;
    }
};

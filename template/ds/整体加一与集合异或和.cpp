/*
低位优先的二进制字典树，维护可重多重集。单次增删、全体加一 O(32)，异或和 O(1)。
元素用 32 位无符号整数，加一自然回绕。删除前须保证该值存在，重数和总数须适合 int。
*/

#include <bits/stdc++.h>

#define V vector
using namespace std;

struct XorSet {
    struct Node {
        int ch[2]{}, cnt = 0;
        uint32_t val = 0;
    };
    V < Node > tr = V < Node >(2);

    void pull(int u) {
        auto &a = tr[tr[u].ch[0]], &b = tr[tr[u].ch[1]];
        tr[u].cnt = a.cnt + b.cnt;
        tr[u].val = ((a.val ^ b.val) << 1) | (b.cnt & 1);
    }

    void add(uint32_t x, int delta = 1) {
        array < int, 32 > path;
        int u = 1;
        for (int i = 0; i < 32; i ++, x >>= 1) {
            int bit = x & 1;
            path[i] = u;
            if (!tr[u].ch[bit]) {
                tr[u].ch[bit] = int(tr.size());
                tr.emplace_back();
            }
            u = tr[u].ch[bit];
        }
        tr[u].cnt += delta;
        for (int i = 31; i >= 0; i --) pull(path[i]);
    }

    void add_one(int u = 1) {
        if (!tr[u].ch[0] && !tr[u].ch[1]) return;
        swap(tr[u].ch[0], tr[u].ch[1]);
        if (tr[u].ch[0]) add_one(tr[u].ch[0]);
        pull(u);
    }
    uint32_t query() { return tr[1].val; }
};

/*
XorSet s; s.add(3, 2); s.add(3, -1);
s.add_one(); auto x = s.query();
*/

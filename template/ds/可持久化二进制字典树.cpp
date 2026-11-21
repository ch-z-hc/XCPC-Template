#include <bits/stdc++.h>

#define V vector

typedef unsigned long long ull;

using namespace std;

/*
存 64 位无符号整数，根 0 表示空集。insert 返回新根，init 清空所有版本。
max_xor(left, right, x, ans) 在 right 减去 left 的剩余元素中，求与 x 的最大异或值。
left 的每个值在 right 中至少要有相同份数。差集为空时返回 false。
前缀版本用 root[l - 1] 和 root[r] 查 [l, r]。每次操作 O(64)，插入新建 65 个节点。
*/
struct PXorTrie {
    struct Node {
        array < int, 2 > ch{};
        int cnt = 0;
    };
    V < Node > tr;

    PXorTrie() { init(); }
    void init() { tr.assign(1, {}); }

    int insert(int p, ull x) {
        int root = int(tr.size());
        tr.push_back(tr[p]); tr[root].cnt ++;
        int u = root;
        for (int i = 63; i >= 0; -- i) {
            int b = int(x >> i & 1), v = int(tr.size());
            p = tr[p].ch[b];
            tr.push_back(tr[p]); tr[v].cnt ++;
            tr[u].ch[b] = v; u = v;
        }
        return root;
    }

    bool max_xor(int left, int right, ull x, ull &ans) {
        if (tr[left].cnt == tr[right].cnt) return false;
        ans = 0;
        for (int i = 63; i >= 0; -- i) {
            int b = int(x >> i & 1), c = b ^ 1;
            if (tr[tr[right].ch[c]].cnt > tr[tr[left].ch[c]].cnt) ans |= 1ull << i;
            else c = b;
            left = tr[left].ch[c]; right = tr[right].ch[c];
        }
        return true;
    }
};

/*
PXorTrie trie; V < int > root(n + 1);
for (int i = 1; i <= n; ++ i) root[i] = trie.insert(root[i - 1], a[i]);
ull ans;
bool ok = trie.max_xor(root[l - 1], root[r], x, ans);
*/

/*
查询两个版本的多重集之差中与 x 的最大异或值。左版本必须是右版本的子集（含重数）。
每次操作 O(64)，每次插入新建 65 个节点。
根 0 为空版本，init 清空所有版本。版本差为空时 max_xor 返回 false。
*/

#include <bits/stdc++.h>

using u64 = unsigned long long;

struct PXorTrie {
    struct Node {
        std::array<int, 2> ch{};
        int cnt = 0;
    };
    std::vector<Node> tr;

    PXorTrie() { init(); }
    void init() { tr.assign(1, {}); }

    int insert(int p, u64 x) {
        int root = int(tr.size());
        tr.push_back(tr[p]);
        tr[root].cnt ++;
        int u = root;
        for (int i = 63; i >= 0; i --) {
            int b = int(x >> i & 1), v = int(tr.size());
            p = tr[p].ch[b];
            tr.push_back(tr[p]);
            tr[v].cnt ++;
            tr[u].ch[b] = v;
            u = v;
        }
        return root;
    }

    bool max_xor(int left, int right, u64 x, u64 &ans) {
        if (tr[left].cnt == tr[right].cnt) return false;
        ans = 0;
        for (int i = 63; i >= 0; i --) {
            int b = int(x >> i & 1), c = b ^ 1;
            if (tr[tr[right].ch[c]].cnt > tr[tr[left].ch[c]].cnt)
                ans |= 1ull << i;
            else
                c = b;
            left = tr[left].ch[c];
            right = tr[right].ch[c];
        }
        return true;
    }
};

/*
PXorTrie trie; int a = trie.insert(0, 5), b = trie.insert(a, 9);
u64 ans; if (trie.max_xor(a, b, 2, ans)) cout << ans;
trie.init();
*/

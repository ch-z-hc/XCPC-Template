#include <bits/stdc++.h>

#define V vector

using namespace std;

// 64 位无符号整数的可持久化 01 Trie，支持重数；根 0 表示空版本。
// insert(root, x) 返回插入后的新根，不修改旧版本；init 清空全部版本。
// max_xor(left, right, x, ans) 将两个版本的多重集合差与 x 的最大异或值写入 ans。
// 要求 left 是 right 的祖先版本，或其多重集合被 right 包含。
// 空差集返回 false，成功返回 true，此时才能使用 ans。
// 单次插入 / 查询 O(64)，每次插入新增 65 个节点，空间 O(64 * 插入次数)。
// 前缀版本 root[i] 插入 a[i]，闭区间 [l, r] 用 root[l - 1]、root[r] 作差。
struct PersistentXorTrie {
    struct Node { array < int, 2 > ch{}; int cnt = 0; };
    V < Node > tr;

    PersistentXorTrie() { init(); }
    void init() { tr.assign(1, {}); }

    int insert(int p, unsigned long long x) {
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

    bool max_xor(int left, int right, unsigned long long x,
                 unsigned long long &ans) {
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

// 调用：PersistentXorTrie trie; V < int > root(n + 1);
// for (int i = 1; i <= n; ++ i) root[i] = trie.insert(root[i - 1], a[i]);
// unsigned long long ans;
// bool ok = trie.max_xor(root[l - 1], root[r], x, ans);

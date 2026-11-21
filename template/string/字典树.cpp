#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
只接收小写字母，可以插入空串和重复串。count 数完全相同的串，prefix 数以它开头的串。
两种计数都含重复项，clear() 清空整棵树。每次操作 O(串长)，每个节点存 26 个儿子。
*/
struct Trie {
    struct Node {
        array < int, 26 > ch{};
        int cnt = 0, end = 0;
    };

    V < Node > tr;

    Trie() { clear(); }
    void clear() { tr.assign(1, Node{}); }

    void insert(const string &s) {
        int u = 0;
        tr[u].cnt ++;
        for (char c : s) {
            int x = c - 'a';
            if (! tr[u].ch[x]) {
                tr[u].ch[x] = int(tr.size());
                tr.emplace_back();
            }
            u = tr[u].ch[x];
            tr[u].cnt ++;
        }
        tr[u].end ++;
    }

    int find(const string &s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (! tr[u].ch[x]) return -1;
            u = tr[u].ch[x];
        }
        return u;
    }

    int prefix(const string &s) {
        int u = find(s);
        return u == -1 ? 0 : tr[u].cnt;
    }

    int count(const string &s) {
        int u = find(s);
        return u == -1 ? 0 : tr[u].end;
    }
};

/*
Trie trie;
trie.insert("aba");
int cnt = trie.prefix("ab");
*/

#include <bits/stdc++.h>

#define V vector

typedef long long ll;

using namespace std;

// AC 自动机；仅处理小写字母，模式串非空，模式编号和状态编号均为 0-based。
// build 为 O(状态数 * 26)，匹配为 O(文本长度 + 状态数 + 模式数)；空间 O(状态数 * 26)。
// 注意新增模式串后必须重新 build；保留原始儿子以支持重复构建。
struct AC {
    struct Node {
        array < int, 26 > ch{}, go{};
        int fail = 0;
    };

    V < Node > tr;
    V < int > pos, ord;

    AC() { clear(); }

    void clear() {
        tr.assign(1, Node{});
        pos.clear();
        ord.clear();
    }

    int insert(const string &s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (! tr[u].ch[x]) {
                tr[u].ch[x] = int(tr.size());
                tr.emplace_back();
            }
            u = tr[u].ch[x];
        }
        int id = int(pos.size());
        pos.push_back(u);
        return id;
    }

    void build() {
        queue < int > q;
        ord = {0};
        for (auto &p : tr) {
            p.fail = 0;
            p.go = p.ch;
        }
        for (int i = 0; i < 26; ++ i)
            if (tr[0].ch[i]) q.push(tr[0].ch[i]);
        while (! q.empty()) {
            int u = q.front(); q.pop();
            ord.push_back(u);
            for (int i = 0; i < 26; ++ i) {
                int v = tr[u].ch[i];
                if (v) {
                    tr[v].fail = tr[tr[u].fail].go[i];
                    q.push(v);
                } else tr[u].go[i] = tr[tr[u].fail].go[i];
            }
        }
    }

    V < ll > query(const string &s) const {
        V < ll > cnt(tr.size()), ans(pos.size());
        int u = 0;
        for (char c : s) {
            u = tr[u].go[c - 'a'];
            cnt[u] ++;
        }
        for (int i = int(ord.size()) - 1; i > 0; -- i) {
            int v = ord[i];
            cnt[tr[v].fail] += cnt[v];
        }
        for (int i = 0; i < int(pos.size()); ++ i) ans[i] = cnt[pos[i]];
        return ans;
    }
};

// 调用示例：
// AC ac;
// ac.insert("aba");
// ac.build();
// V < ll > cnt = ac.query(s);

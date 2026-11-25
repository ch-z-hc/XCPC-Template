#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
选若干行使每列恰被覆盖一次。列从 1 编号，行号唯一，同一行内不得重复填列，零列对应空解。
建表 O(列数 + 行数 + 非零元数)，搜索最坏指数时间，递归最深为列数。求解后可继续加行或重算。
*/

struct DLX {
    struct Node { int l, r, u, d, col, row; };
    int cols = 0;
    V < Node > tr;
    V < int > siz, cur, ans;

    DLX(int cols = 0) { init(cols); }

    void init(int cols_) {
        cols = cols_;
        tr.resize(cols + 1); siz.assign(cols + 1, 0);
        cur.clear(); ans.clear();
        for (int i = 0; i <= cols; ++ i)
            tr[i] = {i - 1, i + 1, i, i, i, 0};
        tr[0].l = cols; tr[cols].r = 0;
    }

    void add_row(int id, const V < int > &columns) {
        int first = 0;
        for (int c : columns) {
            int u = int(tr.size());
            tr.push_back({u, u, tr[c].u, c, c, id});
            tr[tr[c].u].d = u;
            tr[c].u = u;
            siz[c] ++;
            if (! first) first = u;
            else {
                tr[u].l = tr[first].l; tr[u].r = first;
                tr[tr[first].l].r = u; tr[first].l = u;
            }
        }
    }

    void cover(int c) {
        tr[tr[c].l].r = tr[c].r; tr[tr[c].r].l = tr[c].l;
        for (int i = tr[c].d; i != c; i = tr[i].d)
            for (int j = tr[i].r; j != i; j = tr[j].r) {
                tr[tr[j].u].d = tr[j].d; tr[tr[j].d].u = tr[j].u;
                siz[tr[j].col] --;
            }
    }

    void uncover(int c) {
        for (int i = tr[c].u; i != c; i = tr[i].u)
            for (int j = tr[i].l; j != i; j = tr[j].l) {
                tr[tr[j].u].d = j; tr[tr[j].d].u = j;
                siz[tr[j].col] ++;
            }
        tr[tr[c].l].r = c; tr[tr[c].r].l = c;
    }

    bool dfs() {
        if (! tr[0].r) { ans = cur; return 1; }
        int c = tr[0].r;
        for (int j = tr[c].r; j; j = tr[j].r) if (siz[j] < siz[c]) c = j;
        if (! siz[c]) return 0;
        cover(c);
        for (int i = tr[c].d; i != c; i = tr[i].d) {
            cur.push_back(tr[i].row);
            for (int j = tr[i].r; j != i; j = tr[j].r) cover(tr[j].col);
            bool ok = dfs();
            for (int j = tr[i].l; j != i; j = tr[j].l) uncover(tr[j].col);
            cur.pop_back();
            if (ok) { uncover(c); return 1; }
        }
        uncover(c);
        return 0;
    }

    bool solve(V < int > &rows) {
        cur.clear(); ans.clear();
        if (! dfs()) return false;
        rows = ans;
        return true;
    }
};

/*
DLX dlx; dlx.init(cols); dlx.add_row(id, {c1, c2});
V < int > rows; bool ok = dlx.solve(rows);
*/

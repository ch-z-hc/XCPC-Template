#include <bits/stdc++.h>

template < typename T > 
inline void read(T &cnt) {
    cnt = 0; char ch = getchar(); bool op = 1;
    for (; ! isdigit(ch); ch = getchar())
        if (ch == '-') op = 0;
    for (; isdigit(ch); ch = getchar())
        cnt = cnt * 10 + ch - 48;
    cnt = op ? cnt : - cnt;
}

const int N = 1e6 + 5;

char s[N]; int n, siz[N * 2];

namespace SAM {

    struct SAMnode {
        int trans[26];
        int len, link;
    } t[N * 2];
    int tot = 1, lst = 1;

    inline void extend(int c) {
        int cur = ++ tot, p = lst;
        lst = tot; siz[cur] ++;
        t[cur].len = t[p].len + 1;

        for (; ! t[p].trans[c] && p; p = t[p].link)
            t[p].trans[c] = cur;

        if (! p) t[cur].link = 1;
        else if (t[t[p].trans[c]].len == t[p].len + 1)
            t[cur].link = t[p].trans[c];
        else {
            int q = t[p].trans[c];
            int newq = ++ tot;
            t[newq] = t[q]; t[newq].len = t[p].len + 1; 
            t[cur].link = t[q].link = newq;
            for (; t[p].trans[c] == q; p = t[p].link)
                t[p].trans[c] = newq;
        }
    }
}

int nxt[N * 2], to[N * 2], head[N * 2], tot;
long long ans;

inline void add(int u, int v) {
    nxt[++ tot] = head[u];
    head[u] = tot;
    to[tot] = v;
}

inline void dfs(int u) {
    for (int i = head[u]; i; i = nxt[i]) {
        int v = to[i];
        dfs(v);
        siz[u] += siz[v];
    }
    if (siz[u] > 1)
        ans = std::max(ans, 1ll * siz[u] * SAM::t[u].len);
}

int main() {
    scanf("%s", s);
    n = strlen(s);
    for (int i = 0; i < n; ++ i)
        SAM::extend(s[i] - 'a');
    for (int i = 2; i <= SAM::tot; ++ i)
        add(SAM::t[i].link, i);
    dfs(1);
    std::cout << ans << '\n';
    return 0;
}
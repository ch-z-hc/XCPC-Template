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

const int N = 2e6 + 5;

int n; char s[N];
int chg[N];

namespace AC {
    const int C = 26;
    const int SIZE = 2e6 + 5;
    int nClock = 1;
    int ans[SIZE];
    struct node {
        int trans[C], fail;
    } t[SIZE];

    inline void clear() {
        for (int i = 1; i <= nClock; ++ i) {
            ans[i] = 0;
            t[i].fail = 0;
            memset(t[i].trans, 0, sizeof(t[i].trans));
        }
        nClock = 1;
    }

    inline void insert(char *s, int num) {
        int p = 1, len = strlen(s);
        for (int i = 0; i < len; ++ i) {
            int c = s[i] - 'a';
            if (! t[p].trans[c]) t[p].trans[c] = ++ nClock;
            p = t[p].trans[c];
        }
        chg[num] = p;
    }

    inline void build() {
        for (int i = 0; i < 26; ++ i)
            t[0].trans[i] = 1;
        t[1].fail = 0;

        std::queue < int > Q; Q.push(1);

        while (Q.size()) {
            int u = Q.front(); Q.pop();
            for (int i = 0; i < 26; ++ i) {
                if (t[u].trans[i]) {
                    t[t[u].trans[i]].fail = t[t[u].fail].trans[i];
                    Q.push(t[u].trans[i]);
                }
                else t[u].trans[i] = t[t[u].fail].trans[i];
            }
        }
    }

    int head[SIZE], nxt[SIZE], to[SIZE], ru[SIZE], tot;
    inline void add(int u, int v) {
        nxt[++ tot] = head[u];
        head[u] = tot;
        to[tot] = v;
    }

    inline void build_ft() {
        for (int i = 2; i <= nClock; ++ i) {
            ru[t[i].fail] ++;
            add(i, t[i].fail);
        }
    }

    inline void tupo() {
        std::queue < int > Q;
        for (int i = 1; i <= nClock; ++ i)
            if (! ru[i]) Q.push(i);
        while (Q.size()) {
            int u = Q.front(); Q.pop();
            for (int i = head[u]; i; i = nxt[i]) {
                int v = to[i];
                ans[v] += ans[u];
                ru[v] --;
                if (ru[v] == 0) Q.push(v);
            }
        }
    }

    inline void query() {
        int p = 1, len = strlen(s);

        for (int i = 0; i < len; ++ i) {
            int c = s[i] - 'a';
            p = t[p].trans[c];
            ans[p] ++;
        }
        tupo();
        for (int i = 1; i <= n; ++ i) {
            std::cout << ans[chg[i]] << '\n';
        }
    }
}


int main() {
    read(n);
    for (int i = 1; i <= n; ++ i) {
        scanf("%s", s);
        AC::insert(s, i);
    }
    
    AC::build();
    AC::build_ft();

    scanf("%s", s);
    
    AC::query();

    return 0;
}
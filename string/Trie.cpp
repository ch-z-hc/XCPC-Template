#include <bits/stdc++.h>

const int N = 3e6 + 10;

int ch[N][70], bo[N], T = 1;
char s[N];

inline void insert() {
    int len = strlen(s + 1);
    int now = 1;
    for (int i = 1; i <= len; ++ i) {
        int c = s[i];
        if (! ch[now][c])
            ch[now][c] = ++ T;
        now = ch[now][c];
        bo[now] ++;
    }
}

inline int query() {
    int len = strlen(s + 1), now = 1;
    for (int i = 1; i <= len; ++ i) {
        int c = s[i];
        if (! ch[now][c]) return 0;
        now = ch[now][c];
    }
    return bo[now];
}


int main() {
    return 0;
}
/*
十进制整数和空白分隔的单词，支持 GCC 宽整数；数值须适合目标类型，无符号类型只读非负数。
读完返回 false。每字符均摊 O(1)；输出在 flush 或析构时写回，同一文件勿混用其他缓冲读写。
*/

#include <bits/stdc++.h>

typedef long long ll;

using namespace std;

struct FastInput {
    FILE *file;
    char buf[1 << 16];
    size_t pos = 0, len = 0;
    explicit FastInput(FILE *f = stdin) : file(f) {}

    int get() {
        if (pos == len) {
            len = fread(buf, 1, sizeof(buf), file);
            pos = 0;
            if (!len) return EOF;
        }
        return (unsigned char)buf[pos ++];
    }

    int next() {
        int c = get();
        while (c != EOF && c <= ' ') c = get();
        return c;
    }

    template < class T > bool read(T &x) {
        int c = next();
        bool neg = c == '-';
        if (neg || c == '+') c = get();
        if (c < '0' || c > '9') return false;
        x = 0;
        do {
            int d = c - '0';
            x = neg ? x * 10 - d : x * 10 + d;
            c = get();
        } while ('0' <= c && c <= '9');
        return true;
    }

    bool read(string &s) {
        s.clear();
        for (int c = next(); c > ' '; c = get()) s += char(c);
        return !s.empty();
    }
};

struct FastOutput {
    FILE *file;
    char buf[1 << 16];
    size_t pos = 0;
    explicit FastOutput(FILE *f = stdout) : file(f) {}
    ~FastOutput() { flush(); }

    void flush() {
        fwrite(buf, 1, pos, file);
        pos = 0;
    }
    void put(char c) {
        if (pos == sizeof(buf)) flush();
        buf[pos ++] = c;
    }
    void write(const string &s) { for (char c : s) put(c); }
    void write(const char *s) { while (*s) put(*s ++); }
    template < class T > void write(T x) {
        if (x < 0) put('-');
        char s[sizeof(T) * 3 + 1];
        int n = 0;
        do {
            int d = int(x % 10);
            s[n ++] = char('0' + abs(d));
            x /= 10;
        } while (x);
        while (n) put(s[-- n]);
    }
};

/*
FastInput in; FastOutput out;
ll x; string word;
if (in.read(x) && in.read(word)) { out.write(x); out.write(word); out.put('\n'); out.flush(); }
*/

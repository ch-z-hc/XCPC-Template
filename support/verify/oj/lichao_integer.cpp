#include "../../../template/ds/李超线段树.cpp"

// P4097 输入适配；另支持 exact 模式验证大整数端点，不含在线解码。
int main(int argc, char **argv) {
    ios::sync_with_stdio(0); cin.tie(0);
    bool exact = argc == 2 && string(argv[1]) == "exact";
    int n, last = 0; cin >> n;
    LiChaoTree tree(exact ? -20 : 1, exact ? 20 : 39989);
    while (n --) {
        int op; cin >> op;
        if (op == 0) {
            int x; cin >> x;
            if (! exact) x = (x + last - 1) % 39989 + 1;
            last = tree.query_id(x); cout << last << '\n';
        } else {
            int x0, x1; long long y0, y1; cin >> x0 >> y0 >> x1 >> y1;
            if (! exact) {
                x0 = (x0 + last - 1) % 39989 + 1;
                x1 = (x1 + last - 1) % 39989 + 1;
                y0 = (y0 + last - 1) % 1000000000 + 1;
                y1 = (y1 + last - 1) % 1000000000 + 1;
            }
            tree.add_seg(x0, y0, x1, y1);
        }
    }
}

#include "../../../template/ds/李超线段树.cpp"

// P4097 题意回归：数学同值时必须返回最早的编号。
// 修复前返回 2；整数有理数比较应返回 1，失败时以非零状态退出。
// 同一反例也已加入 li_chao_random.cpp 的常规回归。
int main() {
    LiChaoTree tree(1, 39989);
    tree.add_seg(1, 1, 24, 1);
    tree.add_seg(1, 1000000000, 24, 1);
    int id = tree.query_id(24);
    cout << "expected=1 actual=" << id << '\n';
    cout << setprecision(30) << tree.line[1].eval(24) << ' '
         << tree.line[2].eval(24) << '\n';
    return id != 1;
}

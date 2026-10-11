#include <bits/stdc++.h>

using namespace std;

// Linux：先将 gen.cpp、brute.cpp、a.cpp 编译为 gen、brute、a。
// gen 从 argv[1] 读取种子；失败后查看 input.txt 和两份输出。
int main() {
    for (int t = 1; ; ++ t) {
        cout << "Test " << t << endl;
        string cmd = "./gen " + to_string(t) + " > input.txt";
        if (system(cmd.c_str())) return 2;
        if (system("./brute < input.txt > brute.out")) return 2;
        if (system("./a < input.txt > a.out")) return 2;
        if (system("diff -wB brute.out a.out")) {
            cout << "WA\n";
            return 1;
        }
        cout << "AC\n";
    }
}

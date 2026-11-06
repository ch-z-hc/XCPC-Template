#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// Linux：gen、sol、bf 同目录；逐 token 比较，无内置限时。
bool run(string name, string args, int seed) {
    string cmd = "./" + name + " " + args;
    if (system(cmd.c_str()) == 0) return true;
    cerr << "RE " << name << " seed=" << seed << '\n';
    return false;
}

int main() {
    for (int i = 1; ; ++ i) {
        if (! run("gen", to_string(i) + " > in.txt", i) ||
            ! run("sol", "< in.txt > out.txt", i) ||
            ! run("bf", "< in.txt > ans.txt", i)) return 2;
        ifstream out("out.txt"), ans("ans.txt");
        if (! out || ! ans) {
            cerr << "RE open " << (! out ? "out.txt" : "ans.txt")
                 << " seed=" << i << '\n';
            return 2;
        }
        string x, y;
        while (true) {
            bool a = bool(out >> x), b = bool(ans >> y);
            if (out.bad() || ans.bad()) {
                cerr << "RE read output seed=" << i << '\n';
                return 2;
            }
            if (a != b || (a && x != y)) {
                cout << "WA seed=" << i << '\n';
                return 1;
            }
            if (! a) break;
        }
        if (i % 100 == 0) cout << "AC " << i << endl;
    }
}

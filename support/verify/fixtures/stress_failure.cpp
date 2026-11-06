#include <bits/stdc++.h>

using namespace std;

// 对拍工具自身的故障注入，不是算法模板。
int main(int, char** argv) {
    const char *mode = getenv("STRESS_FIXTURE");
    if (! mode) return 0;
    cerr << "fixture stderr\n";
    if (string(mode) == "tokens") {
        bool sol = filesystem::path(argv[0]).stem() == "sol";
        const char *output = getenv(sol ? "STRESS_SOL_OUTPUT" : "STRESS_BF_OUTPUT");
        if (output) cout << output;
        return 0;
    }
    if (string(mode) == "remove_output") {
        remove("out.txt");
        return 0;
    }
    if (string(mode) == "crash") {
        cout << "partial" << flush;
        return 7;
    }
    if (string(mode) == "timeout") {
        cout << "partial" << flush;
        while (1) this_thread::sleep_for(chrono::milliseconds(10));
    }
    cout << "wrong\n";
}

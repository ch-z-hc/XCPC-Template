#include <bits/stdc++.h>

using namespace std;

// 对拍工具自身的故障注入，不是算法模板。
int main() {
    const char *mode = getenv("STRESS_FIXTURE");
    if (! mode) return 0;
    cerr << "fixture stderr\n";
    if (string(mode) == "crash") return 7;
    if (string(mode) == "timeout") {
        cout << "partial" << flush;
        while (1) this_thread::sleep_for(chrono::milliseconds(10));
    }
    cout << "wrong\n";
}

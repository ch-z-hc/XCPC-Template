#include <bits/stdc++.h>

using namespace std;

// 对拍数据生成器示例：生成一个非空整数序列，用命令行参数指定随机种子。
// 每次只向标准输出写入一组测试数据；调试信息应写入标准错误。
int main(int argc, char *argv[]) {
    uint64_t seed = 1;
    if (argc >= 2) seed = stoull(argv[1]);

    mt19937_64 rng(seed);
    uniform_int_distribution < int > gen_n(1, 30), gen_a(-100, 100);

    int n = gen_n(rng);
    cout << n << '\n';
    for (int i = 1; i <= n; ++ i) {
        if (i > 1) cout << ' ';
        cout << gen_a(rng);
    }
    cout << '\n';
    return 0;
}

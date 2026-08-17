#include <cstdint>
#include <iostream>
#include <random>
#include <string>

// 对拍数据生成器示例：生成一个非空整数序列，用命令行参数指定随机种子。
// 每次只向标准输出写入一组测试数据；调试信息应写入标准错误。
int main(int argc, char *argv[]) {
    std::uint64_t seed = 1;
    if (argc >= 2) seed = std::stoull(argv[1]);

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> length_distribution(1, 30);
    std::uniform_int_distribution<int> value_distribution(-100, 100);

    int n = length_distribution(rng);
    std::cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) std::cout << ' ';
        std::cout << value_distribution(rng);
    }
    std::cout << '\n';
    return 0;
}

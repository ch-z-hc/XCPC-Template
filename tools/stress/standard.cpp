#include <algorithm>
#include <iostream>
#include <limits>

// 对拍标准程序示例：以 O(n) 计算非空最大子段和。
int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    long long best = std::numeric_limits<long long>::lowest();
    long long ending_here = 0;
    for (int i = 0; i < n; ++i) {
        long long value;
        std::cin >> value;
        ending_here = std::max(value, ending_here + value);
        best = std::max(best, ending_here);
    }
    std::cout << best << '\n';
    return 0;
}

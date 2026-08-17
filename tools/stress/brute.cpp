#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

// 对拍暴力程序示例：以 O(n^2) 计算非空最大子段和。
int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<long long> values(n);
    for (long long &value : values) std::cin >> value;

    long long answer = std::numeric_limits<long long>::lowest();
    for (int left = 0; left < n; ++left) {
        long long sum = 0;
        for (int right = left; right < n; ++right) {
            sum += values[right];
            answer = std::max(answer, sum);
        }
    }
    std::cout << answer << '\n';
    return 0;
}

#include "../../../../../template/string/字符串匹配.cpp"

int main() {
    std::ios::sync_with_stdio(0); std::cin.tie(0);
    std::string s, t; std::getline(std::cin, s); std::getline(std::cin, t);
    auto ans = kmp(s, t); std::cout << ans.size() << '\n';
    for (int x : ans) std::cout << x << ' ';
    std::cout << '\n';
}

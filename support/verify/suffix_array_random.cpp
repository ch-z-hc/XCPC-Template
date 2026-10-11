#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>

#include "../../template/string/SuffixArray.cpp"

int main() {
    std::mt19937 rng(19260817);
    SA suffix_array;

    for (int test = 0; test < 500; ++test) {
        int n = int(rng() % 31);
        std::string text(n, 'a');
        for (char &ch : text) ch = char('a' + rng() % 4);
        suffix_array.build(text);

        std::vector<int> expected(n);
        for (int i = 0; i < n; ++i) expected[i] = i;
        std::sort(expected.begin(), expected.end(), [&](int x, int y) {
            return text.substr(x) < text.substr(y);
        });
        assert(suffix_array.sa == expected);

        for (int position = 0; position < n; ++position)
            assert(suffix_array.sa[suffix_array.rk[position]] == position);

        for (int x = 0; x < n; ++x) for (int y = 0; y < n; ++y) {
            int expected_lcp = 0;
            while (x + expected_lcp < n && y + expected_lcp < n &&
                   text[x + expected_lcp] == text[y + expected_lcp])
                ++expected_lcp;
            assert(suffix_array.lcp(x, y) == expected_lcp);
        }
    }
    return 0;
}

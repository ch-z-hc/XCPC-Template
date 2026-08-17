#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <vector>

#include "../ds/LiChao_SegT.cpp"

struct BruteLine {
    long double slope, intercept;
    int left, right;
};

int main() {
    constexpr int domain_left = -20;
    constexpr int domain_right = 20;
    std::mt19937 rng(821731);

    for (int test = 0; test < 300; ++test) {
        LiChaoTree tree(domain_left, domain_right);
        std::vector<BruteLine> lines(1);

        for (int operation = 0; operation < 100; ++operation) {
            if (rng() % 2 == 0) {
                int slope = int(rng() % 41) - 20;
                int intercept = int(rng() % 101) - 50;
                int id = tree.add_line(slope, intercept);
                lines.push_back({static_cast<long double>(slope),
                                 static_cast<long double>(intercept),
                                 domain_left, domain_right});
                assert(id == int(lines.size()) - 1);
            } else {
                int x0 = int(rng() % 61) - 30;
                int x1 = int(rng() % 61) - 30;
                int y0 = int(rng() % 101) - 50;
                int y1 = int(rng() % 101) - 50;
                int id = tree.add_segment(x0, y0, x1, y1);
                if (x0 > x1) {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                long double slope = x0 == x1 ? 0.0L
                                              : static_cast<long double>(y1 - y0) / (x1 - x0);
                long double intercept = x0 == x1 ? std::max(y0, y1) : y0 - slope * x0;
                lines.push_back({slope, intercept, std::max(domain_left, x0),
                                 std::min(domain_right, x1)});
                assert(id == int(lines.size()) - 1);
            }

            for (int x = domain_left; x <= domain_right; ++x) {
                int expected_id = 0;
                long double expected_value = -std::numeric_limits<long double>::infinity();
                for (int id = 1; id < int(lines.size()); ++id) {
                    if (x < lines[id].left || lines[id].right < x) continue;
                    long double value = lines[id].slope * x + lines[id].intercept;
                    if (expected_id == 0 || value > expected_value + 1e-9L ||
                        (std::abs(value - expected_value) < 1e-9L && id < expected_id)) {
                        expected_id = id;
                        expected_value = value;
                    }
                }
                assert(tree.query_line(x) == expected_id);
                if (expected_id)
                    assert(std::abs(tree.query(x) - expected_value) < 1e-9L);
                else
                    assert(std::isinf(tree.query(x)) && tree.query(x) < 0);
            }
        }

        tree.clear();
        assert(tree.query_line(0) == 0);
    }
    return 0;
}

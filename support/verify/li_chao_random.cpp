#include <cassert>
#include <limits>
#include <random>
#include <vector>

#include "../../template/ds/LiChaoTree.cpp"

struct BruteLine {
    long double slope, intercept;
    int left, right;
    LiChaoInt a = 0, c = 0;
    long long d = 0;
};

void check(const LiChaoTree &tree, const std::vector<BruteLine> &lines,
           int left, int right) {
    for (int x = left; x <= right; ++x) {
        int expected_id = 0;
        long double expected_value = -std::numeric_limits<long double>::infinity();
        for (int id = 1; id < int(lines.size()); ++id) {
            if (x < lines[id].left || lines[id].right < x) continue;
            const auto &s = lines[id];
            long double value = s.d ? static_cast<long double>(s.a * x + s.c) / s.d
                                   : s.slope * x + s.intercept;
            bool take = expected_id == 0 || value > expected_value;
            if (expected_id && s.d && lines[expected_id].d) {
                const auto &t = lines[expected_id];
                take = (s.a * x + s.c) * t.d > (t.a * x + t.c) * s.d;
            }
            if (take) {
                expected_id = id;
                expected_value = value;
            }
        }
        assert(tree.query_id(x) == expected_id);
        assert(tree.query(x) == expected_value);
    }
}

int main() {
    constexpr int domain_left = -20;
    constexpr int domain_right = 20;
    std::mt19937 rng(821731);

    {
        LiChaoTree tree(-3, 3);
        int id = tree.add_seg(std::numeric_limits<int>::min(),
                                  std::numeric_limits<int>::min(),
                                  std::numeric_limits<int>::max(),
                                  std::numeric_limits<int>::max());
        for (int x = -3; x <= 3; ++x) {
            assert(tree.query_id(x) == id);
            assert(tree.query(x) == x);
        }
    }
    {
        LiChaoTree tree(-3, 3);
        std::vector<BruteLine> lines(1);
        for (auto [slope, intercept] : std::vector<std::pair<long double, long double>>{
                 {-1.4e-9L, -1.2e-9L}, {-0.6e-9L, -1.6e-9L}, {-1.6e-9L, 0.2e-9L}}) {
            tree.add_line_real(slope, intercept);
            lines.push_back({slope, intercept, -3, 3});
        }
        check(tree, lines, -3, 3);
        assert(tree.query_id(1) == 3);
        int duplicate = tree.add_line_real(-1.6e-9L, 0.2e-9L);
        assert(duplicate == 4 && tree.query_id(1) == 3);
    }

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
                                 domain_left, domain_right, slope, intercept, 1});
                assert(id == int(lines.size()) - 1);
            } else {
                int x0 = int(rng() % 61) - 30;
                int x1 = int(rng() % 61) - 30;
                int y0 = int(rng() % 101) - 50;
                int y1 = int(rng() % 101) - 50;
                int id = tree.add_seg(x0, y0, x1, y1);
                if (x0 > x1) {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                long double slope = x0 == x1 ? 0.0L
                                              : (static_cast<long double>(y1) - y0) /
                                                (static_cast<long double>(x1) - x0);
                long double intercept = x0 == x1 ? std::max(y0, y1) : y0 - slope * x0;
                lines.push_back({slope, intercept, std::max(domain_left, x0),
                                 std::min(domain_right, x1)});
                auto &s = lines.back();
                s.d = x0 == x1 ? 1 : x1 - x0;
                s.a = x0 == x1 ? 0 : y1 - y0;
                s.c = x0 == x1 ? std::max(y0, y1) : LiChaoInt(y0) * x1 - LiChaoInt(y1) * x0;
                assert(id == int(lines.size()) - 1);
            }

            check(tree, lines, domain_left, domain_right);
        }

        tree.clear();
        assert(tree.query_id(0) == 0);
    }

    for (int test = 0; test < 300; ++test) {
        LiChaoTree tree(domain_left, domain_right);
        std::vector<BruteLine> lines(1);
        for (int operation = 0; operation < 100; ++operation) {
            long double slope = (int(rng() % 81) - 40) * 1e-10L;
            long double intercept = (int(rng() % 81) - 40) * 1e-10L;
            tree.add_line_real(slope, intercept);
            lines.push_back({slope, intercept, domain_left, domain_right});
            check(tree, lines, domain_left, domain_right);
        }
    }

    for (int test = 0; test < 100; ++test) {
        LiChaoTree tree(domain_left, domain_right);
        std::vector<BruteLine> lines(1);
        for (int operation = 0; operation < 50; ++operation) {
            int x0 = std::numeric_limits<int>::min() + int(rng() % 1000);
            int x1 = std::numeric_limits<int>::max() - int(rng() % 1000);
            long double y0 = int(rng() % 101) - 50;
            long double y1 = int(rng() % 101) - 50;
            if (rng() % 2) {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            tree.add_seg_real(x0, y0, x1, y1);
            if (x0 > x1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            long double slope = (y1 - y0) /
                                (static_cast<long double>(x1) - x0);
            lines.push_back({slope, y0 - slope * x0, domain_left, domain_right});
            check(tree, lines, domain_left, domain_right);
        }
    }
    {
        LiChaoTree tree(1, 39989);
        tree.add_seg(1, 1, 24, 1);
        tree.add_seg(1, 1000000000, 24, 1);
        assert(tree.query_id(24) == 1);
        tree.clear();
        tree.add_seg(1, 1LL, 39989, 39989LL);
        tree.add_seg(1, 39989LL, 39989, 1LL);
        assert(tree.query_id(19995) == 1);
        tree.add_seg(19995, 19996, 19995, 19995);
        assert(tree.query_id(19995) == 3);
        tree.clear();
        tree.add_line(1000000000000000000LL, -1000000000000000000LL);
        tree.add_seg(INT_MIN, -1000000000000000000LL,
                     INT_MAX, 1000000000000000000LL);
        assert(tree.query_id(1) == 2 && tree.query_id(39989) == 1);
    }
    return 0;
}

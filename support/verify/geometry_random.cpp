#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

#include "../../template/geometry/Point.cpp"
#include "../../template/geometry/Line.cpp"
#include "../../template/geometry/Polygon.cpp"
#include "../../template/geometry/ConvexHull.cpp"

void check_same_points(const std::vector<P> &a, const std::vector<P> &b) {
    assert(a.size() == b.size());
    for (int i = 0; i < int(a.size()); ++i)
        assert(a[i].x == b[i].x && a[i].y == b[i].y);
}

std::vector<P> brute_hull(std::vector<P> points) {
    if (points.empty()) return {};
    auto less = [](P a, P b) { return a.x < b.x || (a.x == b.x && a.y < b.y); };
    int first = int(std::min_element(points.begin(), points.end(), less) - points.begin());
    int u = first;
    std::vector<P> hull;
    do {
        hull.push_back(points[u]);
        int v = u;
        for (int w = 0; w < int(points.size()); ++w) {
            int turn = crossop(points[u], points[v], points[w]);
            if (v == u || turn < 0 || (turn == 0 &&
                (points[w] - points[u]).abs2() > (points[v] - points[u]).abs2())) v = w;
        }
        u = v;
    } while (u != first);
    return hull;
}

template <class Less>
void check_order(const std::vector<P> &points, Less less) {
    for (P a : points) for (P b : points) for (P c : points) {
        assert(!less(a, a));
        if (less(a, b) && less(b, c)) assert(less(a, c));
        if (!less(a, b) && !less(b, a) && !less(b, c) && !less(c, b))
            assert(!less(a, c) && !less(c, a));
    }
}

int main() {
    {
        assert(area({}) == 0);
        assert(area({{1, 2}}) == 0);
        assert(area({{1, 2}, {3, 4}}) == 0);
        for (db shift : {0.L, 1e6L, 1e9L}) {
            db side = 1.L / 3;
            V < P > square{{shift, shift}, {shift + side, shift},
                          {shift + side, shift + side}, {shift, shift + side}};
            db expected = (square[1].x - square[0].x) *
                          (square[3].y - square[0].y);
            assert(abs(area(square) - expected) < 1e-12L);
            reverse(square.begin(), square.end());
            assert(abs(area(square) + expected) < 1e-12L);
            square.push_back(square[0]);
            assert(abs(area(square) + expected) < 1e-12L);
        }
        V < P > concave{{0, 0}, {4, 0}, {4, 4}, {2, 1}, {0, 4}};
        for (P &p : concave) p = p + P(1e9L, -1e9L);
        assert(area(concave) == 10);
    }
    {
        P a(0, 200), b(0.75L * EPS, 90), c(1.5L * EPS, 0);
        assert(a < b && b < c && a < c);
        assert(cross(P(0, 0) + P(1, 1), P(2, 1), P(1, 2)) == 1);
        std::vector<P> points{{0.5L * EPS, 31}, {1.25L * EPS, 12},
                              {0, 775}, {0.25L * EPS, 919}};
        check_same_points(convexHull(points), brute_hull(points));
        assert(std::abs(area(convexHull(points)) / EPS - 459.625L) < 1e-9L);
        check_same_points(convexHull({{1, 1}, {1, 1}}), {{1, 1}});
        check_same_points(convexHullNonStrict({{0, 0}, {0.5L * EPS, 0}}),
                          {{0, 0}, {0.5L * EPS, 0}});

        std::vector<P> directions{{0, 0}, {1, 0}, {1, 0.75L * EPS},
            {1, 1.5L * EPS}, {-1, 0}, {0, 1}, {0, -1},
            {0.75L * EPS, 1}, {1.5L * EPS, 1}, {-EPS, 1}, {EPS, -1}};
        check_order(directions, [](P a, P b) { return a < b; });
        check_order(directions, polar_cmpX);
        check_order(directions, polar_cmpY);

        std::vector<P> first{{0, 0}, {1, 3}, {3, 2}}, second{{0, 0}, {5, 0}, {4, -2}};
        check_same_points(convexHull(MincowskySum(first, second)),
                          {{0, 0}, {4, -2}, {7, 0}, {8, 2}, {6, 3}, {1, 3}});
    }
    {
        std::vector<P> points{{0, 0}, {1, 0}, {2, 0}, {1, 0}};
        assert((convexHull(points) == std::vector<P>{{0, 0}, {2, 0}}));
        assert((convexHullNonStrict(points) == std::vector<P>{{0, 0}, {1, 0}, {2, 0}, {1, 0}}));
        assert(isSS({0, 0}, {2, 2}, {0, 2}, {2, 0}));
        assert(!isSS_strict({0, 0}, {1, 0}, {1, 0}, {2, 0}));

        std::vector<P> square{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
        assert(checkConvexHull(square));
        assert(std::abs(std::abs(area(convexCut(square, {1, -1}, {1, 3}))) - 2) < 1e-9L);
        std::reverse(square.begin(), square.end());
        assert(!checkConvexHull(square));
        assert(!checkConvexHull({{0, 0}, {2, 0}, {1, 1}, {2, 2}, {0, 2}}));
    }

    std::mt19937 rng(998244353);
    for (int test = 0; test < 1000; ++test) {
        std::vector<P> points;
        int n = int(rng() % 20) + 1;
        for (int i = 0; i < n; ++i)
            points.emplace_back(int(rng() % 21) * EPS / 4, int(rng() % 1000) * 16);
        check_same_points(convexHull(points), brute_hull(points));
    }
    for (int test = 0; test < 200; ++test) {
        std::vector<P> first, second, pair_sums;
        for (int i = 0; i < 8; ++i) {
            first.emplace_back(int(rng() % 21) * EPS / 4, int(rng() % 1000) * 16);
            second.emplace_back(int(rng() % 21) * EPS / 4, int(rng() % 1000) * 16);
        }
        for (P a : first) for (P b : second) pair_sums.push_back(a + b);
        auto expected = brute_hull(pair_sums);
        first = convexHull(first); second = convexHull(second);
        if (rng() & 1) std::reverse(first.begin(), first.end());
        if (rng() & 1) std::reverse(second.begin(), second.end());
        auto actual = convexHull(MincowskySum(first, second));
        assert(actual.size() == expected.size());
        for (int i = 0; i < int(actual.size()); ++i) {
            assert(std::abs(actual[i].x - expected[i].x) < EPS * 1e-6L);
            assert(std::abs(actual[i].y - expected[i].y) < 1e-9L);
        }
    }
    for (int test = 0; test < 500; ++test) {
        std::vector<P> points;
        int n = int(rng() % 30) + 1;
        for (int i = 0; i < n; ++i)
            points.emplace_back(int(rng() % 21) - 10, int(rng() % 21) - 10);

        std::sort(points.begin(), points.end());
        points.erase(std::unique(points.begin(), points.end()), points.end());
        std::vector<P> hull = convexHull(points);
        assert(checkConvexHull(hull));
        for (int i = 0; i < int(hull.size()); ++i)
            for (int j = i + 1; j < int(hull.size()); ++j)
                assert(!(hull[i] == hull[j]));
        if (hull.size() >= 3) {
            for (P point : points)
                assert(contain(hull, point) != 0);
            for (int i = 0; i < int(hull.size()); ++i)
                assert(crossop(hull[i], hull[(i + 1) % hull.size()],
                                   hull[(i + 2) % hull.size()]) > 0);
        }

        long double brute_diameter = 0;
        for (P first : hull) for (P second : hull)
            brute_diameter = std::max(brute_diameter, first.distTo(second));
        assert(std::abs(convexDiameter(hull) - brute_diameter) < 1e-9L);
    }

    for (int test = 0; test < 300; ++test) {
        std::vector<P> first, second, pair_sums;
        for (int i = 0; i < 8; ++i)
            first.emplace_back(int(rng() % 11) - 5, int(rng() % 11) - 5);
        for (int i = 0; i < 8; ++i)
            second.emplace_back(int(rng() % 11) - 5, int(rng() % 11) - 5);
        for (P a : first) for (P b : second) pair_sums.push_back(a + b);
        first = convexHull(first); second = convexHull(second);
        for (int orientation = 0; orientation < 4; ++orientation) {
            auto p = first, q = second;
            if (orientation & 1) std::reverse(p.begin(), p.end());
            if (orientation & 2) std::reverse(q.begin(), q.end());
            std::rotate(p.begin(), p.begin() + rng() % p.size(), p.end());
            std::rotate(q.begin(), q.begin() + rng() % q.size(), q.end());
            auto sum = MincowskySum(p, q);
            check_same_points(convexHull(sum), convexHull(pair_sums));
        }
    }
    return 0;
}

#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

#include "../geometry/geo.cpp"

int main() {
    {
        std::vector<Point> points{{0, 0}, {1, 0}, {2, 0}, {1, 0}};
        assert((convex_hull(points) == std::vector<Point>{{0, 0}, {2, 0}}));
        assert((convex_hull(points, true) == std::vector<Point>{{0, 0}, {1, 0}, {2, 0}}));
        assert(segments_intersect({0, 0}, {2, 2}, {0, 2}, {2, 0}));
        assert(!segments_intersect({0, 0}, {1, 0}, {1, 0}, {2, 0}, true));

        std::vector<Point> square{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
        assert(is_convex_polygon(square));
        std::reverse(square.begin(), square.end());
        assert(is_convex_polygon(square));
        assert(!is_convex_polygon({{0, 0}, {2, 0}, {1, 1}, {2, 2}, {0, 2}}));
    }

    std::mt19937 rng(998244353);
    for (int test = 0; test < 500; ++test) {
        std::vector<Point> points;
        int n = int(rng() % 30) + 1;
        for (int i = 0; i < n; ++i)
            points.emplace_back(int(rng() % 21) - 10, int(rng() % 21) - 10);

        std::vector<Point> hull = convex_hull(points);
        assert(is_convex_polygon(hull));
        for (int i = 0; i < int(hull.size()); ++i)
            for (int j = i + 1; j < int(hull.size()); ++j)
                assert(!(hull[i] == hull[j]));
        if (hull.size() >= 3) {
            for (const Point &point : points)
                assert(point_in_polygon(hull, point) != PointLocation::outside);
            for (int i = 0; i < int(hull.size()); ++i)
                assert(orientation(hull[i], hull[(i + 1) % hull.size()],
                                   hull[(i + 2) % hull.size()]) > 0);
        }

        long double brute_diameter = 0;
        for (const Point &first : hull) for (const Point &second : hull)
            brute_diameter = std::max(brute_diameter, first.distance_to(second));
        assert(std::abs(convex_diameter(hull) - brute_diameter) < 1e-9L);
    }

    for (int test = 0; test < 300; ++test) {
        std::vector<Point> first, second, pair_sums;
        for (int i = 0; i < 8; ++i)
            first.emplace_back(int(rng() % 11) - 5, int(rng() % 11) - 5);
        for (int i = 0; i < 8; ++i)
            second.emplace_back(int(rng() % 11) - 5, int(rng() % 11) - 5);
        for (const Point &a : first) for (const Point &b : second) pair_sums.push_back(a + b);
        assert(minkowski_sum(first, second) == convex_hull(pair_sums));
    }
    return 0;
}

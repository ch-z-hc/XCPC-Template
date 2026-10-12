#include <cmath>
#include <iomanip>
#include <iostream>

#include "../../../template/geometry/Point.cpp"
#include "../../../template/geometry/Line.cpp"
#include "../../../template/geometry/Polygon.cpp"
#include "../../../template/geometry/ConvexHull.cpp"
#include "../../../template/math/AdaptiveSimpson.cpp"

int main() {
    bool ok = true;
    V < P > point{{2, 3}};
    auto left = MincowskySum({}, point), right = MincowskySum(point, {});
    std::cout << "Minkowski empty operand: " << left.size() << ' '
              << right.size() << ", expected 0 0\n";
    ok &= left.empty() && right.empty();
    for (double scale : {1e-308, 1e308}) {
        double l = scale, r = 1.1 * scale;
        long double expected = log(static_cast<long double>(r) / l);
        double actual = simpson([](double x) { return 1 / x; }, l, r, 1e-10, 4);
        std::cout << std::setprecision(18) << "Simpson scale=" << scale
                  << " actual=" << actual << " expected=" << expected << '\n';
        ok &= std::isfinite(actual) && std::abs(actual - expected) <= 1e-10L;
    }
    return !ok;
}

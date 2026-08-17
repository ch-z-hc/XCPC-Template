#include <bits/stdc++.h>
using namespace std;

// 二维浮点几何基础；坐标类型为 long double，几何判定使用 EPS。
// Point 的排序和相等运算使用精确坐标，以满足 STL 排序要求；近似比较请使用 almost_equal。
// 凸包按逆时针顺序返回且不重复首点，点和容器下标均为 0-based。
using GeometryReal = long double;
constexpr GeometryReal GEOMETRY_EPS = 1e-9L;

int geometry_sign(GeometryReal value) {
    return value < -GEOMETRY_EPS ? -1 : value > GEOMETRY_EPS ? 1 : 0;
}

bool almost_equal(GeometryReal first, GeometryReal second) {
    return geometry_sign(first - second) == 0;
}

struct Point {
    GeometryReal x = 0, y = 0;

    Point() = default;
    Point(GeometryReal x_, GeometryReal y_) : x(x_), y(y_) {}

    Point operator+(const Point &other) const { return {x + other.x, y + other.y}; }
    Point operator-(const Point &other) const { return {x - other.x, y - other.y}; }
    Point operator*(GeometryReal scale) const { return {x * scale, y * scale}; }
    Point operator/(GeometryReal scale) const { return {x / scale, y / scale}; }

    bool operator<(const Point &other) const {
        return x != other.x ? x < other.x : y < other.y;
    }
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }

    GeometryReal norm_squared() const { return x * x + y * y; }
    GeometryReal norm() const { return sqrtl(norm_squared()); }
    GeometryReal distance_to(const Point &other) const { return (*this - other).norm(); }
    GeometryReal angle() const { return atan2l(y, x); }
    GeometryReal dot(const Point &other) const { return x * other.x + y * other.y; }
    GeometryReal cross(const Point &other) const { return x * other.y - y * other.x; }
    Point rotate_90() const { return {-y, x}; }
    Point rotate(GeometryReal angle_) const {
        return {x * cosl(angle_) - y * sinl(angle_),
                x * sinl(angle_) + y * cosl(angle_)};
    }
    Point unit() const { return *this / norm(); }
};

ostream &operator<<(ostream &output, const Point &point) {
    return output << point.x << ' ' << point.y;
}

bool almost_equal(const Point &first, const Point &second) {
    return almost_equal(first.x, second.x) && almost_equal(first.y, second.y);
}

GeometryReal cross(const Point &origin, const Point &first, const Point &second) {
    return (first - origin).cross(second - origin);
}

int orientation(const Point &origin, const Point &first, const Point &second) {
    return geometry_sign(cross(origin, first, second));
}

bool lines_parallel(const Point &first_a, const Point &first_b,
                    const Point &second_a, const Point &second_b) {
    return geometry_sign((first_b - first_a).cross(second_b - second_a)) == 0;
}

optional<Point> line_intersection(const Point &first_a, const Point &first_b,
                                  const Point &second_a, const Point &second_b) {
    GeometryReal first_weight = cross(second_a, second_b, first_a);
    GeometryReal second_weight = -cross(second_a, second_b, first_b);
    GeometryReal denominator = first_weight + second_weight;
    if (geometry_sign(denominator) == 0) return nullopt;
    return (first_a * second_weight + first_b * first_weight) / denominator;
}

bool on_segment(const Point &first, const Point &second, const Point &point,
                bool strict = false) {
    if (orientation(first, second, point) != 0) return false;
    int position = geometry_sign((point - first).dot(point - second));
    return strict ? position < 0 : position <= 0;
}

bool segments_intersect(const Point &first_a, const Point &first_b,
                        const Point &second_a, const Point &second_b,
                        bool strict = false) {
    int first_side = orientation(first_a, first_b, second_a);
    int second_side = orientation(first_a, first_b, second_b);
    int third_side = orientation(second_a, second_b, first_a);
    int fourth_side = orientation(second_a, second_b, first_b);
    if (strict) return first_side * second_side < 0 && third_side * fourth_side < 0;
    if (first_side == 0 && on_segment(first_a, first_b, second_a)) return true;
    if (second_side == 0 && on_segment(first_a, first_b, second_b)) return true;
    if (third_side == 0 && on_segment(second_a, second_b, first_a)) return true;
    if (fourth_side == 0 && on_segment(second_a, second_b, first_b)) return true;
    return first_side * second_side < 0 && third_side * fourth_side < 0;
}

bool line_intersects_segment(const Point &line_a, const Point &line_b,
                             const Point &segment_a, const Point &segment_b,
                             bool strict = false) {
    int first_side = orientation(line_a, line_b, segment_a);
    int second_side = orientation(line_a, line_b, segment_b);
    return strict ? first_side * second_side < 0 : first_side * second_side <= 0;
}

Point projection(const Point &line_a, const Point &line_b, const Point &point) {
    Point direction = line_b - line_a;
    if (geometry_sign(direction.norm_squared()) == 0) return line_a;
    return line_a + direction * (direction.dot(point - line_a) / direction.norm_squared());
}

Point reflection(const Point &line_a, const Point &line_b, const Point &point) {
    return projection(line_a, line_b, point) * 2 - point;
}

GeometryReal distance_to_segment(const Point &first, const Point &second,
                                 const Point &point) {
    if (almost_equal(first, second)) return first.distance_to(point);
    Point projected = projection(first, second, point);
    if (on_segment(first, second, projected)) return point.distance_to(projected);
    return min(first.distance_to(point), second.distance_to(point));
}

GeometryReal distance_between_segments(const Point &first_a, const Point &first_b,
                                       const Point &second_a, const Point &second_b) {
    if (segments_intersect(first_a, first_b, second_a, second_b)) return 0;
    return min({distance_to_segment(first_a, first_b, second_a),
                distance_to_segment(first_a, first_b, second_b),
                distance_to_segment(second_a, second_b, first_a),
                distance_to_segment(second_a, second_b, first_b)});
}

GeometryReal directed_angle(const Point &first, const Point &second) {
    return atan2l(first.cross(second), first.dot(second));
}

GeometryReal polygon_signed_area(const vector<Point> &polygon) {
    GeometryReal twice_area = 0;
    for (int i = 0; i < int(polygon.size()); ++i)
        twice_area += polygon[i].cross(polygon[(i + 1) % polygon.size()]);
    return twice_area / 2;
}

enum class PointLocation { outside, boundary, inside };

PointLocation point_in_polygon(const vector<Point> &polygon, const Point &point) {
    bool inside = false;
    for (int i = 0; i < int(polygon.size()); ++i) {
        Point first = polygon[i], second = polygon[(i + 1) % polygon.size()];
        if (on_segment(first, second, point)) return PointLocation::boundary;
        if (geometry_sign(first.y - second.y) > 0) swap(first, second);
        if (geometry_sign(point.y - first.y) >= 0 &&
            geometry_sign(point.y - second.y) < 0 &&
            orientation(point, second, first) > 0)
            inside = !inside;
    }
    return inside ? PointLocation::inside : PointLocation::outside;
}

// keep_collinear 为 true 时保留边界点。复杂度 O(n log n)。
vector<Point> convex_hull(vector<Point> points, bool keep_collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());
    if (points.size() <= 2) return points;

    bool collinear = true;
    for (int i = 2; i < int(points.size()); ++i)
        if (orientation(points[0], points[1], points[i]) != 0) collinear = false;
    if (collinear) return keep_collinear ? points : vector<Point>{points.front(), points.back()};

    vector<Point> hull;
    for (const Point &point : points) {
        while (hull.size() >= 2) {
            int turn = orientation(hull[hull.size() - 2], hull.back(), point);
            if (turn > 0 || (keep_collinear && turn == 0)) break;
            hull.pop_back();
        }
        hull.push_back(point);
    }
    int lower_size = int(hull.size());
    for (int i = int(points.size()) - 2; i >= 0; --i) {
        while (int(hull.size()) > lower_size) {
            int turn = orientation(hull[hull.size() - 2], hull.back(), points[i]);
            if (turn > 0 || (keep_collinear && turn == 0)) break;
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    hull.pop_back();
    return hull;
}

bool is_convex_polygon(const vector<Point> &polygon) {
    int direction = 0;
    for (int i = 0; i < int(polygon.size()); ++i) {
        int turn = orientation(polygon[i], polygon[(i + 1) % polygon.size()],
                               polygon[(i + 2) % polygon.size()]);
        if (turn == 0) continue;
        if (direction && turn != direction) return false;
        direction = turn;
    }
    return true;
}

// 输入为逆时针凸包；旋转卡壳复杂度 O(n)。
GeometryReal convex_diameter(const vector<Point> &hull) {
    int n = int(hull.size());
    if (n <= 1) return 0;
    if (n == 2) return hull[0].distance_to(hull[1]);
    int opposite = 1;
    GeometryReal answer = 0;
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        while (fabsl(cross(hull[i], hull[next], hull[(opposite + 1) % n])) >
               fabsl(cross(hull[i], hull[next], hull[opposite])) + GEOMETRY_EPS)
            opposite = (opposite + 1) % n;
        answer = max({answer, hull[i].distance_to(hull[opposite]),
                      hull[next].distance_to(hull[opposite])});
    }
    return answer;
}

// 用有向直线 line_a -> line_b 切割凸多边形，保留左侧和直线上的部分，复杂度 O(n)。
vector<Point> cut_convex_polygon(const vector<Point> &polygon,
                                 const Point &line_a, const Point &line_b) {
    vector<Point> result;
    for (int i = 0; i < int(polygon.size()); ++i) {
        Point first = polygon[i], second = polygon[(i + 1) % polygon.size()];
        int first_side = orientation(line_a, line_b, first);
        int second_side = orientation(line_a, line_b, second);
        if (first_side >= 0) result.push_back(first);
        if (first_side * second_side < 0)
            result.push_back(*line_intersection(first, second, line_a, line_b));
    }
    return result;
}

// 输入为两个点集；返回其凸包的 Minkowski 和，复杂度 O(n log n + m log m)。
vector<Point> minkowski_sum(vector<Point> first, vector<Point> second) {
    first = convex_hull(move(first));
    second = convex_hull(move(second));
    if (first.empty() || second.empty()) return {};

    auto rotate_to_lowest = [](vector<Point> &polygon) {
        int start = int(min_element(polygon.begin(), polygon.end(),
            [](const Point &a, const Point &b) {
                return a.y != b.y ? a.y < b.y : a.x < b.x;
            }) - polygon.begin());
        rotate(polygon.begin(), polygon.begin() + start, polygon.end());
    };
    rotate_to_lowest(first);
    rotate_to_lowest(second);

    vector<Point> first_edge(first.size()), second_edge(second.size());
    for (int i = 0; i < int(first.size()); ++i)
        first_edge[i] = first[(i + 1) % first.size()] - first[i];
    for (int i = 0; i < int(second.size()); ++i)
        second_edge[i] = second[(i + 1) % second.size()] - second[i];

    vector<Point> result{first[0] + second[0]};
    int i = 0, j = 0;
    while (i < int(first_edge.size()) || j < int(second_edge.size())) {
        Point step;
        if (j == int(second_edge.size())) step = first_edge[i++];
        else if (i == int(first_edge.size())) step = second_edge[j++];
        else {
            int turn = geometry_sign(first_edge[i].cross(second_edge[j]));
            if (turn > 0) step = first_edge[i++];
            else if (turn < 0) step = second_edge[j++];
            else step = first_edge[i++] + second_edge[j++];
        }
        result.push_back(result.back() + step);
    }
    result.pop_back();
    return convex_hull(move(result));
}

// Usage:
// vector<Point> hull = convex_hull(points);
// bool intersect = segments_intersect(a, b, c, d);

#include <bits/stdc++.h>
using namespace std;

// 离散整数横坐标上的最大值 Li Chao Tree；查询横坐标必须位于 [left, right]。
// add_line 为 O(log X)，add_segment 为 O(log^2 X)，query 为 O(log X)，空间 O(X)。
// 相同横坐标处函数值之差小于 1e-9 时，优先返回较早加入的直线编号。
struct LiChaoTree {
    using Value = long double;

    struct Line {
        Value slope = 0, intercept = 0;
        int id = 0;

        Value value_at(int x) const { return slope * x + intercept; }
    };

    int left = 1, right = 0;
    vector<int> tree;
    vector<Line> lines;

    explicit LiChaoTree(int right_ = 0) { init(1, right_); }
    LiChaoTree(int left_, int right_) { init(left_, right_); }

    void init(int right_) { init(1, right_); }

    void init(int left_, int right_) {
        left = left_;
        right = right_;
        int length = max(0, right - left + 1);
        tree.assign(4 * length + 4, 0);
        lines.assign(1, Line{});
    }

    void clear() {
        fill(tree.begin(), tree.end(), 0);
        lines.assign(1, Line{});
    }

    int add_line(Value slope, Value intercept) {
        int id = create_line(slope, intercept);
        if (left <= right) insert_line(1, left, right, id);
        return id;
    }

    int add_segment(int x0, Value y0, int x1, Value y1) {
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        Value slope = x0 == x1 ? 0 : (y1 - y0) / (x1 - x0);
        Value intercept = x0 == x1 ? max(y0, y1) : y0 - slope * x0;
        int id = create_line(slope, intercept);
        int query_left = max(left, x0), query_right = min(right, x1);
        if (query_left <= query_right)
            insert_segment(1, left, right, query_left, query_right, id);
        return id;
    }

    int query_line(int x) const {
        int node = 1, segment_left = left, segment_right = right, answer = 0;
        while (segment_left <= segment_right) {
            if (better(tree[node], answer, x)) answer = tree[node];
            if (segment_left == segment_right) break;
            int middle = segment_left + (segment_right - segment_left) / 2;
            if (x <= middle) {
                node *= 2;
                segment_right = middle;
            } else {
                node = node * 2 + 1;
                segment_left = middle + 1;
            }
        }
        return answer;
    }

    Value query(int x) const {
        int id = query_line(x);
        return id ? lines[id].value_at(x) : -numeric_limits<Value>::infinity();
    }

private:
    int create_line(Value slope, Value intercept) {
        int id = int(lines.size());
        lines.push_back({slope, intercept, id});
        return id;
    }

    bool better(int first, int second, int x) const {
        if (first == 0) return false;
        if (second == 0) return true;
        Value first_value = lines[first].value_at(x);
        Value second_value = lines[second].value_at(x);
        if (abs(first_value - second_value) < 1e-9L) return first < second;
        return first_value > second_value;
    }

    void insert_line(int node, int segment_left, int segment_right, int id) {
        if (tree[node] == 0) {
            tree[node] = id;
            return;
        }
        int middle = segment_left + (segment_right - segment_left) / 2;
        if (better(id, tree[node], middle)) swap(tree[node], id);
        if (segment_left == segment_right) return;
        if (better(id, tree[node], segment_left))
            insert_line(node * 2, segment_left, middle, id);
        else if (better(id, tree[node], segment_right))
            insert_line(node * 2 + 1, middle + 1, segment_right, id);
    }

    void insert_segment(int node, int segment_left, int segment_right,
                        int query_left, int query_right, int id) {
        if (query_left <= segment_left && segment_right <= query_right) {
            insert_line(node, segment_left, segment_right, id);
            return;
        }
        int middle = segment_left + (segment_right - segment_left) / 2;
        if (query_left <= middle)
            insert_segment(node * 2, segment_left, middle, query_left, query_right, id);
        if (middle < query_right)
            insert_segment(node * 2 + 1, middle + 1, segment_right,
                           query_left, query_right, id);
    }
};

// Usage:
// LiChaoTree tree(left, right);
// int line_id = tree.add_line(slope, intercept);
// int segment_id = tree.add_segment(x0, y0, x1, y1);
// auto best_value = tree.query(x);

#include <bits/stdc++.h>
using namespace std;

// 返回 1-based 离散化编号；原数组不会被修改。
template <class T>
vector<int> discretize(const vector<T> &a) {
    vector<T> key = a;
    sort(key.begin(), key.end());
    key.erase(unique(key.begin(), key.end()), key.end());

    vector<int> id;
    id.reserve(a.size());
    for (const T &x : a)
        id.push_back(int(lower_bound(key.begin(), key.end(), x) - key.begin()) + 1);
    return id;
}

// Usage:
// vector<int> id = discretize(a);
// 如需恢复值，保留排序去重后的 key，并使用 key[id[i] - 1]。

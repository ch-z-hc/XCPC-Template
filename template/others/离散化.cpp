#include <bits/stdc++.h>

#define V vector

using namespace std;

// 返回 1-based 离散化编号；原数组不会被修改。
// 输入和结果位置均为 0-based；复杂度 O(n log n)，空间 O(n)。
// key 返回排序去重后的原值，编号 k 对应 key[k - 1]；key 不得与 a 同对象。
template < class T >
V < int > discretize(const V < T > &a, V < T > &key) {
    key = a;
    sort(key.begin(), key.end());
    key.erase(unique(key.begin(), key.end()), key.end());

    V < int > id;
    id.reserve(a.size());
    for (const T &x : a)
        id.push_back(int(lower_bound(key.begin(), key.end(), x) - key.begin()) + 1);
    return id;
}

// 调用示例：
// V < int > a{30, 10, 30}, key;
// V < int > id = discretize(a, key); // id = {2, 1, 2}，key = {10, 30}。
// 原值为 key[id[i] - 1]，不同值个数为 int(key.size())。

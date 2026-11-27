// 将 a 数组离散化，key 是去重后的原值表，编号 k 的原值为 key[k - 1]，排名是 1-based。

#include <bits/stdc++.h>

#define V vector

using namespace std;

template < class T > V < int > discretize(const V < T > &a, V < T > &key) {
    key = a;
    sort(key.begin(), key.end());
    key.erase(unique(key.begin(), key.end()), key.end());

    V < int > id;
    id.reserve(a.size());
    for (const T &x : a)
        id.push_back(int(lower_bound(key.begin(), key.end(), x) - key.begin()) + 1);
    return id;
}

/*
V < int > a{30, 10, 30}, key;
auto id = discretize(a, key);
*/

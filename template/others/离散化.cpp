// 将 a 数组离散化，key 是去重后的原值表，编号 k 的原值为 key[k - 1]，排名是 1-based。

#include <bits/stdc++.h>

template <class T> std::vector<int> discretize(const std::vector<T> &a, std::vector<T> &key) {
    key = a;
    std::sort(key.begin(), key.end());
    key.erase(std::unique(key.begin(), key.end()), key.end());

    std::vector<int> id;
    id.reserve(a.size());
    for (const T &x : a)
        id.push_back(int(std::lower_bound(key.begin(), key.end(), x) - key.begin()) + 1);
    return id;
}

/*
std::vector<int> a{30, 10, 30}, key;
auto id = discretize(a, key);
*/

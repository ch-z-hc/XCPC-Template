#include <bits/stdc++.h>

#define V vector

using namespace std;

/*
返回每个 a[i] 的排名，排名从 1 数，相同值同排名。key 是从小到大去重后的原值表。
编号 k 的原值是 key[k - 1]。key 和 a 须传不同数组。时间 O(n log n)，空间 O(n)。
*/
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

/*
V < int > a{30, 10, 30}, key;
auto id = discretize(a, key); // id = {2, 1, 2}，key = {10, 30}
*/

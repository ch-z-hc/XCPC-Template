/*
使用 Boost.Multiprecision 的有符号任意精度整数和既约分数；分母非零。
整数加减 O(d)，普通乘除至多 O(d²)，d 为操作数的机器字数；分数运算还需约分。
*/

#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;

using BigInt = boost::multiprecision::cpp_int;
using Fraction = boost::multiprecision::cpp_rational;

/*
BigInt a("123456789012345678901234567890"), b = 17;
BigInt q = a / b, r = a % b, c = a * b + r;
Fraction x(a, b), y(3, 5);
Fraction z = (x + y) * (x - y) / y;
cout << (x < y) << ' ' << numerator(z) << '/' << denominator(z);
*/

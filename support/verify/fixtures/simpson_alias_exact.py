"""仅用有理数验证积分反例，不调用被测模板，也不使用浮点积分。"""
from fractions import Fraction as F

l, r = F(-1, 10), F(3, 10)


def f(x):
    return x**4 / 5 - x**6


def simpson(a, b):
    return (b - a) * (f(a) + 4 * f((a + b) / 2) + f(b)) / 6


coarse = simpson(l, r)
mid = (l + r) / 2
fine = simpson(l, mid) + simpson(mid, r)
exact = (r**5 - l**5) / 25 - (r**7 - l**7) / 7
assert coarse == fine == F(493, 7500000)
assert exact == F(1161, 17500000)
assert abs(fine - exact) > F(1, 10000000000)
left, right = simpson(l, mid), simpson(mid, r)
quarter_l, quarter_r = (l + mid) / 2, (mid + r) / 2
left_fine = simpson(l, quarter_l) + simpson(quarter_l, mid)
right_fine = simpson(mid, quarter_r) + simpson(quarter_r, r)
local_delta = abs(left_fine - left) + abs(right_fine - right)
assert local_delta > 15 * F(1, 10000000000)
print(f"coarse = fine = {coarse}; exact = {exact}; error = {fine - exact}")
print(f"next-level sum of absolute local differences = {local_delta}")
print("PASS: exact rational proof of false convergence and new guard rejection")

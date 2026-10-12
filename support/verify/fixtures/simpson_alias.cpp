#include "../../../template/math/自适应辛普森积分.cpp"

// 连续六次多项式；粗、细两级 Simpson 恰好相同，误差估计为 0 但真实误差非零。
// 修复前退出 1，修复后应退出 0；原判据的精确推导见 simpson_alias_exact.py。
int main() {
    auto f = [](double x) { return x * x * x * x * (0.2 - x * x); };
    double actual = simpson(f, -0.1, 0.3, 1e-10);
    long double expected = 1161.L / 17500000;
    cout << setprecision(20) << "actual=" << actual << " expected=" << expected
         << " error=" << abs(actual - expected) << '\n';
    return abs(actual - expected) <= 1e-10L ? 0 : 1;
}

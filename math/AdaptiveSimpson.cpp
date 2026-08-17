#include <bits/stdc++.h>

#define debug(a) cout << #a << " = " << (a) << ' '
#define db double

const db eps = 1e-7;

using namespace std;

db a, b, L, R;

db f(db x) {
    return 2 * sqrt((1 - (x * x / a / a))) * b;
}

db simpson(db l, db r) {
    db mid = (l + r) / 2;
    return (r - l) * (f(l) + 4 * f(mid) + f(r)) / 6;
}

db asr(db l, db r, db eps, db ans, int step) {
    db mid = (l + r) / 2;
    db fl = simpson(l, mid), fr = simpson(mid, r);
    if (abs(fl + fr - ans) <= 15 * eps && step < 0)
        return fl + fr + (fl + fr - ans) / 15;
    return asr(l, mid, eps / 2, fl, step - 1) +
           asr(mid, r, eps / 2, fr, step - 1);
}

db calc(db l, db r, db eps) {
    return asr(l, r, eps, simpson(l, r), 12);
}

void solve() {
    cin >> a >> b >> L >> R;
    printf("%.3lf\n", calc(L, R, eps));
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t --)
        solve();
    return 0;
}
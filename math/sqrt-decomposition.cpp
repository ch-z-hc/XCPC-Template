void H(int n) {
    int l = 1, r;
    while (l <= n) {
        r = n / (n / l);
        // n / [l ~ r] 的取值一样 
        l = r + 1;
    }
}
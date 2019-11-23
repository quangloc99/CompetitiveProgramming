/**
 * Author: Tran Quang Loc (darkkcyan)
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/SPOJ/PON.cpp
 */
#ifndef __SIZEOF_INT128__
#error "no int 128"
#endif

typedef __int128 i128;

long long mpow(long long base, long long exp, long long mod) {
    long long ans = 1;
    for (; exp > 0; exp >>= 1) {
        if (exp &  1) ans = (long long)((i128)ans * base % mod);
        base = (long long)((i128)base * base % mod);
    }
    return ans;
}

bool is_prob_prime(long long num) {
    if (num <= 4) return num == 2 or num == 3;
    int tz = 0; 
    long long d = num - 1;
    for (; (d & 1) == 1; d >>= 1) ++tz;

    for (auto p: {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (num == p) return true;
        long long x = mpow(p, d, num);
        if (x == 1 or x == num - 1) continue;
        bool ok = 0;
        for (int i = 0; i < tz; ++i) {
            x = (long long)((i128)x * x % num);
            if (x != num - 1) continue;
            ok = 1;
            break;
        }
        if (!ok) return false;
    }
    return true;
}

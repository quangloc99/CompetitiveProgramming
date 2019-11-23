/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.spoj.com/problems/PON/
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

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

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest--) {
        llong num; cin >> num;
        cout << (is_prob_prime(num) ? "YES\n" : "NO\n");
    }

    return 0;
}

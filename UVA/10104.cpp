#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

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

tuple<long long, long long, long long> extended_gcd(long long a, long long b) {
    if (b == 0) return {a, 1, 0};
    long long gcd, x1, y1;                          // 2 lines can be written as one in C++17
    tie(gcd, x1, y1) = extended_gcd(b, a % b);      // auto [gcd, x1, y1] = extended_gcd(b, a % b);
    return {gcd, y1, x1 - (a / b) * y1};
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    long long a, b;
    while (cin >> a >> b) {
        long long gcd, x, y;
        tie(gcd, x, y) = extended_gcd(a, b);
        cout << x << ' ' << y << ' ' << gcd << '\n';
    }

    return 0;
}

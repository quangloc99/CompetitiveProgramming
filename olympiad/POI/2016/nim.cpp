/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://szkopul.edu.pl/problemset/problem/X6IwPa2H9FSd3Ly6bYp5t8Vu/site/?key=statement
 * Idea: we can just do dp like this:
 *   dp[m][r][mask] = the number of way to pick the subset of x heaps in the first m heaps,
 *   such that x % d == r, and xor of those picked heap is mask.
 * The result is dp[n][0][xor of all heaps];
 * So how to optimize?
 * - We can sort the number in increasing order, and process them like above, but limit the highest bit
 *   at each moment to be the number of bit of the heap's size at that moment. This can optimize the
 *   solution, because, suppose mask can has maximum x's bit, then we won't process more than
 *   O((sum of all heaps) / 2^x) times, and each dp we do in O(2^x * d), so total time complexity is
 *   O((sum of all heaps) / 2^x * 2^x * d) = O((sum of all heaps) * d).
 * - For optimizing memory, I was thought of keeping 2 last dp's state in order to remove the first dimension.
 *   But even doing that, the memory is about 80MB. For further optimization, we notice that 
 *   dp[i][r] only depends on dp[i - 1][(r - 1) % d] and dp[i - 1][r], so we can do compute in just one same
 *   state. See the code for more details.
 */
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

const llong mod = (llong)1e9+7;
void add_mod(int& u, llong v) {
    v += u;
    if (v > mod) v -= mod;
    u = (int)v ;
}

int dp[12][(1 << 20) + 100] = {0};
int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, d; cin >> n >> d;
    vector<int> a(n);
    rep(i, n) cin >> a[i];

    llong xor_a = 0;
    rep(i, n) xor_a ^= a[i];
    sort(a.begin(), a.end());

    dp[0][0] = 1;
    int cur_size = 1;
    for (auto num: a) {
        int new_size = cur_size;
        while (new_size <= num) new_size <<= 1;
        memcpy(dp[d], dp[d - 1], sizeof(int) * new_size);

        for (int cur_d = d; cur_d -- > 0; ) {
            auto cur_dp = dp[cur_d];
            auto old_dp = cur_d == 0 ? dp[d] : dp[cur_d - 1];
            rep(i, cur_size) {
                add_mod(cur_dp[i ^ num], old_dp[i]);
            }
        }
        cur_size = new_size;
    }

    cout << (dp[0][xor_a] - (n % d == 0) + mod) % mod;

    return 0;
}

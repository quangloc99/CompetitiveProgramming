/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.infoarena.ro/problema/desc
 * Idea: do dp:
 * - dp[i][f] = the number of decomposition of i with all number >= f.
 *   calculating this dp is fine, there is no more than O(i^(1/3))
 *   divisors for each i.
 * - There is one problem for this way to do dp, that the number will be too big.
 *   So instead I filter all the divisors, make a table for each (i/f) so
 *   I can archive this dp in O(M^2) times, where M = number of divisors of n.
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


vector<llong> get_divisors(llong num) {
    vector<llong> u, v;
    for (llong i = 1; i * i <= num; ++i) {
        if (num % i) continue;
        u.push_back(i);
        if (i * i != num)
            v.push_back(num / i);
    }
    u.insert(u.end(), v.rbegin(), v.rend());
    return u;
}

int main(void) {
    freopen("desc.in", "r", stdin);
    freopen("desc.out", "w", stdout);
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    llong n, k;
    cin >> n >> k;
    auto divisors = get_divisors(n);
    int cnt_div = len(divisors);

    assert(cnt_div <= 10000);

    vector<vector<short>> div_res(cnt_div, vector<short>(cnt_div + 10, -1));
    vector<vector<int>> dp(cnt_div, vector<int>(cnt_div + 10, 0));

    rep(i, cnt_div) {
        int v = i;
        rep(u, i + 1) {
            if (divisors[i] % divisors[u]) continue;
            while (v >= 0 and divisors[i] != divisors[u] * divisors[v]) --v;
            if (u > v) break;
            div_res[i][u] = (short)v;
            div_res[i][v] = (short)u;
        }
    }

    rep1(base, cnt_div - 1) {
        dp[base][base] = 1;
        for (int start = base; start--; ) {
            dp[base][start] = dp[base][start + 1];
            if (div_res[base][start] == -1) continue;
            dp[base][start] += dp[div_res[base][start]][start];
        }
    }

    cout << dp[cnt_div - 1][1] << '\n';

    int prev = 1;
    for (int num = cnt_div - 1; num > 0; ) {
        for (int i = prev; i < cnt_div; ++i) {
            if (div_res[num][i] == -1) continue;
            llong cnt = dp[num][i] - dp[num][i + 1];
            if (k > cnt) k -= cnt;
            else {
                cout << divisors[i] << ' ';
                num = div_res[num][i];
                prev = i;
                break;
            }
        }

    }

    return 0;
}

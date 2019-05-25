/**
 * Author: Tran Quang Loc (darkkcyan)
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
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

#define maxn 1010101
long double dp[maxn][2];  // n_pair/has_unmatch

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    cout.precision(12);
    while (ntest--) {
        int n;
        cin >> n;
        string a, b; cin >> a >> b;
        int n_pair = 0;
        bool unmatch_side = 0;
        rep(i, n) {
            if (a[i] == '1' and b[i] == '1') ++n_pair;
            if (a[i] == '1' and b[i] == '0') unmatch_side = 0;
            if (a[i] == '0' and b[i] == '1') unmatch_side = 1;
        }
        
        dp[0][1] = 0;
        dp[0][0] = 1;

        rep1(i, n_pair) {
            dp[i][1] = 1 - dp[i - 1][0];

            dp[i][0] = 0;
            rep(f, 100)
                dp[i][0] = 1 - (
                        dp[i - 1][1] * i / (i + 1) +
                        dp[i][0] / (i + 1)
                );
        }
        cout << fixed << dp[n_pair][!unmatch_side] << '\n';
    }
    return 0;
}

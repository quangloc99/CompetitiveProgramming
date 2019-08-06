/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://csacademy.com/contest/balkan-oi-2017-day-2/
 * The ediroail is also in the link.
 * This solution is based on the editorial.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define ii pair<int, int>
#define xx first
#define yy second
#define len(v) ((int)v.size())
#define rep(i,n) for (int i = -1; ++i < n; )
#define rep1(i,n) for (int i = 0; i++ < n; )

template<class T>
T minimize(T& x, const T y) {
    return x = min(x, y);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int ntest; cin >> ntest;
    rep1(testcase, ntest) {
        int n; cin >> n;
        vector<int> a(n);
        unordered_set<int> vals;
        for (auto& i: a) {
            cin >> i;
            vals.insert(i);
        }
        if (len(vals) == 1) {
            cout << "0\n";
            continue;
        }
        if (len(vals) == 2) {
            cout << (vals.count(2) ? "0\n" : "-1\n");
            continue;
        }


        int ans = n;
        for (auto try_remove_val: {0, 1}) {
            vector<int> b = a;
            b.resize(remove(b.begin(), b.end(), try_remove_val) - b.begin());
            if (b[0] == 2 or b.back() == 2) {
              minimize(ans, n - len(b));
              continue;
            }
            bool has_two2 = false;
            rep1(i, len(b) - 1) if (b[i] == 2 and b[i - 1] == 2) {
              has_two2 = true;
              break;
            }
            minimize(ans, n - len(b) + (has_two2 ? 0 : 1));
        }

        int cnt2 = (int)count(a.begin(), a.end(), 2);

        static int dp[2][5010][3][4];
        
        rep(cur2, cnt2 + 1) rep(last, 3) rep(mask, 4) {
            dp[1][cur2][last][mask] = n * n;  // too lazy to declare infinity
        }

        dp[1][cnt2][2][0] = 0;

        rep(i, n) {
            bool cur = i & 1;
            bool prev = !cur;
            rep(cur2, cnt2 + 1) rep(last, 3) rep(mask, 4) {
                dp[cur][cur2][last][mask] = n * n;
            }

            rep(prev2, cnt2 + 1) rep(last, 3) rep(mask, 4) {
                minimize(dp[cur][prev2][last][mask], dp[prev][prev2][last][mask] + 1);
                if (a[i] == 2) {
                    if (prev2) {
                        minimize(dp[cur][prev2 - 1][2][mask], dp[prev][prev2][last][mask]);
                    }
                } else {
                    bool diff_group = last != 2 and last != a[i];
                    if (!diff_group or prev2) 
                        minimize(dp[cur][prev2 - diff_group][a[i]][mask | (1 << a[i])], dp[prev][prev2][last][mask]);
                }
            }
        }

        rep(cur2, cnt2 + 1) rep(last, 3) 
            minimize(ans, dp[~n & 1][cur2][last][3]);
        cout << ans << '\n';
    }
  
    return 0;
}

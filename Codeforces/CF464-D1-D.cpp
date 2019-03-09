/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/13694
 * This code was implemented with the same idea in the editorial.
 * Main ideas:
 * - It is "not really hard" to see that the answer is EV1 * k, where EV1 is the expected value for only 1 item.
 * - To find that EV1, we can use dp. The dp formula is a little long, so please see the editorial.
 * - The total idea is O(n^2). To optimize, we can elimitate the state with extremely low EV. The editorial stated that
 *   if the item level exceeded 700 (about O(sqrt(n)), it will be very small. So we end up with a solution of O(n*sqrt(n))
 */
#include <bits/stdc++.h>

using namespace std;

#define llong long long
#define len(x) ((int)x.size())
#define rep(i, n) for (int i = 0; i < n; ++i)
#define rep1(i, n) for (int i = 1; i <= n; ++i)
#define ldouble long double

#define MAX_INTERESTING_LEVEL 750
int n, k;

int main(void) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> n >> k;
    ldouble ans = 0;
    vector<ldouble> dp(MAX_INTERESTING_LEVEL);
    rep1(f, n) {
        vector<ldouble> nextDp(MAX_INTERESTING_LEVEL);
        rep1(i, len(dp) - 1) {
            ldouble evNotPick = dp[i] * (k - 1) / k;
            ldouble evPick = 0;
            evPick += ((i + 1 == len(dp) ? .0 : dp[i + 1]) + i) / (i + 1);
            evPick += (dp[i] * i + 1LL * (i + 1) * i / 2) / (i + 1);
            evPick /= k;

            nextDp[i] = evPick + evNotPick;
        }
        dp = move(nextDp);
    }
    ans = dp[1] * k;
    cout << fixed << setprecision(12) << ans;

    return 0;
}

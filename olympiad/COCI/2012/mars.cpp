/**
 * Author: Tran Quang  Loc (darkkcyan)
 * Problem https://dunjudge.me/analysis/problems/1362/
 * Official solution can be found here: http://hsin.hr/coci/archive/2012_2013/, this problem is from the 1st round.
 * This solution is the same as the first solution.
 * I think they did not clearly explain their observation, so here is my try :).
 * The observation is simple: from a position, you always know which elements is going to be next candidate
 * for next position.
 * For example:
 * - Suppose you are at the first position, clearly you have to pick the sibling for the next position.
 * - Now you are at the second position, the sibling is already picked (and it is the first position).
 *   so for the next position, we have to pick the cousins shared the same grandparent with the same node.
 * - The third position is the same as first, we have to pick the sibling for the forth position.
 * - Now at the forth position, we know that the sibling and all cousins shard the same grand parent are picked.
 *   so for the next position, we need to pick the 1 cousin that shares the same grand grandparent with this elements.
 * - ...
 * - At the 8th position, we need to pick a cousin that shares grand grand grand parent with the current element.
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

#define maxk 10
#define maxn (1 << maxk)
#define inf ((llong)INT_MAX)
int k, n;
llong dis[maxn][maxn];
llong dp[maxn][maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> k;
    n = 1 << k;
    rep(i, n) rep(f, n) cin >> dis[i][f];

    rep(i, n) rep(f, n) dp[i][f] = inf;
    rep(i, n) dp[0][i] = 0;

    rep1(i, n - 1) {
        int block_size = (i & -i) << 1;
        for (int block_start = 0; block_start < n; block_start += block_size) {
            int mid = block_start + block_size / 2;
            for (int u = block_start; u < mid; ++u)
            for (int v = mid; v < block_start + block_size; ++v) {
                dp[i][u] = min(dp[i][u], dp[i - 1][v] + dis[v][u]);
                dp[i][v] = min(dp[i][v], dp[i - 1][u] + dis[u][v]);
            }
        }
    }
    llong ans = *min_element(dp[n - 1], dp[n - 1] + n);
    cout << ans;


    return 0;
}

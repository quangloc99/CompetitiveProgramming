/**
 * Author: Tran Quang Loc (darkkcyan)
 * Official solution here:
 *     https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/CEOI/official/2016/kangaroo-solution.pdf
 * but I think the whole solution is not correct. See my reason here:
 *     https://github.com/quangloc99/CompetitiveProgramming/blob/master/olympiad/CEOI/2016/error-in-offical-solution-of-kagaroo.md
 *
 * See the namespace solution_51_points and solution_100_points for the code + explaination to
 * get 51 and 100 points respectively.
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

#define rem ((llong)1e9+7)
#define UP 1
#define DOWN 0

namespace solution_51_points {
    // the solution is similar to the official solution, but I don't swap cs and cf.
    // I check them by hand and decrease cf if necessary.
    const int maxn = 222;
    llong dp[maxn][maxn][maxn][2];  // dp[n][cs][cf][dir]
                                    //   n - the number of cells in the garden.
                                    //   cs - the starting cell
                                    //   cf - the final cell
                                    //   dir - the current direction to move (UP or DOWN)
    llong pref_sum[maxn][maxn][maxn][2];
    // pref_sum[n][cs][cf][dir] = dp[n][1][cf][dir] + dp[n][2][cf][dir] + ... + dp[n][cs][cf][dir]
    // in other words: pref_sum[n][cs][cf][dir] = sum of dp[n][1..cs][cf][dir]

    // these 2 functions lazily calculate pref_sum and dp respectively.
    llong cal_pref_sum(int, int, int, bool);
    llong cal(int, int, int, bool);

    llong cal_pref_sum(int n, int cs, int cf, bool dir) {
        if (cs == 0) return 0;
        llong& ans = pref_sum[n][cs][cf][dir];
        if (ans == -1) ans = cal_pref_sum(n, cs - 1, cf, dir) + cal(n, cs, cf, dir);
        return ans %= rem;
    }

    llong cal(int n, int cs, int cf, bool dir) {
        llong& ans = dp[n][cs][cf][dir];
        if (ans != -1) return ans;
        if (cf == cs) return 0;
        if (n == 2) return ((dir == DOWN and cs > cf) or (dir == UP and cs < cf));
        int range_begin, range_end;
        if (dir == DOWN) {
            range_begin = 1;
            range_end = cs - 1;
        } else {
            range_begin = cs;
            range_end = n - 1;
        }

        int nxt_cf = cf - (cf > cs);
        ans = cal_pref_sum(n - 1, range_end, nxt_cf, !dir);
        ans += rem - cal_pref_sum(n - 1, range_begin - 1, nxt_cf, !dir);
        ans %= rem;
        // clog << n << ' ' << cs << ' ' << cf << ' ' << dir << ' ' << ans << endl;
        return ans;
    }

    llong solve(int n, int cs, int cf) {
        memset(dp, -1, sizeof(dp));
        memset(pref_sum, -1, sizeof pref_sum);
        return (cal(n, cf, cs, UP) + cal(n, cf, cs, DOWN)) % rem;
    }
};

namespace solution_100_points {
    // Solution used here is "Connected component" dp trick. So this solution and the above are not the same.
    // The "original and currently only" post about this trick: https://codeforces.com/blog/entry/47764
    // thought it is not really informative and the discussed problem was way harder than this one.
    // So I think I will do really detailed comment, for myself too.
    // 
    // Let's simplize the problem statement: given n, cs, cf (1 <= cs, cf <= n). Find the number of 
    // permutation of n number from 1 to n that have the first and last elements equals to cs and cf respectively
    // and the permutation must be alternated (a_1 < a_2 > a_3 < a_4 > ..., or vise versa).
    //
    // In this trick, we do as following: fill in number by number, but not in some positions, but in our "components".
    // For example, suppose we are adding the following numbers: 1, 2, 3 one by one.
    // - Firstly, we add "1". Since we are currently have no added elements, so there is currently no components,
    // so by adding 1, we now have one component: {1}.
    // - Now, we add "2". We now can 2 things:
    //   1) Add "2" into our only component. There are 2 ways to do so: {1, 2} or {2, 1}.
    //     Note that we have only 1 component, but now with 2 elements.
    //   2) Create a new component, so the components now are {1} and {2}. But because the order matters.
    //      So we actually have 2 way to do it: {1}, {2}, or {2}, {1}
    // - Add "3" into our components. In the first case above, where we only have 1 component, we can only do 
    //   just 2 things above. But in the second case, we can do 1 more operation:
    //   merging 2 components by add 3 in the middle. So beside add 3 into the existed components or creating new one,
    //   we can merge the second cases like the following: {1, 3, 2} or {2, 3, 1}, and resulting only one component.
    //
    // Before the continuing, I have to re-note that: order matters!
    //
    // In conclusion, suppose we are having n components, and we want to add 1 elements, we can do 3 operations:
    // - Creating a new component. The number of way to create (or insert) a new component is n + 1,
    //   since there are n - 1 places we can insert it between 2 existed components, and we can also insert
    //   it at the beginning or the end. The total number of components is now n + 1
    // - Adding it to an existed component. There are totally 2 * n ways, because for each components, we can insert it
    //   at the beginning or the end. The total number of components unchanged.
    // - Merging 2 existed component. There are n - 1 ways to do so. And the number of components is now n - 1.
    // But base on the problem statement and yours objective, the number of ways can be changed! You are going to see
    // it right now.
    //
    // That is the ideas, so now is the dp parts, and the solution for this problem.
    // Let's dp[n][i] = the number of way to add elements 1, 2, .. n in to our components such that at the ends, 
    // we have i components and each components must *satisfies the alternated condition*.
    // See the formula in the code with the comments.
    const int maxn = 2020;
    llong dp[maxn][maxn];

    llong solve(int n, int cs, int cf) {
        memset(dp, 0, sizeof dp);
        dp[0][0] = 1;  // 0 elements, 0 components, there is 1 way to do it.

        rep1(i, n) {
            if (i == cs or i == cf) {  // we must add cs at the beginning and cf at the end.
                rep1(f, i) {
                    // there are 2 cases here:
                    // - Add cs to the first component, and it must be the first element (the same for cf, but to the last).
                    // - Creating new component. There is only one way to do so, since cs must be the very first element (same to cf).
                    // Each of these 2 cases has only 1 way to do.
                    (dp[i][f] += dp[i - 1][f] + dp[i - 1][f - 1]) %= rem;
                }
            } else {
                int unfilled_endpoints = int(i < cs) + int(i < cf);
                rep1(f, i) {
                    // we merge f + 1 components into f components. There are f ways to do so.
                    (dp[i][f] += dp[i - 1][f + 1] * f) %= rem;  

                    // Create a new component. The special case is f = 1, then there is only 1 way.
                    // For f >= 2, we must consider if the first and the last elements are filled or not.
                    int places_for_new_comp = 1;
                    if (f > 1) places_for_new_comp = f - 2 + unfilled_endpoints;
                    (dp[i][f] += dp[i - 1][f - 1] * places_for_new_comp) %= rem;

                    // We don't do adding element into component here. 
                    // What we are doing here is adding elements from smallest to biggest.
                    // If we don't do adding element into component, we always know for sure, when merging 2 components,
                    // we can always archive the alternating pattern.
                    // (dp[i][f] += dp[i - 1][f] * (2 * f + unfilled_endpoints - 2)) %= rem;  // well, this line is necessary.

                }
            }
        }

        return dp[n][1];  // after adding all n elements, our result must be connected, so there is only 1 component.
    }
};

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    // using namespace solution_51_points;
    using namespace solution_100_points;

    int n, cs, cf;
    cin >> n >> cs >> cf;
    cout << solve(n, cs, cf);

    return 0;
}

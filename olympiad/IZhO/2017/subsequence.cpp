/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/IZhO17_subsequence
 * Category: dp, meet in the middle.
 *
 * The approach is similar to finding the longest increasing subsequence.
 * We define dp[i] = longest beautiful subsequence that end at i-th element.
 * dp[i] = max(dp[j] + 1) for j from 1 to i and bitCount(a[j] & a[i]) == k[i];
 * To find j for each i, we can use meet in the middle. We know that a[i] will have no more than 20 bits.
 * So we can break each a[i] into 2 halfs, each halfs has 10 bits.
 * Let's define f[i][u][v][k] = the length of the longest beautiful subsequece can be archive from the first i numbers,
 * the last number in the subsequence have the first half equals to u and bitCount(v & second_half) = k.
 * We can use the array f[i] to calculate dp[i + 1] in O(2^10) as well as update the f[i + 1] also in O(2^10).
 * Therefore we can end up with O(2^10 * n) solution.
 *
 * Note that to reduce the array size, we can discard the first dimension, because at each moment we use each f[i] only once to 
 * calculate dp[i + 1] and update f[i + 1]. And also because we also need to find the sequence, I use array trace
 * instead of f. trace[u][v][k] = the id of the last element of the (longest) beautiful sequence having length equals to f[u][v][k]
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

#define maxn 101010
#define maxk 25
#define halfk 10
int n;
int a[maxn], k[maxn];
pair<int, int> dp[maxn];

int trace[1 << halfk][1 << halfk][halfk + 1];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    // clog << sizeof(dp) + sizeof(n) + sizeof(a) + sizeof(k) + sizeof(trace) << endl;
    cin >> n;
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> k[i];

    rep(i, (1 << halfk))
    rep(f, (1 << halfk))
    rep(diff, halfk + 1) 
        trace[i][f][diff] = -1;

    pair<int, int> ans(0, -1);
    rep(i, n) {
        int first_half = a[i] & ((1 << 10) - 1);
        int second_half = a[i] >> 10;
        dp[i] = {1, -1};
        rep(prev_first_half, (1 << 10)) {
            int first_half_diff = __builtin_popcount(prev_first_half & first_half);
            int second_half_diff = k[i] - first_half_diff;
            if (second_half_diff < 0 or second_half_diff > 10) continue;
            int cur_trace = trace[prev_first_half][second_half][second_half_diff];
            if (cur_trace == -1) continue;
            dp[i] = max(dp[i], {dp[cur_trace].xx + 1, cur_trace});
        }

        ans = max(ans, {dp[i].xx, i});

        rep(next_second_half, (1 << 10)) {
            int second_half_diff = __builtin_popcount(next_second_half & second_half);
            int& cur_trace = trace[first_half][next_second_half][second_half_diff];
            if (cur_trace != -1 and dp[cur_trace].xx > dp[i].xx) continue;
            cur_trace = i;
        }
    }

    cout << ans.xx << '\n';
    vector<int> rev_i(1, ans.yy);
    while (dp[rev_i.back()].yy != -1) {
        // assert(__builtin_popcount(a[rev_i.back()] & a[dp[rev_i.back()].yy]) == k[rev_i.back()]);
        rev_i.push_back(dp[rev_i.back()].yy);
    }
    for (int i = ans.xx; i--; ) cout << rev_i[i] + 1 << ' ';

    return 0;
}

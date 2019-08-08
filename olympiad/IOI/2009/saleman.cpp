/**
 * Author: Tran Quang Loc
 * Problem: https://oj.uz/problem/view/IOI09_salesman
 * Test data + official solution: http://www.ioi2009.org/?ln=2&id=363
 * My idea seems to be different from the official solution.
 * Idea:
 * - Let's assume all the fairs held on different dates.
 * - The dp idea is easy to see: processing the fairs in the chronological order
 *   and find the subsequence of fairs that maximize our profit. The process is
 *   really similar to finding LIS of an integer sequence: to find the answer for the
 *   i-th fair, we find out which fair from the first (i - 1) fairs we must start from.
 *   Also like finding the LIS, we can use some sort of data structure to help us.
 * - When finding the maximum profit fairs, we notice that the cost is also effected
 *   by the traveling cost. But we can easily see that the cost function is actually a
 *   linear function. But wait, there is more: there are only 2 slopes. We we can just
 *   store the y-intercept of those lines and do the max query over them.
 *
 * That is the basic idea. For the fairs that held in the same days, we can see that
 * it is only profitable if we travel in only 1 direction.
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

template<bool is_rev>  // for reversed query, i.e query range [i, +infinity) rather than (0, i]
struct MaxBit {
    vector<llong> data;
    MaxBit(int n, llong init_val = 0) : data(n + 10, init_val) {}

    void upd(int i, llong val) {
        if (is_rev) i = len(data) - i - 1;
        for (++i; i < len(data); i += i & -i) data[i] = max(data[i], val);
    }

    llong get(int i, llong ans = 0) {
        if (is_rev) i = len(data) - i - 1;
        for (++i; i > 0; i -= i & -i) ans = max(ans, data[i]);
        return ans;
    }
};

#define maxn 501010
#define inf ((llong)1e13)
int n;
llong u, d;
int s;
vector<pair<int, int>> fairs[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> u >> d >> s;
    rep(i, n) {
        int t, l, m; cin >> t >> l >> m;
        fairs[t].emplace_back(l, m);
    }

    MaxBit<false> down_bit(maxn, -inf); 
    MaxBit<true> up_bit(maxn, -inf);

    auto find_max_profit = [&](int pos) {
        return max(down_bit.get(pos, -inf) - pos * d, up_bit.get(pos, -inf) + pos * u);
    };

    auto update = [&](int pos, llong val) {
        down_bit.upd(pos, val + pos * d);
        up_bit.upd(pos, val - pos * u);
    };
    update(s, 0);
    
    rep(fair_time, maxn) {
        auto& cur = fairs[fair_time];
        if (!len(cur)) continue;
        sort(cur.begin(), cur.end());
        vector<llong> cur_ans(len(cur)), max_down(len(cur)), max_up(len(cur));
        rep(i, len(cur))
            cur_ans[i] = find_max_profit(cur[i].xx) + cur[i].yy;

        llong temp_down = -inf;
        rep(i, len(cur)) { 
            max_down[i] = max(cur_ans[i], temp_down - cur[i].xx * d + cur[i].yy);
            temp_down = max(temp_down, max_down[i] + d * cur[i].xx);
        }

        llong temp_up = -inf;
        for (int i = len(cur); i--; ) {
            max_up[i] = max(cur_ans[i], temp_up + cur[i].xx * u + cur[i].yy);
            temp_up = max(temp_up, max_up[i] - cur[i].xx * u);
        }

        rep(i, len(cur)) {
            cur_ans[i] = max(cur_ans[i], max(max_up[i], max_down[i]));
            // clog << group.xx << ' ' << cur[i].xx << ' ' << cur[i].yy << ' ' << cur_ans[i] << endl;
            update(cur[i].xx, cur_ans[i]);
        }
    }

    cout << find_max_profit(s);

    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/IOI16_aliens
 * Solution is describe in both official solution, as well as this site http://serbanology.com/show_article.php?art=The%20Trick%20From%20Aliens
 * The "odd" thing about this problem is that there is no where that explain the convexity of the dp function,
 * so it will be actually tough to apply this approach when competing.
 * 
 * Also the final result for this problem is lambda * k, not lambda * cnt[lambda]. This part cost me a lot of time debugging. I guest
 * this should be use/consider be for using when applying this approach.
 */
#include "aliens.h"

#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

struct line {
    llong a, b;  
    llong operator()(llong x) { return a * x + b; }

    line(llong a_, llong b_) : a(a_), b(b_) {}
};

bool cross(const line& back, const line& mid, const line& front) {
    return (back.b - mid.b) * (front.a - mid.a) - (back.a - mid.a) * (front.b - mid.b) > 0;
}

long long take_photos(int n, int m, int k, std::vector<int> r, std::vector<int> c) {
    vector<pair<int, int>> ranges;
    rep(i, n) {
        if (r[i] > c[i]) swap(r[i], c[i]);
        ranges.emplace_back(r[i], c[i] + 1);
    }
    // for (auto i: ranges) clog << i.xx << ' ' << i.yy << endl;
    // clog << endl;
    stable_sort(ranges.begin(), ranges.end(), [&](const pair<int, int>& u, const pair<int, int>& v) { 
            if (u.yy == v.yy) return u.xx >= v.xx;
            return u.yy < v.yy;
        });
    int new_length = 0;
    rep(i, n) {
        while (new_length and ranges[new_length - 1].xx >= ranges[i].xx) --new_length;
        ranges[new_length++] = ranges[i];
    }

    ranges.resize(new_length);
    n = new_length;

    // for (auto i: ranges) clog << i.xx << ' ' << i.yy << endl;
    // clog << endl;

    auto find_number_of_shot = [&](llong penalty_cost) {
        if (n == 0) return make_pair(0ll, 0);
        deque<pair<line, int>> qu;
        qu.emplace_back(line(-2 * ranges[0].xx, 1ll * ranges[0].xx * ranges[0].xx), 0);
        llong last_dp;
        int last_k;
        // clog << "-----------" << penalty_cost << endl;
        rep(i, n) {
            llong cur_x = ranges[i].yy;
            while (len(qu) > 1 and qu[0].xx(cur_x) > qu[1].xx(cur_x)) qu.pop_front();
            assert(len(qu));

            last_dp = qu[0].xx(cur_x) + cur_x * cur_x + penalty_cost;
            last_k = qu[0].yy + 1;
            
            if (i == n - 1) break;
            llong addition = max(ranges[i].yy - ranges[i + 1].xx, 0);
            addition *= addition;

            // clog << i << ' ' << last_dp << ' ' << last_k << ' ' << addition << endl;
            // clog << qu[0].xx.a << ' ' << qu[0].xx.b <<endl;

            line new_line(- 2ll * ranges[i + 1].xx, 1ll * ranges[i + 1].xx * ranges[i + 1].xx + last_dp - addition);
            // clog << new_line.a << ' ' << new_line.b << endl;

            while (len(qu) > 1 and cross(qu[len(qu) - 2].xx, qu.back().xx, new_line)) qu.pop_back();
            qu.emplace_back(new_line, last_k);
        }
        // clog << penalty_cost << ' ' << last_dp - k * penalty_cost << ' ' << last_k << endl;
        // clog << endl;
        return make_pair(last_dp - 1ll * k * penalty_cost, last_k);
    };

    llong lo = 0, hi = 1ll * m * m;
    while (lo < hi) {
        llong mid = (lo + hi) / 2;
        if (find_number_of_shot(mid).yy <= k) hi = mid;
        else lo = mid + 1;
    }

    auto ans = find_number_of_shot(hi).xx;
    if (hi) {
        auto u = find_number_of_shot(hi - 1);
        if (u.yy <= k) ans = min(ans, u.xx);
    }
    return ans;
}

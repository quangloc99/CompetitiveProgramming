/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/IOI09_regions
 * Idea is the same as https://github.com/radoslav11/CompetitiveProgramming/blob/master/Olympiad/IOI/IOI%2009-regions.cpp
 * This solution is faster though. The preprocess part for the heavy region can be done in O(N/B * N)
 * And the pair of light regions can be done in O(B) using 2 pointers technique.
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

const int maxn = 201010;
const int root = 1;
const int threshold = 500;
int n, r, q;
int h[maxn];
vector<int> gr[maxn];

int cnt_h[maxn] = {0};
vector<llong> pre_comp_ans1[maxn], pre_comp_ans2[maxn];

int dfs_cal_heavy(int u, int cur_heavy, int cnt_heavy_up) {
    pre_comp_ans1[cur_heavy][h[u]] += cnt_heavy_up;
    cnt_heavy_up += h[u] == cur_heavy;
    int cnt_heavy_down = 0;
    for (auto v: gr[u]) {
        cnt_heavy_down += dfs_cal_heavy(v, cur_heavy, cnt_heavy_up);
    }
    pre_comp_ans2[cur_heavy][h[u]] += cnt_heavy_down;
    cnt_heavy_down += h[u] == cur_heavy;
    return cnt_heavy_down;
}

int eu_tour_start[maxn], eu_tour_end[maxn], cur_tour = 0;
vector<pair<int, int>> euler_tours[maxn];

void dfs_cal_light(int u) {
    bool is_light = cnt_h[h[u]] <= threshold;
    if (is_light) { 
        euler_tours[h[u]].emplace_back(
            euler_tours[h[u]].size() ? euler_tours[h[u]].back().xx + 1 : 1,
            cur_tour
        );
        eu_tour_start[u] = cur_tour++;
    }

    for (auto v: gr[u]) {
        dfs_cal_light(v);
    }

    if (is_light) {
        euler_tours[h[u]].emplace_back(
            euler_tours[h[u]].back().xx - 1,
            cur_tour
        );
        eu_tour_end[u] = cur_tour++;
    }
}


int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> r >> q;
    cin >> h[1];
    for (int i = 2; i <= n; ++i) {
        int s; 
        cin >> s >> h[i];
        gr[s].push_back(i);
    }

    rep1(i, n) cnt_h[h[i]]++;
    rep1(i, r) if (cnt_h[i] > threshold) {
        pre_comp_ans1[i].resize(r + 1);
        pre_comp_ans2[i].resize(r + 1);
        dfs_cal_heavy(root, i, 0);
    }

    dfs_cal_light(root);
    while (q--) {
        int r1, r2; cin >> r1 >> r2;
        if (cnt_h[r1] > threshold) {
            cout << pre_comp_ans1[r1][r2] << endl;
            continue;
        }
        if (cnt_h[r2] > threshold) {
            cout << pre_comp_ans2[r2][r1] << endl;
            continue;
        }

        llong ans = 0;
        int u = 0, v = 0;
        for (; v < len(euler_tours[r2]); ++v) {
            if (v and euler_tours[r2][v].xx < euler_tours[r2][v - 1].xx) continue;
            while (u < len(euler_tours[r1]) and euler_tours[r1][u].yy < euler_tours[r2][v].yy)
                ++u;
            if (u) ans += euler_tours[r1][u - 1].xx;
        }
        cout << ans << endl;
    }

    return 0;
}

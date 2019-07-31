/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://wcipeg.com/problem/coi07p3
 * Idea:
 * - Use Dijkastra's algorithm.
 * - We have O(2 * k + 1) nodes: 1, n, B_i and A_i
 * - For B_i and A_i, their distance should be 1.
 * - After that, we can also use the original passage to go from nodes to nodes.
 *   So we sort all that nodes in the increasing order, and connect 2 neighbor element in the sorted array.
 *   Their cost should be their distance in the original passage.
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
#define sqr(x) ((x) * (x))
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

llong get_level(llong x) {
    return (llong)ceil((sqrt(x) - 1.0) / 2);
}

llong get_smallest_neighbor(llong x) {
    llong lv = get_level(x);
    for (llong u = sqr(lv * 2 + 1), v = sqr(lv * 2 - 1); ; u -= lv * 2, v -= lv * 2 - 2) {
        if (u - x >= lv * 2) continue;
        return v - (u - x - 1);
    }
}

#define maxn 201010
#define inf ((llong)1e16)
llong n;
int k;
llong b[maxn];
vector<pair<llong, int>> gr[maxn];

llong dis[maxn];
void sssp(int s, int n_nodes) {
    rep(i, n_nodes) dis[i] = inf;
    dis[s] = 0;
    priority_queue<
        pair<llong, int>,
        vector<pair<llong, int>>,
        greater<pair<llong, int>>
    > pq;

    for (pq.push({0, s}); len(pq); ) {
        llong d = pq.top().xx;
        int u = pq.top().yy;
        pq.pop();
        if (dis[u] < d) continue;
        for (auto i: gr[u]) {
            int v = i.yy;
            llong nd = i.xx + d;
            if (nd >= dis[v]) continue;
            dis[v] = nd;
            pq.push({nd, v});
        }
    }
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> k;
    rep(i, k) {
        cin >> b[2 * i];
        b[2 * i + 1] = get_smallest_neighbor(b[2 * i]);
    }

    b[2 * k] = 1;
    b[2 * k + 1] = n;
    vector<llong> vals(b, b + 2 * k + 2);
    sort(vals.begin(), vals.end());
    vals.resize(unique(vals.begin(), vals.end()) - vals.begin());

    rep(i, len(vals) - 1) {
        gr[i].emplace_back(vals[i + 1] - vals[i], i + 1);
        gr[i + 1].emplace_back(vals[i + 1] - vals[i], i);
    }

    rep(i, k) {
        int u = (int)(lower_bound(vals.begin(), vals.end(), b[2 * i]) - vals.begin());
        int v = (int)(lower_bound(vals.begin(), vals.end(), b[2 * i + 1]) - vals.begin());
        gr[u].emplace_back(1, v);
        gr[v].emplace_back(1, u);
    }

    sssp(0, len(vals));
    cout << dis[lower_bound(vals.begin(), vals.end(), n) - vals.begin()];

    return 0;
}

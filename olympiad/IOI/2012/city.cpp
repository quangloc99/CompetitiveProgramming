/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/IOI12_city
 * Official editorial: http://www.ioi2012.org/hints-for-tasks-of-day-2/index.html
 * This solution is pretty much the same as the editorial.
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


int DistanceSum(int n, int *x, int *y) {
    map<pair<int, int>, int> points;
    rep(i, n) points[{x[i], y[i]}] = i;

    struct Dsu {
        mutable vector<int> root;
        vector<int> cnt;
        Dsu(int n_): root(n_), cnt(n_, 1) {
            rep(i, len(root)) root[i] = i;
        }

        int find_set(int u) const { return u == root[u] ? u : root[u] = find_set(root[u]); }
        void join(int u, int v) {
            u = find_set(u);
            v = find_set(v);
            if (u == v) return;
            if (rand() & 1) swap(u, v);
            root[u] = v;
            cnt[v] += cnt[u];
        }

        int set_count(int u) { return cnt[find_set(u)]; }
    };

    const int dx[] = {0, 1};
    const int dy[] = {1, 0};

    llong ans = 0;
    rep(dir, 2) {
        Dsu dsu(n);
        rep(i, n) {
            if (points.count({x[i] + dx[dir], y[i] + dy[dir]})) {
                dsu.join(points[{x[i] + dx[dir], y[i] + dy[dir]}], i);
            }
        }
        vector<set<int>> gr(n);
        rep(i, n) {
            auto f = points.find({x[i] + dx[!dir], y[i] + dy[!dir]});
            if (f == points.end()) continue;
            int u = dsu.find_set(i);
            int v = dsu.find_set(f->yy);
            gr[u].insert(v);
            gr[v].insert(u);
        }

        vector<int> tree_size = dsu.cnt;
        function<void(int, int)> dfs = [&](int u, int p) {
            for (auto v: gr[u]) {
                if (v == p) continue;
                dfs(v, u);
                tree_size[u] += tree_size[v];
                ans += 1ll * tree_size[v] * (n - tree_size[v]);
            }
        };
        dfs(dsu.find_set(0), -1);
    }

    return (int)(ans % (1000 * 1000 * 1000));
}

// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: http://usaco.org/index.php?page=viewproblem2&cpid=901
// Solution: http://usaco.org/current/data/sol_exercise_platinum_jan19.html
//
// This code implemented the same idea as in the official solution.
//
// Tue Feb 25 20:09:02 MSK 2020: START coding
// Tue Feb 25 20:34:07 MSK 2020: DONE coding
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

#define CONCAT_(x, y) x##y/*{{{*/
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int __db_level = 0;
bool __db_same_line = false;
#define clog cerr << string(!__db_same_line ? __db_level * 2 : 0, ' ')
struct debug_block {
    function<void()> fn;
    void print_name() { __db_same_line = true; fn(); clog << endl; __db_same_line = false;  }
    debug_block(function<void()> fn_): fn(fn_) { clog << "{ "; print_name(); ++__db_level; }
    ~debug_block() { --__db_level; clog << "} "; print_name(); }
};
#define DB(args...) debug_block CONCAT(dbbl, __LINE__)([=]{ clog << args; })
#define deb(...)  if (1) { (clog << "[" #__VA_ARGS__ "] = [" << __VA_ARGS__) << "]"; if (!__db_same_line) clog << endl; }
#else
#define clog if (0) cerr
#define DB(...)
#define deb(...)
#endif
template<class T>
ostream& operator,(ostream& out, const T& thing) { return out << ", " << thing; }
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return (out << "(" << p.first, p.second) << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return (out << "(" << get<0>(t), get<1>(t)) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return (out << "(" << get<0>(t), get<1>(t), get<2>(t)) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out, container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }/*}}}*/

// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

tuple<int, int> ordered(int u, int v) {
    if (u > v) swap(u, v);
    return make_tuple(u, v);
}

const int maxn = 201010;
const int lg_maxn = 20;
int n, m;
vector<int> gr[maxn];

int depth[maxn], up[lg_maxn][maxn];
void dfs1(int u, int p) {
    depth[u] = depth[p] + 1;
    up[0][u] = p;
    rep(i, lg_maxn - 1)
        up[i + 1][u] = up[i][up[i][u]];
    for (auto v: gr[u]) {
        if (v != p) dfs1(v, u);
    }
}

int lift(int u, int till_depth) {
    for (int i = 0, d = depth[u] - till_depth; d > 0; i++, d >>= 1)
        if (d & 1) u = up[i][u];
    return u;
}

map<tuple<int, int>, int> cnt_double[maxn];
pair<int, int> till_lca(int u, int v) {
    assert(u != v);
    if (depth[u] > depth[v]) {
        u = lift(u, depth[v] + 1);
        if (up[0][u] == v) return {u, -1};
        u = up[0][u];
    } else if (depth[v] > depth[u]) {
        v = lift(v, depth[u] + 1);
        if (up[0][v] == u) return {-1, v};
        v = up[0][v];
    }
    for (int i = lg_maxn; i--; ) {
        if (up[i][u] == up[i][v]) continue;
        u = up[i][u];
        v = up[i][v];
    }
    return {u, v};
}

int add[maxn] = {0}, rem[maxn] = {0};
llong ans = 0;
void dfs2(int u, int p = - 1) {
    ans += 1ll * add[u] * (add[u] - 1) / 2;
    for (auto v: gr[u]) {
        if (v == p) continue;
        dfs2(v, u);
        ans += 1ll * add[u] * add[v];
        add[u] += add[v];
    }
    for (auto& it: cnt_double[u]) {
        ans -= 1ll * it.second * (it.second - 1) / 2;
    }
    add[u] -= rem[u];
}

int main(void) {
#ifndef LOCAL
    freopen("exercise.in", "r", stdin);
    freopen("exercise.out", "w", stdout);
#endif
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep(i, n - 1) {
        int u, v; cin >> u >> v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }

    dfs1(1, 1);

    for (int i = n - 1; i < m; ++i) {
        int u, v; cin >> u >> v;
        int lu, lv;
        tie(lu, lv) = till_lca(u, v);
        ++add[u];
        ++add[v];
        if (lu == -1) --add[u];
        else ++rem[lu];
        if (lv == -1) --add[v];
        else ++rem[lv];
        if (lu != -1 and lv != -1)
            cnt_double[up[0][lu]][ordered(lu, lv)] ++;
    }

    ans = 0;
    dfs2(1);
    cout << ans;

    return 0;
}

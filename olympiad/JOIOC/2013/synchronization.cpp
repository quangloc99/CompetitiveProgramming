// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://oj.uz/problem/view/JOI13_synchronization
// Idea:
// - When we re-adding an edge (u, v) suppose the answer before adding this edge of u and v were ans[u] and ans[v]
//   respectively. Then their new answer will be ans[u] + ans[v] - c, where c is the common part of the answer of u and
//   v. And actually, c is the last answer when u and v was connected. So before removing (u, v) we store the answer
//   at the edge so we can easily find c when the edge is connected.
// - We not only need to find the answer for each nodes, but also its connected component. We can represents it by 
//   the nodes with lowest depth. One way to find this node that we use fenwick tree to mark which edge is connected (or
//   disconnected) and use binary search to find the lower node which connect to the current node.
//
// Mon Feb 10 17:10:32 MSK 2020: START coding
// Mon Feb 10 17:28:43 MSK 2020: DONE coding
// Mon Feb 10 17:37:46 MSK 2020: DONE debugging
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

/*{{{*/
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int debug = 1;
#define DB(...) stringstream CONCAT(ss, __LINE__); CONCAT(ss, __LINE__) << __VA_ARGS__; debug_block CONCAT(dbbl, __LINE__)(CONCAT(ss, __LINE__).str())
#else
int debug = 0;
#define DB(...)
#endif
int __db_level = 0;
#define clog if (debug) cerr << string(__db_level * 2, ' ')
struct debug_block {
    string name;
    debug_block(const string& name_): name(name_) { clog << "{ " << name << endl; ++__db_level; }
    ~debug_block() { --__db_level; clog << "} " << name << endl; }
};
#define deb(...) "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]"
#define debln(...)  clog << "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]" << endl
#define _ << ", " <<
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return out << "(" << p.first _ p.second << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return out << "(" << get<0>(t) _ get<1>(t) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return out << "(" << get<0>(t) _ get<1>(t) _ get<2>(t) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out _ container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }
/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

const int maxn = 201010;
const int log_maxn = 20;
struct BIT {
    int data[maxn];
    void upd(int i, int del = 1) {
        for (++i; i < maxn; i += i & -i) data[i] += del;
    }

    int get(int i) {
        int ans = 0;
        for (++i; i > 0; i -= i & -i) ans += data[i];
        return ans;
    }

    int get(int l, int r) { // [l; r)
        return get(r) - get(l);
    }
};

int n, m, q;
pair<int, int> edges[maxn];

vector<int> gr[maxn];
int depth[maxn];
int up[log_maxn][maxn];
int euler_start[maxn], euler_stop[maxn], counter = 0;

void pre_dfs(int u, int p) {
    euler_start[u] = counter++;
    up[0][u] = p;
    rep(i, log_maxn - 1) up[i + 1][u] = up[i][up[i][u]];
    for (auto v: gr[u]) {
        if (v == p) continue;
        depth[v] = depth[u] + 1;
        pre_dfs(v, u);
    }
    euler_stop[u] = counter++;
}

int get_ancestor(int u, int d) {
    assert(d <= depth[u]);
    d = depth[u] - d;
    for (int i = 0; d > 0; d >>= 1, ++i)
        if (d & 1) u = up[i][u];
    return u;
}

BIT sum_bit;

int get_presentative(int u) {
    int s = sum_bit.get(euler_start[u]);
    int l = 0, r = depth[u];
    while (r - l > 1) {
        int mid = (l + r + 1) / 2;
        if (sum_bit.get(euler_start[get_ancestor(u, mid)]) == s)
            r = mid;
        else l = mid;
    }
    return get_ancestor(u, r);
}

bool is_connected[maxn] = {0};
int prev_edge_ans[maxn] = {0};
int ans[maxn];

void set_presentative(int u, int val) {
    sum_bit.upd(euler_start[u], val);
    sum_bit.upd(euler_stop[u], -val);
}

void toggle_edge(int id) {
    int u = edges[id].second, p = get_presentative(edges[id].first);
    is_connected[id] ^= 1;
    if (!is_connected[id]) {
        ans[u] = ans[p];
        prev_edge_ans[id] = ans[p];
    } else {
        ans[p] = ans[p] + ans[u] - prev_edge_ans[id];
    }
    set_presentative(u, is_connected[id] ? -1 : 1);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m >> q;
    rep1(e, n - 1) {
        cin >> edges[e].first >> edges[e].second;
        gr[edges[e].first].push_back(edges[e].second);
        gr[edges[e].second].push_back(edges[e].first);
    }
    depth[1] = 1;
    pre_dfs(1, 1);
    fill(ans + 1, ans + n + 1, 1);

    rep1(e, m) {
        if (depth[edges[e].first] > depth[edges[e].second])
            swap(edges[e].first, edges[e].second);
    }

    rep1(i, n) set_presentative(i, 1);

    rep(i, m) {
        int e; cin >> e;
        toggle_edge(e);
    }

    while (q--) {
        int u; cin >> u;
        cout << ans[get_presentative(u)] << '\n';
    }

    return 0;
}

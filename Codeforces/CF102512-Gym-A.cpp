// vim: foldmethod=marker
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://codeforces.com/gym/102512/problem/A
#include <bits/stdc++.h>
using namespace std;

template<class Graph, int node_start_id = 1>
struct block_cut_tree_builder {
    int n;
    vector<bool> is_art;
    vector<int> low, num;
    const Graph& gr;

    block_cut_tree_builder(int n_, const Graph& gr_)
        : n(n_ + node_start_id), is_art(n), low(n), num(n, -1), gr(gr_)
    { }

    vector<int> stk;
    vector<vector<int>> components;
    int root_cnt_child, counter;

    void dfs_find_art(int u, int p = -1) {
        low[u] = num[u] = counter++;
        stk.push_back(u);
        for (auto v: gr[u]) {
            if (v == p) continue;
            if (num[v] == -1) {
                if (p == -1) ++root_cnt_child;
                dfs_find_art(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] < num[u]) continue;
                is_art[u] = p >= 0 or root_cnt_child > 1;
                components.emplace_back(1, u);
                for (auto& cur_comp = components.back(); cur_comp.back() != v; stk.pop_back())
                    cur_comp.push_back(stk.back());
            } else low[u] = min(low[u], num[v]);
        }
    }

    vector<vector<int>> build() {
        counter = 0;
        for (int i = node_start_id; i < n; ++i)
            if (num[i] == -1) {
                root_cnt_child = 0;
                dfs_find_art(i);
            }
        vector<vector<int>> bc_tree(n + components.size() + 1);
        int new_node = n;
        for (auto& comp: components) {
            int cur_node = new_node ++;
            for (auto u: comp) {
                bc_tree[u].push_back(cur_node);
                bc_tree[cur_node].push_back(u);
            }
        }
        return bc_tree;
    }
};

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
const int lg_maxn = 20;
int n, m;
vector<int> gr[maxn];
vector<vector<int>> bc_tree;

int depth[maxn * 2];
int up[lg_maxn + 1][maxn * 2], cnt_actual[lg_maxn + 1][maxn * 2];
void dfs_bc_tree(int u, int p) {
    depth[u] = depth[p] + 1;
    up[0][u] = p;
    cnt_actual[0][u] = u <= n;
    rep(i, lg_maxn) {
        up[i + 1][u] = up[i][up[i][u]];
        cnt_actual[i + 1][u] = cnt_actual[i][u] + cnt_actual[i][up[i][u]];
    }

    for (auto v: bc_tree[u]) {
        if (v != p) dfs_bc_tree(v, u);
    }
}

int count_actual(int u, int v) {
    int ans = -(u <= n) - (v <= n);
    if (depth[u] < depth[v]) swap(u, v);
    for (int i = 0, d = depth[u] - depth[v]; d > 0; d >>= 1, ++ i)
        if (d & 1) {
            ans += cnt_actual[i][u];
            u = up[i][u];
        }

    if (u == v) return ans + (v <= n);
    for (int i = lg_maxn + 1; i--; ) {
        if (up[i][u] == up[i][v]) continue;
        ans += cnt_actual[i][u] + cnt_actual[i][v];
        u = up[i][u];
        v = up[i][v];
    }
    return ans + cnt_actual[0][u] + cnt_actual[0][v] + (up[0][u] <= n);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep(i, m) {
        int u, v; cin >> u >> v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }
    
    bc_tree = block_cut_tree_builder(n, gr).build();
    memset(depth, -1, sizeof(depth));
    rep1(i, n) {
        if (depth[i] != -1) continue;
        depth[i] = 0;
        dfs_bc_tree(i, i);
    }

    int q; cin >> q;
    rep1(cur_query, q) {
        int u, v; cin >> u >> v;
        assert(up[lg_maxn][u] == up[lg_maxn][v]);  // well they are connected

        cout << count_actual(u, v) << '\n';
    }

    return 0;
}

// vim: foldmethod=marker

// Author: Tran Quang Loc (darkkcyan)
// Problem: https://codeforces.com/contest/1000/problem/E
#include <bits/stdc++.h>
using namespace std;

enum BLOCK_TREE_TYPE {
    BLOCK_CUT_TREE = 0, BRIDGE_TREE = 1
};

template<class Graph, int node_start_id = 1>
struct block_tree_builder {
    int n;
    vector<int> low, num;
    const Graph& gr;

    block_tree_builder(int n_, const Graph& gr_)
        : n(n_ + node_start_id), low(n), num(n, -1), gr(gr_)
    { }

    vector<int> stk;
    vector<vector<int>> bc_tree;
    int counter;

    void add_tree_edge(int u, int v) {
        bc_tree[u].emplace_back(v);
        bc_tree[v].emplace_back(u);
        // clog << u << ' ' << v << endl; 
    }

    int new_component(int last_node = -1) {
        int comp_id = (int)bc_tree.size();
        bc_tree.emplace_back();
        for (; stk.size() and (!bc_tree.back().size() or bc_tree.back().back() != last_node); stk.pop_back())
            add_tree_edge(comp_id, stk.back());
        return comp_id;
    }

    template<BLOCK_TREE_TYPE tree_type>
    void dfs(int u, int p = -1) {
        low[u] = num[u] = counter++;
        stk.push_back(u);
        for (auto v: gr[u]) {
            if (v == p) continue;
            if (num[v] != -1) {
                low[u] = min(low[u], num[v]);
                continue;
            }
            dfs<tree_type>(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] < num[u] + tree_type) continue;
            int comp_id = new_component(v);
            add_tree_edge(u, tree_type == BLOCK_CUT_TREE ? comp_id : v);
        }
    }

    template<BLOCK_TREE_TYPE tree_type>
    vector<vector<int>> build() {
        counter = 0;
        bc_tree.assign(n, vector<int>());
        for (int i = node_start_id; i < n; ++i)
            if (num[i] == -1) {
                dfs<tree_type>(i);
                new_component();
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

const int maxn = 301010;
int n, m;
vector<int> gr[maxn];

vector<vector<int>> bridge_tree;
pair<int, int> dfs_bridge_tree(int u, int p = -1) {
    int is_bridge = p != -1 and u <= n and p <= n;
    int ans = is_bridge, max_bridge = 0;
    for (auto v: bridge_tree[u]) {
        if (v == p) continue;
        auto [v_ans, v_max_bridge] = dfs_bridge_tree(v, u);
        ans = max({ans, v_ans, max_bridge + v_max_bridge});
        max_bridge = max(max_bridge, v_max_bridge);
    }
    max_bridge += is_bridge;
    return {ans, max_bridge};
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep(i, m) {
        int u, v; cin >> u >> v;
        gr[u].emplace_back(v);
        gr[v].emplace_back(u);
    }

    bridge_tree = block_tree_builder(n, gr).build<BRIDGE_TREE>();
    cout << dfs_bridge_tree(1).first;

    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/BOI18_minmaxtree
 * Official editorial: https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/Balkan/official/2018/minmaxtree-solution.pdf
 * Sadly, every links to the official website seems to be dead.
 * This solution is basically just the editorial, and this solution is an attemp to implement
 * the linear solution in the editorial.
 * Here are some few issues of that solution:
 * - The 3rd graph (the graph between the left node in the bipartite graph) could be a MULTIGRAPH!!!
 * - I don't think a simple DFS on the 3rd graph can orientate the edges so that each nodes
 *   can have out degree at less 1. Really! For example, if you are going to orientate the edges
 *   from the parent to the child in the dfs tree (and for the back-edge in the dfs tree, we
 *   orientate from the child to the ancestor), then this will be fail in the graph whose
 *   leaf in dfs tree has only 1 degree, in this case we must orientate from the child to its parent.
 *   On the other hand, if the do the other way, that we pick one child and orientate the edge from
 *   that child to the parent, what if the root has only 1 degree???
 * - To solve that problem I changed from dfs to somewhat greedy solution: pick an node
 *   with minimum degrees, assign it and remove it. Repeat the process until there are no nodes left.
 * 
 * The linear solution seemed to be interesting, but now I don't think so since it contains
 * the 2 issues above that cost me lots of time find out where I was wrong. Seriously, the normal 
 * maximum matching algorithm should be implemented!!!
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

#define maxn 70101
#define maxlg 18
int n, k;
vector<int> gr[maxn];
int queries[maxn];
int up[maxlg][maxn];
int depth[maxn];

void dfs(int u, int p) {
    up[0][u] = p;
    depth[u] = depth[p] + 1;
    rep(i, maxlg - 1) up[i + 1][u] = up[i][up[i][u]];
    for (auto v: gr[u]) {
        if (v == p) continue;
        dfs(v, u);
    }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    for (int d = depth[u] - depth[v], i = 0; d > 0; d >>= 1, ++i)
        if (d & 1) u = up[i][u];
    if (u == v) return u;
    for (int i = maxlg; i--; ) {
        if (up[i][u] == up[i][v]) continue;
        u = up[i][u];
        v = up[i][v];
    }
    return up[0][u];
}

template<typename T> 
struct bin_lifting_ranges {
    T value[maxlg][maxn];
    function<T(T const&, T const&)> update_func;
    bin_lifting_ranges(const T& init_val, function<T(const T&, const T&)> const& fun)
        : update_func(fun)
    {
        rep(i, maxlg)   
            fill(value[i], value[i] + maxn, init_val);
    }

    void update(int u, int dst_depth, const T& new_value) {
        for (int i = 0, d = depth[u] - dst_depth; d > 0; d >>= 1, ++i) {
            if (~d & 1) continue;
            value[i][u] = update_func(value[i][u], new_value);
            u = up[i][u];
        }
    }

    void propagate() {
        for (int i = maxlg; --i > 0; ) {
            int f = i - 1;
            rep1(u, n) {
                int v = up[f][u];
                value[f][u] = update_func(value[f][u], value[i][u]);
                value[f][v] = update_func(value[f][v], value[i][u]);
            }
        }
    }

    T& operator[](int i) { return value[0][i]; }
};

bin_lifting_ranges<int>
    min_ranges(-1, [](int u, int v) {
        if (u == -1) return v; else if (v == -1) return u;
        return queries[u] > queries[v] ? u : v;
    }),
    max_ranges(-1, [](int u, int v) {
        if (u == -1) return v; else if (v == -1) return u;
        return queries[u] < queries[v] ? u : v;
    });

vector<int> affect_edges[maxn];
set<int> edge_cost[maxn];
bool used[maxn];
void assign_deg_1() {
    queue<int> qu;
    auto push = [&](int cost) {
        if (used[cost]) return ;
        used[cost] = 1;
        qu.push(cost);
    };
    rep1(i, n) if (len(edge_cost[i]) == 1) {
        push(*edge_cost[i].begin());
    }
    for (; len(qu); qu.pop()) {
        int cost = qu.front();
        for (auto v: affect_edges[cost]) {
            if (len(edge_cost[v]) == 1) assert(edge_cost[v].count(cost));
            if (len(edge_cost[v]) < 2) continue;
            edge_cost[v].erase(cost);
            push(*edge_cost[v].begin());
        }
    }
}

set<pair<int, int>> new_gr[maxn];
bool assigned[maxn] = {0};
void assign_deg_2() {
    rep1(i, n) {
        if (len(edge_cost[i]) < 2) continue;
        int u = *edge_cost[i].begin(), v = *edge_cost[i].rbegin();
        new_gr[u].insert({v, i});
        new_gr[v].insert({u, i});
    }
    set<pair<int, int>> prio;
    rep(i, k) {
        prio.insert({len(new_gr[i]), i});
    }

    while (len(prio)) {
        int u = prio.begin()->yy;
        prio.erase(prio.begin());
        assigned[u] = 1;
        auto t = *new_gr[u].begin();
        edge_cost[t.yy].erase(t.xx);
        if (assigned[t.xx]) continue;
        prio.erase({len(new_gr[t.xx]), t.xx});
        new_gr[t.xx].erase({u, t.yy});
        prio.insert({len(new_gr[t.xx]), t.xx});
    }
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n - 1) {
        int u, v; cin >> u >> v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }

    dfs(1, 1);
    cin >> k;
    rep(i, k) {
        char type; int u, v;
        cin >> type >> u >> v >> queries[i];
        int d = depth[lca(u, v)];
        auto& ranges = type == 'm' ? min_ranges : max_ranges;
        ranges.update(u, d, i);
        ranges.update(v, d, i);
    }

    min_ranges.propagate();
    max_ranges.propagate();

    rep1(i, n) {
        for (auto x: {min_ranges[i], max_ranges[i]}) {
            if (x == -1) continue;
            affect_edges[x].push_back(i);
            edge_cost[i].insert(x);
        }
    }

    assign_deg_1();
    assign_deg_2();
    rep1(i, n) {
        if (up[0][i] == i) continue;
        cout << i << ' ' << up[0][i] << ' ';
        if (len(edge_cost[i]) == 0) cout << "0\n";
        else cout << queries[*edge_cost[i].begin()] << '\n';
    }

    return 0;
}


/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem B in https://codeforces.com/gym/102257/
 * Main idea: sqrt decomposition
 * - Before proceccing, we sort all initial edges in non-increasing order, as well as the queries 
 *   of type one.
 * - Split all the queries into sqrt(q) buckets with size eqausl to sqrt(q).
 * - From each buckets, there will be no more than sqrt(q) queries that update, as well as
 *   the query that ask. 
 * - For each bucket, we gather every edge that will be updated in this bucket. Let's all them "active" edges.
 * - For each query of type 2 in the bucket, we find out what will the cost of the active edges be till this query.
 *   This part can be done in O(q * sqrt(q)) by bruteforcing, requires no special data structure.
 * - Then we process the queries of type 2 in the bucket in non-increasing order. For each of them:
 *   + We join the initially edge that is not active, has not been changed and has cost not less than the considering query.\
 *     (or they are changed but still has the same value as the beginning).
 *   + We also join the updated edges (in the queries of type 1) before the current bucket
 *     that is not active and has the cost not less than the considering query.
 *   + The 2 steps above can be done using 2 pointers technique (that why we sort them in the beginning).
 *   + After those 2 steps, we somehow must join those active edges before this query, and then still has the state
 *     before joining those active edges. For this part I implemented my DSU in special way.
 */
#include <bits/stdc++.h>
#pragma GCC optimize("O3")
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

struct Dsu {
    vector<int> dsu, cnt;
    vector<pair<int*, int>> changes;
    bool store_changes;

    inline void add_change(int& val) { if (store_changes) changes.emplace_back(&val, val); }
    Dsu(int n) : dsu(n), cnt(n, 1), store_changes(0) {
        rep(i, n) dsu[i] = i;
    }

    inline int find_set(int u) { 
        if (u == dsu[u]) return u;
        add_change(dsu[u]);
        dsu[u] = find_set(dsu[u]);
        return dsu[u];
    }

    int get_count(int u) { return cnt[find_set(u)]; }

    inline void join(int u, int v) {
        u = find_set(u);
        v = find_set(v);
        if (u == v) return ;
        if (cnt[u] < cnt[v]) swap(u, v);
        add_change(dsu[v]);
        add_change(cnt[u]);
        dsu[v] = u;
        cnt[u] += cnt[v];
    }

    void join(const pair<int, int>& edge) { join(edge.xx, edge.yy); }

    void restore() {
        for (int i = len(changes); i--; ) *(changes[i].xx) = changes[i].yy;
        changes.clear();
    }
};
#define maxn 101010
int n, m, q;
pair<int, int> edges[maxn];
vector<pair<int, int>> init_cost, updated_cost;
int cur_edge_cost[maxn];

struct query {
    int type; 
    union {
        struct { int edge_id, new_cost; };
        struct { int source, weight, ans; };
    };
    vector<pair<int, int>> cur_block_changed_edges;
};

query qu[maxn];

void solve() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;

    rep1(i, m) {
        cin >> edges[i].xx >> edges[i].yy >> cur_edge_cost[i];
        init_cost.emplace_back(cur_edge_cost[i], i);
    }

    cin >> q;
    rep(i, q) {
        cin >> qu[i].type;
        if (qu[i].type == 1) cin >> qu[i].edge_id >> qu[i].new_cost;
        else cin >> qu[i].source >> qu[i].weight;

        if (qu[i].type == 1) updated_cost.emplace_back(qu[i].new_cost, qu[i].edge_id);
    }

    sort(init_cost.rbegin(), init_cost.rend());
    sort(updated_cost.rbegin(), updated_cost.rend());

    int block_size = (int)round(sqrt(q));
    // int block_size = 1;

    static int active_id[maxn];
    memset(active_id, -1, sizeof active_id);
    for (int block_start = 0; block_start < q; block_start += block_size) {
        int block_end = min(block_start + block_size, q);
        vector<pair<int, int>> active_edges;

        for (int i = block_start; i < block_end; ++i) {
            if (qu[i].type != 1 or active_id[qu[i].edge_id] != -1) continue;
            active_id[qu[i].edge_id] = len(active_edges);
            active_edges.emplace_back(qu[i].edge_id, cur_edge_cost[qu[i].edge_id]);
        }

        for (int i = block_start; i < block_end; ++i) {
            if (qu[i].type == 1) active_edges[active_id[qu[i].edge_id]].yy = qu[i].new_cost;
            else qu[i].cur_block_changed_edges = active_edges;
        }


        vector<query*> ask_queries;
        for (int i = block_start; i < block_end; ++i)
            if (qu[i].type == 2) ask_queries.emplace_back(qu + i);

        sort(ask_queries.begin(), ask_queries.end(), [](query* u, query* v) { return u->weight > v->weight; });
        Dsu dsu(n + 1);
        auto init_cost_it = init_cost.begin();
        auto updated_cost_it = updated_cost.begin();
        for (auto cur_qu: ask_queries) {
            dsu.store_changes = 0;

            for (; init_cost_it != init_cost.end() and init_cost_it->xx >= cur_qu->weight; ++init_cost_it) {
                if (active_id[init_cost_it->yy] != -1 or cur_edge_cost[init_cost_it->yy] != init_cost_it->xx) continue;
                dsu.join(edges[init_cost_it->yy]);
            }

            for (; updated_cost_it != updated_cost.end() and updated_cost_it->xx >= cur_qu->weight; ++updated_cost_it) {
                if (active_id[updated_cost_it->yy] != -1 or cur_edge_cost[updated_cost_it->yy] != updated_cost_it->xx) continue;
                dsu.join(edges[updated_cost_it->yy]);
            }

            dsu.store_changes = 1;
            for (auto& i: cur_qu->cur_block_changed_edges) {
                if (i.yy < cur_qu->weight) continue;
                dsu.join(edges[i.xx]);
            }
            cur_qu->ans = dsu.get_count(cur_qu->source);
            dsu.restore();
        }

        for (auto i: active_edges) {
            cur_edge_cost[i.xx] = i.yy;
            active_id[i.xx] = -1;
        }
    }

    rep(i, q) if (qu[i].type == 2) 
        cout << qu[i].ans << '\n';
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    solve();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    clog << (float)duration / 1000000;
    return 0;
}

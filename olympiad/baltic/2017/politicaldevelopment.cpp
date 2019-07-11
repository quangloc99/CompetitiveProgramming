/**
 * Author: Tran Quang Loc (darkkcyan)
 * Link to problem + online submitting: https://open.kattis.com/problems/politicaldevelopment
 *
 * Idea: greedily pick the node with smallest degree. We are sure that this is always be the node with degree < k.
 * Take all of its neightbor, construct a subgraph with these nodes and find the maximum clique locally in this graph.
 * Finally, remove this node from the (global) graph. Some nodes will have degree decreased, so we can continue these steps
 * until there no more nodes left.
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
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

int find_max_clique(vector<int> const& gr_mask) {
    int ans = 0;
    rep(mask, (1 << len(gr_mask))) {
        bool ok = true;
        rep(i, len(gr_mask)) {
            if ((~mask >> i) & 1) continue;
            if ((gr_mask[i] & mask) == mask) continue;
            ok = false;
            break;
        }

        if (!ok) continue;
        ans = max(ans, __builtin_popcount(mask));
    }
    return ans;
}

#define maxn 50500
int n, k;
set<int> gr[maxn];
vector<int> reversed_gr[maxn];

vector<int> build_gr_mask(vector<int> const& nodes) {
    vector<int> gr_mask(len(nodes));
    rep(u, len(nodes)) {
        gr_mask[u] = 1 << u;
        rep(v, len(nodes)) {
            if (!gr[nodes[u]].count(nodes[v])) continue;
            gr_mask[u] |= 1 << v;
        }
        // clog << u << ' ' << bitset<3>(gr_mask[u]) << endl;
    }
    return move(gr_mask);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> k;
    rep(i, n) {
        int cnt; cin >> cnt;
        while (cnt --) {
            int u; cin >> u;
            gr[i].insert(u);
            reversed_gr[u].push_back(i);
        }
    }

    int ans = 0;
    set<pair<int, int>> prio;
    rep(i, n) prio.insert({len(gr[i]), i});

    while (len(prio)) {
        int u = prio.begin()->yy;
        prio.erase(prio.begin());
        assert(len(gr[u]) < k);

        vector<int> nodes(gr[u].begin(), gr[u].end());
        nodes.push_back(u);
        ans = max(ans, find_max_clique(build_gr_mask(nodes)));

        for (auto i: reversed_gr[u]) {
            if (!prio.count({len(gr[i]), i})) continue;
            prio.erase({len(gr[i]), i});
            gr[i].erase(u);
            prio.insert({len(gr[i]), i});
        }
        reversed_gr[u].clear();
        gr[u].clear();
    }
    cout << ans;

    return 0;
}

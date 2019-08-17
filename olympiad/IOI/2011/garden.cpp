/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/IOI11_garden
 * Test data + solution: https://ioinformatics.org/page/ioi-2011/37
 * My idea seems to be similar to the official solution, but my solution run in O(M + N + Q*logN)
 * Idea:
 * - Build a new directional graph with 2 * N nodes. For each node in an old graph, we make 2 nodes corresponding to 
 *   2 states of the original node: when we enter this node using the most beautiful edge to this node, 
 *   and when we don't.
 * - When building the graph we are not going to use all the edges. Since from a state when can determine where
 *   to go next, so we got totally 2 * N nodes with 2 * N edges.
 * - This graph is special: it is an "almost" tree (or forest of "almost" tree). The tree will have 2 * N - 1 edges 
 *   and also 1 additional edge. This "almost" tree will now have 1 cycle.
 * - Notice that our node P now has 2 corresponding nodes. For each of them, we find out whether it lies inside the cycle
 *   and the distance from the other nodes to it. If it lies in the cycle, then we group the nodes by their distance modulo
 *   the cycle, otherwise we just group them by their distance. After that we sort each group by their distance.
 * - For each query, we also consider P's 2 corresponding nodes. If the corresponding node does not lie in a cycle, then
 *   we already got the result since we group the nodes by the distance. Otherwise the group we want the answer
 *   is the group G[i] % (cycle_length), then we can do BINARY SEARCH to find what nodes can reach P.
 */
#include<bits/stdc++.h>
#include "garden.h"
#include "gardenlib.h"

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

void count_routes(int N, int M, int P, int R[][2], int Q, int G[]) {
    const int inf = 2 * N + 10;

    vector<int> next_node(N * 2, -1);
    rep(i, M) {
        int prio[] = {next_node[R[i][0] << 1] == -1, next_node[R[i][1] << 1] == -1};
        rep(f, 2) {
            int u = R[i][f], v = R[i][!f];
            bool v_prio = prio[!f];
            if (next_node[u << 1] == -1)
                next_node[u << 1] = v << 1 | v_prio; 
            else if (next_node[u << 1 | 1] == -1) 
                next_node[u << 1 | 1] = v << 1 | v_prio;
        }
    }

    rep(i, N) {
        if (next_node[i << 1] == -1) continue;
        if (next_node[i << 1 | 1] != -1) continue;
        next_node[i << 1 | 1] = next_node[i << 1] & ~1;
        if (next_node[next_node[i << 1 | 1]] / 2 == i)
            next_node[i << 1 | 1] |= 1;
    }

    vector<vector<int>> gr(2 * N);
    rep(i, 2 * N) {
        if (next_node[i] == -1) continue;
        gr[next_node[i]].emplace_back(i);
    }

    auto bfs = [&](int root) -> vector<int> {
        vector<int> dis(2 * N, -1);
        queue<int> qu;
        for (qu.push(root), dis[root] = 0; len(qu); qu.pop()) {
            int u = qu.front();
            for (auto v: gr[u]) {
                if (dis[v] != -1) continue;
                dis[v] = dis[u] + 1;
                qu.push(v);
            }
        }
        return dis;
    };

    vector<int> dis_to_p[2];
    int cycle_length[2] = {inf, inf};
    rep(i, 2) {
        int u = P << 1 | i;
        dis_to_p[i] = bfs(u);
        if (next_node[u] == -1) continue;
        if (dis_to_p[i][next_node[u]] == -1) continue;
        cycle_length[i] = dis_to_p[i][next_node[u]] - dis_to_p[i][u] + 1;
    }

    vector<vector<int>> group[2];
    rep(i, 2) {
        group[i].resize(2 * N);
        for (int v = 0; v < 2 * N; v += 2) {
            int dis = dis_to_p[i][v];
            if (dis == -1) continue;
            group[i][dis % cycle_length[i]].emplace_back(dis);
        }

        for (auto& f: group[i]) 
            sort(f.begin(), f.end());
    }

    rep(i, Q) {
        int ans = 0;
        int query = G[i];
        rep(f, 2) {
            if (cycle_length[f] >= inf) {
                if (query >= inf) continue;
                ans += len(group[f][query]);
            } else {
                auto& cur_group = group[f][query % cycle_length[f]];
                auto iter = upper_bound(cur_group.begin(), cur_group.end(), query);
                ans += (int)(iter - cur_group.begin());
            }
        }
        answer(ans);
    }
}

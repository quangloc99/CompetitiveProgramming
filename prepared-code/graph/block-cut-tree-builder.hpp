// Author: Tran Quang Loc (darkkcyan)
// Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF102512-Gym-A.cpp

// Tree's structure built with this code:
// - Suppose there are n nodes in the original graphs, and its has m biconnected components.
//   The built tree will have n + m nodes in total.
// - The tree includes all n nodes in the original graphs (numbered from 1 to n, or you can adjust node_start_id).
//   m nodes will represent the binconnected comonents (numbered from n + 1 to n + m).
// - If node u in the original graph is in the component v, then nodes u and v + n in the tree will be connected.
// - Hence the tree has this properties:
//   + All original nodes has label <= n.
//   + All leaves in the tree are original nodes which are not articular points.
//   + All nodes with label <= n, which are not leaves, are articular points in the original graph.
//   + You can write some more if you want.

template<class Graph, int node_start_id = 1>
struct block_cut_tree_builder {
    int n;
    vector<int> low, num;
    const Graph& gr;

    block_cut_tree_builder(int n_, const Graph& gr_)
        : n(n_ + node_start_id), low(n), num(n, -1), gr(gr_)
    { }

    vector<int> stk;
    vector<vector<int>> bc_tree;
    int counter;

    void dfs(int u, int p = -1) {
        low[u] = num[u] = counter++;
        stk.push_back(u);
        for (auto v: gr[u]) {
            if (v == p) continue;
            if (num[v] != -1) {
                low[u] = min(low[u], num[v]);
                continue;
            }
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] < num[u]) continue;
            // now this node is articular points if it is non-root or it has more than 1 child.
            int comp_id = (int)bc_tree.size();
            bc_tree.emplace_back();
            for (int cur_node = u; ; cur_node = stk.back(), stk.pop_back()) {
                bc_tree[comp_id].push_back(cur_node);
                bc_tree[cur_node].push_back(comp_id);
                if (cur_node == v) break;
            }
        }
    }

    vector<vector<int>> build() {
        counter = 0;
        bc_tree.assign(n, vector<int>());
        for (int i = node_start_id; i < n; ++i)
            if (num[i] == -1) dfs(i);
        return bc_tree;
    }
};

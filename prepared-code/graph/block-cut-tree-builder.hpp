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

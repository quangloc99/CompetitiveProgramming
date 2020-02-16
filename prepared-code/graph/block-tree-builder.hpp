// Author: Tran Quang Loc (darkkcyan)

// Block cut tree tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF102512-Gym-A.cpp
// Bridge tree tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF1000-D2-E.cpp

// Tree's structure built with this code:
// - Suppose there are n nodes in the original graphs, and its has m biconnected/bridge components.
//   The built tree will have n + m nodes in total.
// - The tree includes all n nodes in the original graphs (numbered from 1 to n, or you can adjust node_start_id).
//   m nodes will represent the binconnected comonents (numbered from n + 1 to n + m).
// - If node u in the original graph is in the component v, then nodes u and v + n in the tree will be connected.
// - If the built tree is a bridge tree and (u, v) is an edge in the original graph, then (u, v) is also connected in
//   the tree.

// See also: 
// Block cut tree: https://en.wikipedia.org/wiki/Biconnected_component
// Bridge tree: https://www.quora.com/q/threadsiiithyderabad/The-Bridge-Tree-of-a-graph

enum BLOCK_TREE_TYPE { BLOCK_CUT_TREE = 0, BRIDGE_TREE = 1 };

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

// input:
// n: number of nodes.
// gr: adjacentcy lists of the graph.

// Usage:
// auto bc_tree = block_tree_builder(n, gr).build<BLOCK_CUT_TREE>();
// auto bridge_tree = block_tree_builder(n, gr).build<BRIDGE_TREE>();

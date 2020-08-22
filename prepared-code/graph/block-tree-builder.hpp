// Author: Tran Quang Loc (darkkcyan)
// See also: 
// Block cut tree: https://en.wikipedia.org/wiki/Biconnected_component
// Bridge tree: https://www.quora.com/q/threadsiiithyderabad/The-Bridge-Tree-of-a-graph

// Block cut tree tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF102512-Gym-A.cpp
// Bridge tree tested with https://codeforces.com/contest/1000/submission/88415652

// # Basic tree's structure built with this code:
// - Suppose there are n nodes in the original graphs, and its has m biconnected/bridge components.
//   The built tree will have n + m nodes in total.
// - The tree includes all n nodes in the original graphs (numbered from 1 to n, or you can adjust node_start_id).
//   m nodes will represent the binconnected comonents (numbered from n + 1 to n + m).
// - If node u in the original graph is in the component v, then nodes u and v + n in the tree will be connected.
// - If the built tree is a bridge tree and (u, v) is an edge in the original graph, then (u, v) is also connected in
//   the tree.
// This is NOT tree of components, but tree of both components and original nodes.
// But there is also a helper function at the end when iterating tree of components is really needed (but really, try not to use it).

// # MUST READ: why this structure instead of an actual tree of components + array of nodes of each component?
// Typically, with tree of components + array of nodes, we must do the following:
// 1) Gather information of nodes inside each component
// 2) Solve the problem for the tree.
// 3) (optional) calculate the actual answer for each nodes inside the component.
// But with this structure, we really don't need step 1 and 3, just 2,
// thus we can write MUCH SIMPLER code, as we just solve the problem with the tree. Here is an example:
// - With tree of component (still this code but with helper function): https://codeforces.com/contest/1389/submission/88414059
// - With this structure: https://codeforces.com/contest/1389/submission/88415059

// # Additional properties (the code can be removed code if not needed)
// - Let's bc be the result of the function. For each i be the original node, the comopnent id will be bc[i][0]

enum BLOCK_TREE_TYPE { BLOCK_CUT_TREE = 0, BRIDGE_TREE = 1 };

// input:
// n: number of nodes.
// gr: adjacentcy lists of the graph.
// Usage:
// auto bc_tree = build_block_tree<BLOCK_CUT_TREE>(n, gr);
// auto bridge_tree = build_block_tree<BRIDGE_TREE>(n, gr);
template<BLOCK_TREE_TYPE tree_type, int node_start_id = 1, class Graph = vector<int>[]>
vector<vector<int>> build_block_tree(int n, const Graph& gr) {
    n += node_start_id;
    vector<int> low(n), num(n, -1), stk;
    vector<vector<int>> bc_tree(n);
    
    auto new_edge = [&](int u, int v) {
        bc_tree[u].push_back(v);
        bc_tree[v].push_back(u);
    };
    
    auto new_component = [&](int last_node = -1) {
        int comp_id = (int)bc_tree.size();
        bc_tree.emplace_back();
        for (; stk.size() and (!bc_tree.back().size() or bc_tree.back().back() != last_node); stk.pop_back())
            new_edge(comp_id, stk.back());
        return comp_id;
    };
    
    int counter = 0;
    function<void(int, int)> dfs = [&](int u, int p) {
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
            if (low[v] < num[u] + tree_type) continue;
            int comp_id = new_component(v);
            new_edge(u, tree_type == BLOCK_CUT_TREE ? comp_id : v);
        }
    };
    
    for (int i = node_start_id; i < n; ++i) {
        if (num[i] == -1) {
            dfs(i, -1);
            if (tree_type == BRIDGE_TREE) new_component();
        }
        // make the component id to be the first element. can be removed if not needed
        for (int& f: bc_tree[i]) if (f > n) {
            swap(bc_tree[i][0], f);
            break;
        }
        // modification?: move all non-bridge/cut point to the end for faster iterating through adj component.
    }
    return bc_tree;
}

// Helper function for iterating tree of components base on generated tree.
// bc_tree: the generated tree from the pervious function.
// comp: the component id.
// fn: the call back
// Usage:
// foreach_adj_comp(bc_tree, comp, [&](int adj_comp, int eu, int ev) {
//     // adj_comp is adjacency component to comp,
//     // connected by bridge (eu-ev) if bc is bridge tree.
//     // or cut-point eu if bc is block_cut_tree, (ev will also be adj_comp but not important)
//     // `return` in this function will have the same affect as `continue`.
//     // and `throw false` is the same as `break`.
// }):
template<BLOCK_TREE_TYPE tree_type>
void foreach_adj_comp(const vector<vector<int>>& bc_tree, int comp, function<void(int, int, int)> fn) {
    try {
        for (int elm: bc_tree[comp]) if (bc_tree[elm].size() > 1)
            for (int oelm: bc_tree[elm]) {
                if (oelm == comp) continue;
                int adj_comp = tree_type == BRIDGE_TREE ? bc_tree[oelm][0] : oelm;
                fn(adj_comp, elm, oelm);
            }
    } catch (bool break_signal) {}
}

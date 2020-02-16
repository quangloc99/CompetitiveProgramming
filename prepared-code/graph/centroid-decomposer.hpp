/**
 * Author: Tran Quang Loc (darkkcyan)
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF342-D2-E.cpp
 */
template<class Graph, int node_start_id = 1>
struct centroid_decomposer {
    int n;
    const Graph& gr;
    vector<bool> mark;
    vector<int> child_cnt;

    centroid_decomposer(int n_, const Graph& gr_)
        : n(n_ + node_start_id), gr(gr_), mark(n), child_cnt(n) {}

    int dfs_count_child(int u, int p) {
        child_cnt[u] = 1;
        for (auto v: gr[u]) if (v != p and !mark[v])
            child_cnt[u] += dfs_count_child(v, u);
        return child_cnt[u];
    }

    // possible modification: return a pair which contain a new centroid and
    // the size of its region (the `total` variable)
    int find_centroid(int u, int p = -1, int total = -1) {
        assert(u != -1);
        if (p == -1) total = dfs_count_child(u, u);
        pair<int, int> big_child(total - child_cnt[u], -1);
        for (auto v: gr[u]) if (!mark[v] and v != p)
            big_child = max(big_child, {child_cnt[v], v});
        if (big_child.first > total / 2) 
            return find_centroid(big_child.second, u, total);
        mark[u] = 1;
        return u;
    }

    // possible modification: return adjacency list instead of parent list
    vector<int> build_centroid_tree(int s = node_start_id) {
        s = find_centroid(s);
        vector<int> cen_parent(n, -1);
        queue<int> qu;
        for (qu.push(s), cen_parent[s] = s; qu.size(); qu.pop()) {
            int u = qu.front();
            for (auto v: gr[u]) {
                if (mark[v]) continue;
                int cv = find_centroid(v);
                cen_parent[cv] = u;
                qu.push(cv);
            }
        }
        return cen_parent;
    }
};

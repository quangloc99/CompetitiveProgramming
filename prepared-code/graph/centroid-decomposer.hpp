/**
 * Author: Tran Quang Loc (darkkcyan)
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF342-D2-E.cpp
 */
template<int nn> class centroid_decomposer {
    int childcnt[nn];
    int dfscountchild(int u, int p) {
        childcnt[u] = 1;
        for (auto v: gr[u]) if (v != p and !mark[v])
            childcnt[u] += dfscountchild(v, u);
        return childcnt[u];
    }

public:
    // uncomment these 2 lines for custom graph
    // int n;
    // vector<int> gr[nn];
    bitset<nn> mark;

    // possible modification:
    // - return a pair which contain a new centroid and the size of its region (which is the `total` variable)
    int find_centroid(int u, int p = -1, int total = -1) {
        assert(u != -1);
        if (p == -1) total = dfscountchild(u, u);
        pair<int, int> big_child(total - childcnt[u], -1);
        for (auto v: gr[u]) if (!mark[v] and v != p)
            big_child = max(big_child, {childcnt[v], v});
        if (big_child.first > total / 2) 
            return find_centroid(big_child.second, u, total);
        mark[u] = 1;
        return u;
    }

    // possible modification:
    // - return adjacency list instead of parent list
    vector<int> build_centroid_tree(int s = 1) {
        s = find_centroid(s);
        vector<int> cen_parent(n + 1, -1);
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

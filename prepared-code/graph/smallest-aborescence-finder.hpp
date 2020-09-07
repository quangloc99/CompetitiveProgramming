#define SAFinder SmallestAborescenceFinder  // fucking long name
template<class cost_type = long long, size_t buff_size = 300100, int start_id = 1>
struct SmallestAborescenceFinder {
    // randomized heap
    struct edge_heap {
        void* operator new (size_t sz) {
            static char buff[buff_size * sizeof(edge_heap)];
            static char* ptr = buff;
            char* ans = ptr;
            ptr += sz;
            return ans;
        }
        
        int from, to;
        cost_type cost, lazy;
        edge_heap *l, *r;
        edge_heap(int f = 0, int t = 0, cost_type c = 0, cost_type lz = 0)
            : from(f), to(t), cost(c), lazy(lz) {}
    };
    
    static void propagate(edge_heap* hp) {
        if (!hp) return ;
        hp->cost -= hp->lazy;
        if (hp->l) hp->l->lazy += hp->lazy;
        if (hp->r) hp->r->lazy += hp->lazy;
        hp->lazy = 0;
    }
    
    static edge_heap* merge(edge_heap* u, edge_heap* v) {
        static mt19937 rng(time(0));
        propagate(u);
        propagate(v);
        if (!u) return v;
        if (!v) return u;
        if (u->cost > v->cost) swap(u, v);
        if (rng() & 1) swap(u->l, u->r);
        u->l = merge(u->l, v);
        return u;
    }
    
    // DSU that supports union by rank and callback when 2 nodes join for additional operations if needed.
    struct DSU {
        vector<int> par, rank;
        function<void(int, int)> cb;
        DSU(int n, function<void(int, int)> cb_ = [](int, int){}) : par(n + 10), rank(n + 10, 1), cb(cb_) {
            for (int i = (int)par.size(); i--; )  par[i] = i;
        }

        int find_set(int u) { return u == par[u] ? u : par[u] = find_set(par[u]); }
        void join(int u, int v) {
            u = find_set(u);
            v = find_set(v);
            if (u == v) return ;
            if (rank[u] == rank[v]) ++rank[v];
            else if (rank[u] > rank[v]) swap(u, v);
            par[u] = v;
            cb(v, u);
        }
    };
    
    int n, root;
    vector<edge_heap*> edge_set;
    vector<int> fv;
    
    SmallestAborescenceFinder(int n_, int root_)
        : n(n_ + start_id), root(root_), edge_set(n), fv(n, -1) {}
    
    cost_type shift_cost = 0;
    void add_edge(int from, int to, cost_type cost) {
        if (to == root) return ;
        shift_cost = min(shift_cost, cost);
        edge_set[to] = merge(edge_set[to], new edge_heap(from, to, cost));
    }
    
    optional<cost_type> find() {
        cost_type ans = -shift_cost * (n - 1 - start_id);  // - start_id because at the beginning start_id was added to n.
        
        // step 1: add all non-root nodes to the queue
        queue<int> qu;
        for (int i = start_id; i < n; ++i) {
            if (i == root or !edge_set[i]) continue;
            edge_set[i]->lazy -= shift_cost;
            propagate(edge_set[i]);
            qu.push(i);
        }

        DSU d1(n), d2(n, [&](int u, int v) {  // we also need to merge the edge sets of u and v when these 2 nodes are joined
            edge_set[u] = merge(edge_set[u], edge_set[v]);
            edge_set[v] = 0;
        });

        vector zero_edge_gr(n, vector<int>());
        
        for (; qu.size(); qu.pop()) {
            int u = d2.find_set(qu.front());
            
            // step 2.1: find the smallest edge that go into u.
            auto mx = *edge_set[u];
            int v = d2.find_set(mx.from);
            fv[u] = v;
            
            edge_set[u] = merge(edge_set[u]->l, edge_set[u]->r);
            ans += mx.cost;
            zero_edge_gr[mx.from].push_back(mx.to);
            
            // step 2.2: subtract all edges of the set by mx.cost
            if (edge_set[u]) {
                edge_set[u]->lazy += mx.cost;
                propagate(edge_set[u]);
            }
            
            if (d1.find_set(u) == d1.find_set(v)) {
                // step 3: find the edge
                vector<int> nodes;  // need to get all the nodes before joining,
                                    // because otherwise their set changes while joining.
                for (int x = v; x != u; x = d2.find_set(fv[x])) nodes.push_back(x);
                
                // step 4: join them
                for (auto x: nodes) {
                    d2.join(u, x);
                }
                
                // step 5: remove new rings
                int joined = d2.find_set(u);
                while (edge_set[joined] and d2.find_set(edge_set[joined]->from) == joined) {
                    edge_set[joined] = merge(edge_set[joined]->l, edge_set[joined]->r);
                }
                
                // step 6: add new node to the queue
                if (edge_set[joined]) qu.push(joined);
            }
            d1.join(u, v);
        }

        // check and find the actual tree
        
        fill(fv.begin(), fv.end(), -1);
        int cnt = start_id;
        
        // build the dfs tree of the graph created by all zero_edge, and the latest added
        // nodes need to be tranversed first. the tree will be the answer. 
        // This is correct, because:
        // - back edge should always be removed.
        // - we need to break the latest joined cycle before the other, hence we need to 
        // tranverse lastest added nodes first. so the cross edge will also need to be removed.
        function<void(int)> dfs = [&](int u) {
            ++cnt;
            for (int i = (int)zero_edge_gr[u].size(); i--; ) {
                int v = zero_edge_gr[u][i];
                if (fv[v] != -1) continue;
                fv[v] = u;
                dfs(v);
            }
        };
        dfs(root);
        bool has_ans = cnt == n;
        if (has_ans) return {ans};
        return {};
    }
    
};

#define SAFinder SmallestAborescenceFinder  // fucking long name
bool clear_buff = false;  // [optinal] dirty flag for clearing the buffer
template<class cost_type = long long, size_t buff_size = 300100, int start_id = 1>
struct SmallestAborescenceFinder {
    // randomized heap
    struct edge_heap {
        void* operator new (size_t sz) {
            static char buff[buff_size * sizeof(edge_heap)];
            static char* ptr = buff;
            if (clear_buff) {
                ptr = buff;
                clear_buff = false;
            }
            char* ans = ptr;
            ptr += sz;
            return ans;
        }
        
        int from, to;
        cost_type cost, lazy;
        edge_heap *l, *r;
        edge_heap(int f = 0, int t = 0, cost_type c = 0, cost_type lz = 0)
            : from(f), to(t), cost(c), lazy(lz), l(0), r(0) {}
    };

    
    static void propagate(edge_heap* hp) {
        if (!hp) return ;
        hp->cost -= hp->lazy;
        if (hp->l) hp->l->lazy += hp->lazy;
        if (hp->r) hp->r->lazy += hp->lazy;
        hp->lazy = 0;
    }
    
    static edge_heap* merge(edge_heap* u, edge_heap* v) {
        static mt19937 rng_(time(0));
        propagate(u);
        propagate(v);
        if (!u) return v;
        if (!v) return u;
        if (u->cost > v->cost) swap(u, v);
        if (rng_() & 1) swap(u->l, u->r);
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
    
    // return nothing if there is no answer, otherwise the smallest aborescence
    // the built tree can be access via fv array.
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

        // picked_edges[0] will be edges of the tree in the compressed graph
        // picked_edges[i] for i > 0 will stores cycles
        //   ealier compressed cycle will have smaller id
        // list are use for fast removal
        vector<list<pair<int, int>>> picked_edges(1);
        vector<list<pair<int, int>>::iterator> fv_it(n);  // store the position of the edge in the list for fast removal
        vector cycle_list(n, list<int>());                // list of "sorted" cycles's id for each nodes.
        
        for (; qu.size(); qu.pop()) {
            int u = d2.find_set(qu.front());
            
            // step 2.1: find the smallest edge that go into u.
            auto mx = *edge_set[u];
            int v = d2.find_set(mx.from);
            
            fv[u] = mx.from;
            picked_edges[0].emplace_front(mx.from, mx.to);
            fv_it[u] = picked_edges[0].begin();
            
            edge_set[u] = merge(edge_set[u]->l, edge_set[u]->r);
            ans += mx.cost;
            
            // step 2.2: subtract all edges of the set by mx.cost
            if (edge_set[u]) {
                edge_set[u]->lazy += mx.cost;
                propagate(edge_set[u]);
            }
            
            if (d1.find_set(u) == d1.find_set(v)) {
                // step 3: find the edge
                vector<int> nodes = {u};  // need to get all the nodes before joining,
                                    // because otherwise their set changes while joining.
                for (int x = v; x != u; x = d2.find_set(fv[x])) nodes.push_back(x);
                
                // step 4: join them, create new cycle
                list<pair<int, int>> cycle;
                int cycle_id = (int)picked_edges.size();
                for (auto x: nodes) {
                    d2.join(u, x);
                    cycle.push_back(*fv_it[x]);
                    cycle_list[fv_it[x]->second].push_back(cycle_id);
                    picked_edges[0].erase(fv_it[x]);
                }
                picked_edges.push_back(move(cycle));
                
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
        
        // cycle are iterated in the following order:
        // - cycle having ealier discovered nodes will be tranversed first
        // - cycle compressed first (having smaller id) willbe tranversed first
        function<void(list<pair<int, int>>&)> assign_edge = [&](list<pair<int, int>>& edge_list) {
            vector<int> nodes;
            for (auto [from, to]: edge_list) {
                nodes.push_back(to);
                if (fv[to] == -1) fv[to] = from;
            }
            edge_list.clear();
            for (auto u: nodes) {
                for (auto cycle_id: cycle_list[u]) {
                    assign_edge(picked_edges[cycle_id]);
                }
            }
        };
        
        assign_edge(picked_edges[0]);
        for (int i = start_id; i < n; ++i) {
            if (i == root) continue;
            if (fv[i] == -1) {
                return {};
            }
        }
        
        return {ans};
    }
    
};

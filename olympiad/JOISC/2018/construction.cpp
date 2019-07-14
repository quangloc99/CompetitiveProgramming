/**
 * Author: Tran Quang Loc (darkkcyan)
 * Link to problem + online submiting: https://oj.uz/problem/view/JOI18_construction
 * Idea + proof in the comment section of zscoder: https://codeforces.com/blog/entry/58433?#comment-422392
 * About the proof, I just want to talk more about the heavy separator.
 * What ko_osaga was saying is, suppose we are updating the liveliness from a node, say x. For each light edge from this node to the root, say (u, v)
 * (u is the parent of v), the heavy edge connected from u  will be a new separator. The total number of heavy separator made by this operation
 * equals to the number of light edges from x. That make the overall total number of heavy separator equal to the number of light edges of all nodes,
 * and that is O(nlogn).
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

#define maxn 101010
int n;
int c[maxn];
vector<int> gr[maxn];
int parent[maxn];
int query[maxn];

int subtree_size[maxn];
shared_ptr<int> chain_head[maxn];
int chain_id[maxn];
void build_hld(int u) {
    subtree_size[u] = 1;
    for (auto v: gr[u]) {
        build_hld(v);
        subtree_size[u] += subtree_size[v];
        if (!chain_head[u] or subtree_size[*chain_head[u]] < subtree_size[*chain_head[v]])
            chain_head[u] = chain_head[v];
    }
    if (!chain_head[u]) chain_head[u].reset(new int());
    chain_id[u] = chain_id[*chain_head[u]] + 1;
    *chain_head[u] = u;
}

deque<pair<int, int>> chain_data[maxn];
vector<pair<int, int>> do_operation(int u, int liveliness) {
    if (u == 0) return {};
    int head = *chain_head[u];
    auto ans = do_operation(parent[head], liveliness);
    int n_remove = chain_id[head] - chain_id[u] + 1;
    int s = n_remove;

    while (s > 0 and chain_data[head].size()) {   // check the size just for safety.
        int n_removing = min(s, chain_data[head].front().yy);
        s -= n_removing;
        chain_data[head].front().yy -= n_removing;
        ans.emplace_back(chain_data[head].front().xx, n_removing);
        if (chain_data[head].front().yy == 0) chain_data[head].pop_front();
    }

    if (s > 0) {
        assert(false);
    }

    chain_data[head].emplace_front(liveliness, n_remove);
    return move(ans);
}

struct Bit {
    vector<llong> data;
    Bit(int size) : data(size + 10) {}
    void upd(int i, llong val) { for (++i; i < len(data); i += i & -i) data[i] += val; }
    llong get(int i) { llong ans = 0; for (++i; i > 0; i -= i & -i) ans += data[i]; return ans; }
};

llong compute_cost(const vector<pair<int, int>>& chain) {
    vector<int> vals;
    for (auto i: chain) vals.push_back(i.xx);
    sort(vals.begin(), vals.end());
    vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
    llong ans = 0;
    Bit bit(len(vals));
    for (int i = len(chain); i--; ) {
        int cur_val = (int)(lower_bound(vals.begin(), vals.end(), chain[i].xx) - vals.begin());
        llong cur_ans = 1ll * chain[i].yy * bit.get(cur_val - 1);
        ans += cur_ans;
        bit.upd(cur_val, chain[i].yy);
    }
    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep1(i, n) cin >> c[i];
    parent[1] = 0;
    rep(i, n - 1) {
        int p, u; cin >> p >> u;
        query[i] = u;
        parent[u] = p;
        gr[p].push_back(u);
    }

    build_hld(1);
    chain_data[1].emplace_back(c[1], 1);
    rep(i, n - 1) {
        int u = query[i];
        cout << compute_cost(do_operation(parent[u], c[u])) << '\n';
        int head = *chain_head[u];
        if (head == u) {
            chain_data[head].emplace_front(c[u], 1);
        } else {
            assert(len(chain_data[head]) == 1);
            chain_data[head].front().yy ++;
        }
    }

    return 0;
}

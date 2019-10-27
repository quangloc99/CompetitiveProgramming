/**
 * Problem: https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=0&problem=981&mosmsg=Submission+received+with+ID+24111816
 * The idea is actually surprisngly simple, you just need to know it:
 * https://en.wikipedia.org/wiki/De_Bruijn_sequence
 * https://en.wikipedia.org/wiki/De_Bruijn_graph
 *
 * This problem asks us to construct De Bruijn sequence, and the answer is
 * the Hamilton cycle in the De Bruijn graph with n-dimension. But since the
 * De Bruijn graph with n-dimension is the "line graph" of the De Bruijn with
 * (n - 1)-dimension, we just need to find the Eulerian cycle of that graph
 * instead, and the problem is whole lot easier!!!
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()

#define maxn 23
struct edge {
    // bool *picked;  // well, we don't need this since we have directed graph
    int target;
    int cost;
    edge(int tar, int cst): target(tar), cost(cst) {}
};

vector<vector<edge>> build_de_bruijn_graph_2(int num_length) {
    int n = 1 << num_length;
    int mask = n - 1;

    vector<vector<edge>> gr(n);
    // notice the order, I want the smaller edge to be at the end.
    rep(i, n) for (int f = 2; f--; ) {
        gr[i].emplace_back((i << 1 | f) & mask, f);
    }
    return gr;
}

list<edge> get_euler_cycle(vector<vector<edge>> gr, int start_at) {
    function<list<edge>(int)> dfs = [&](int u) {
        // for simplicity I made the reversed list and reverse it back later
        list<edge> ans;
        while (gr[u].size()) {
            ans.push_front(gr[u].back());
            gr[u].pop_back();
            u = ans.front().target;
        }

        for (auto it = ans.begin(); it != ans.end(); ++it) {
            ans.splice(it, dfs(it->target));
        }
        return ans;
    };
    auto ans = dfs(start_at);
    reverse(ans.begin(), ans.end());
    return ans;
}

vector<int> ans[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest--) {
        int n, k; cin >> n >> k;
        if (ans[n].size() == 0) {
            auto s = get_euler_cycle(build_de_bruijn_graph_2(n - 1), 0);
            rep(i, n - 1) {
                s.emplace_front(-1, 0);
            }
            for (auto i: s) ans[n].push_back(i.cost);
            // for (auto i: s) clog << i.cost;
            // clog << endl;
        }
        
        int cur_ans = 0;
        rep(i, n) {
            cur_ans = cur_ans << 1 | ans[n][k + i];
        }
        cout << cur_ans << '\n';
    }

    return 0;
}

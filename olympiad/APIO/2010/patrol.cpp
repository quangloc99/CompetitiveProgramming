/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://dmoj.ca/problem/apio10p2
 * Editorial: http://apio-olympiad.org/2010/
 * This solution is "almost" the same as the editorial.
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
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

#define maxn 101010
int k, n;
vector<int> gr[maxn];

multiset<int, greater<int>> max_children_branches[maxn];

int get_top_branches(int u, int cnt) {
    int ans = 0;
    for (auto i: max_children_branches[u]) {
        if (cnt-- == 0) break;
        ans += i + 1;
    }
    return ans;
}

int ans = 0;
void upd_ans(int rev_ans) {
    int new_ans = 2 * (n - 1) + k - rev_ans;
    ans = min(ans, new_ans);
}

void up_dfs(int u, int p) {
    for (auto v: gr[u]) {
        if (v == p) continue;
        up_dfs(v, u);
        max_children_branches[u].insert(get_top_branches(v, 1));
        if (max_children_branches[u].size() > 4)
            max_children_branches[u].erase(--max_children_branches[u].end());
    }

    upd_ans(get_top_branches(u, 2));
}

int down_dfs(int u, int p, int max_parent_pair, int max_parent_branch) {
    upd_ans(max_parent_pair + get_top_branches(u, 2));
    if (u != p)
        max_children_branches[u].insert(max_parent_branch);
    upd_ans(get_top_branches(u, 4));
    int max_children_pair = 0;

    for (auto v: gr[u]) {
        if (v == p) continue;
        int v_branch = get_top_branches(v, 1);
        bool has = max_children_branches[u].count(v_branch);
        if (has) max_children_branches[u].erase(max_children_branches[u].find(v_branch));
        int cur_max_pair = max({get_top_branches(u, 2), max_parent_pair, max_children_pair});
        max_children_pair = max(down_dfs(v, u, cur_max_pair, get_top_branches(u, 1)), max_children_pair);
        if (has) max_children_branches[u].insert(v_branch);
    }

    if (u != p)
        max_children_branches[u].erase(max_children_branches[u].find(max_parent_branch));

    max_children_pair = max(max_children_pair, get_top_branches(u, 2));
    return max_children_pair;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> k;
    rep(i, n - 1) {
        int u, v; cin >> u >> v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }

    int root = 1;
    ans = 2 * (n - 1) + k;
    up_dfs(root, root);
    if (k == 2) down_dfs(root, root, 0, 0);
    cout << ans;

    return 0;
}

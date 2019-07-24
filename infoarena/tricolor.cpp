/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.infoarena.ro/problema/tricolor
 * Idea: it is just dynamic problem. I defined dp[u][i] = the maximum number of 
 * twinned pair if there are i nodes from the subtree rooted at u is painted
 * white that has no white ancestor.
 *
 * I did not define the global array, but I compute the array dp[u] on the fly
 * using cal_dp function (note that p is the parent of u, not the "i" mentioned
 * above).
 *
 * It seems like this is O(n^3) solution, since the combine function run in
 * O(a.size() * b.size()), but this solution is in fact O(n^2). I actually
 * wanted to write a formal proof, but then I think it is not neccesary. The
 * key for a proof is that: dp[u] will have length equals to the number
 * of leaves in the subtree rooted at u.
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

#define maxn 5010
int n;
vector<int> gr[maxn];

vector<llong> combine(vector<llong> const& a, vector<llong> const& b) {
    vector<llong> ans(len(a) + len(b) - 1);
    rep(i, len(a))
        rep(f, len(b)) {
            ans[i + f] = max(ans[i + f], a[i] + b[f] + 1ll * i * f);
        }
    return move(ans);
}

vector<llong> cal_dp(int u, int p) {
    vector<llong> ans(1, 0);
    llong sum_max_dp = 0;
    for (auto v: gr[u]) {
        if (v == p) continue;
        auto child_dp = cal_dp(v, u);
        llong max_dp = 0;
        rep(i, len(child_dp)) 
            max_dp = max(max_dp, i + child_dp[i]);
        sum_max_dp += max_dp;
        ans = move(combine(ans, child_dp));
    }
    if (len(ans) < 2) ans.push_back(0);
    ans[1] = max(ans[1], sum_max_dp);
    return move(ans);
}

int main(void) {
    // ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    ifstream cin("tricolor.in");
    ofstream cout("tricolor.out");
    int ntest; cin >> ntest;
    while (ntest --) {
        cin >> n;
        rep1(i, n) gr[i].clear();
        rep(i, n - 1) {
            int u, v; cin >> u >> v;
            gr[u].push_back(v);
            gr[v].push_back(u);
        }

        auto root_dp = cal_dp(1, 1);
        cout << *max_element(root_dp.begin(), root_dp.end()) << '\n';
    }

    return 0;
}

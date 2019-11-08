/**
 * Author: Tran Quang Loc 
 * Problem: https://oj.uz/problem/view/IZhO12_biochips
 * Idea:
 * Most of the solution did like dp[u][cnt] = maximum result that we picked cnt nodes in the subtree of u.
 * But here I did a little bit different.
 * Here I decompose the tree in euler-tour technique in the form: ...u, (nodes in sub tree of u), u, ...
 * Each node has 2 positions.  Then for each nodes I drops its first position but keeps where it was. 
 * Let's call it prev_pos[i], where i is the second position of node u.
 * Let's make a new array b. For each node, we put its cost in the array b at its second position.
 *
 * So the problems transformed into this: we need to pick m numbers in b, but when we pick the i-th number, 
 * we must not pick number at position between prev_pos[i] till i. This is actually easy and
 * we can do it in O(NM). Also note that the memory is not realy big, we need a bit optimization.
 *
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

#define maxn 200010
int n, m;
int a[maxn];
vector<int> gr[maxn];

vector<int> topo_a, prev_pos;
void dfs(int u) {
    int t = len(topo_a);
    for (auto v: gr[u]) {
        dfs(v);
    }
    topo_a.push_back(a[u]);
    prev_pos.push_back(t);
}

llong dp[2][maxn];
int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;

    rep1(i, n) {
        int p; cin >> p >> a[i];
        gr[p].push_back(i);
    }

    dfs(0);

    memset(dp[0], 0, sizeof dp[0]);

    rep1(i, m) {
        dp[i & 1][0] = INT_MIN;
        rep(u, n) {
            dp[i & 1][u + 1] = max(dp[i & 1][u], dp[~i & 1][prev_pos[u]] + topo_a[u]);
        }
    }

    cout << dp[m & 1][n];

    return 0;
}

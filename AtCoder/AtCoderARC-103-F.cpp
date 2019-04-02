/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://arc103.contest.atcoder.jp/editorial
 * This solution implemented the same idea as in the editorial.
 * Implementation note:
 * The final part, that check if the final tree coressponds to the D values given by the input, we can just check for D[root],
 * because during, the tree reconstruction process, all the D values are related to each other.
 * There is a ez way to check that no need to do bfs/dfs: we just need to sum up all the subtree size of all the other node beside the
 * root. This number will be the distance from root to all the other nodes, and can be prove ez-ly by little math transformation.
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

#define maxn 101010
int n;
int nchild[maxn];
map<llong, int> inv_d;

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep1(i, n) {
      llong d; cin >> d;
      inv_d[d] = i;
    }
    rep1(i, n) nchild[i] = 1;

    vector<pair<int, int>> ans;
    while (len(inv_d) > 1) {
      llong d = inv_d.rbegin()->xx;
      int u = inv_d.rbegin()->yy;
      inv_d.erase(--inv_d.end());
      llong td = d - 1ll * (n - 2 * nchild[u]);
      // clog << u << ' ' << d << ' ' << nchild[u] << endl;
      if (!inv_d.count(td)) {
        cout << "-1";
        return 0;
      }
      int v = inv_d[td];
      ans.push_back({u, v});
      // clog << u << ' ' << v << endl;
      nchild[v] += nchild[u];
    }
    llong droot = inv_d.begin()->xx;
    int root = inv_d.begin()->yy;
    llong s = 0;
    rep1(i, n) if (i != root) s += nchild[i];
    // clog << root << ' ' << s << endl;
    if (s != droot) {
      cout << -1;
      return 0;
    }
    rep(i, n - 1) cout << ans[i].xx << ' ' << ans[i].yy << '\n';

    return 0;
}

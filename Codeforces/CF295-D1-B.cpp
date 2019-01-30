/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial was in Russian, and it was very short (at least for this problem), so I won't include it here.
 * Idea: First things to do is processing the query in reversed orders.
 *       The main algorithm used here is Floyd with a little modification. When adding a new node, these following things must be computed:
 *       - the shortest path from all added nodes to the new nodes.
 *       - the shortest path from the new node to all the other added nodes.
 *       - the shortest path between 2 added nodes that goes through the new nodes.
 *       As we know, Floyd is basically DP, so after processing the distances between the fisrt i added nodes (or the last i removed node),
 *       we can use those infomation to compute those things above in O(n^2), so overall algorithm is O(n^3).
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long /*{{{*/
#define ii pair<int, int>
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
template<class x> vector<typename x::value_type> tovec(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> tovec(u a, u b) { return vector<ptrtype(u)>(a, b); }
template<typename x> ostream& operator<<(ostream& cout, const vector<x>& a) {
  cout << "[";
  if (len(a)) cout << a[0];
  rep1(i, len(a) - 1) cout << ", " << a[i];
  return cout << "]";
}
template<typename u, typename v> ostream& operator<<(ostream& cout, const pair<u, v>& a) {
  return cout << "(" << a.xx << ", " << a.yy << ")";
}/*}}}*/

#define maxn 555
int n;
llong d[maxn][maxn];
int remlist[maxn];
llong ans[maxn];
int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n;
  rep1(i, n) rep1(f, n) cin >> d[i][f];
  rep(i, n) cin >> remlist[i];

  llong curans = 0;
  for (int i = n; i--; ) {
    int mid = remlist[i];
    for (int f = i + 1; f <= n; ++f) {
      curans += d[mid][remlist[f]] + d[remlist[f]][mid];
    }

    for (int f = i + 1; f < n; ++f)
      for (int g = i + 1; g < n; ++g) {
        int u = remlist[f], v = remlist[g];
        
        // from u to mid that goes through v
        if (d[u][v] + d[v][mid] < d[u][mid]) {
          curans -= d[u][mid];
          d[u][mid] = d[u][v] + d[v][mid];
          curans += d[u][mid];
        }
        // from mid to u that goes through v
        if (d[mid][v] + d[v][u] < d[mid][u]) {
          curans -= d[mid][u];
          d[mid][u] = d[mid][v] + d[v][u];
          curans += d[mid][u];
        }
      }

    for (int f = i + 1; f < n; ++f)
      for (int g = i + 1; g < n; ++g) {
        int u = remlist[f], v = remlist[g];
        // from u to v that goes through mid
        if (d[u][mid] + d[mid][v] < d[u][v]) {
          curans -= d[u][v];
          d[u][v] = d[u][mid] + d[mid][v];
          curans += d[u][v];
        }
      }

    ans[i] = curans;
  }

  rep(i, n) cout << ans[i] << ' ';


  return 0;
}

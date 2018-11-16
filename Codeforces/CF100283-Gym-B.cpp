/**
 * Authors: Tran Quang Loc (darkkcyan)
 * Solution explaination: 
 *  We can see that the all the maximum edges from all the mini-max paths, form the MST of the original graph.
 *  The algorithm used here is Kruskal. When an edge joins 2 components, the result of the problem is updated as following:
 *  - The u and v be the size of the first and second component respectively. Let c be the cost of the edge.
 *  - Increase the result of the nodes in the first component by v * c. Do the same with the second component but by u * c.
 *
 *  In order to do that, a modified version of dsu is used as following:
 *  - When 2 node is join, a new "virtual" node is created to be their parents.
 *  - The parent will has some informations of its children: the number of the component it presents, the "updated value" for all the node
 *  in its component.
 *
 *  So when process each edge, the "updated value" is used, and after the Kruskal algorithm is completed, we lazily, dynamically compute the
 *  result for all nodes.
 *
 *  There will be O(n) virtual node, because the number of join operators is O(n). But I multiply by 4 for carefullness.
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

// Solution starts here.
#define maxn 101010
int n, m;
pair<int, ii> edge[maxn];
int dsu[maxn * 4];
ii child[maxn * 4];
int nodecnt;
llong ans[maxn * 4];
int childcnt[maxn * 4];

int findp(int u) { return dsu[u] = dsu[u] == u ? u : findp(dsu[u]); }
void join(int u, int v) {
  u = findp(u);
  v = findp(v);
  if (u == v) return ;
  ++nodecnt;
  ans[nodecnt] = 0;
  dsu[nodecnt] = nodecnt;
  dsu[u] = dsu[v] = nodecnt;
  child[nodecnt] = {u, v};
  childcnt[nodecnt] = childcnt[u] + childcnt[v];
}

int main(void) {
  freopen("road.in", "r", stdin);
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

  int t; cin >> t;
  rep1(testcase, t) {
    cin >> n >> m;
    rep1(i, n) {
      dsu[i] = i;
      child[i] = {-1, -1};
      ans[i] = 0;
      childcnt[i] = 1;
    }
    nodecnt = n;
    rep(i, m) cin >> edge[i].yy.xx >> edge[i].yy.yy >> edge[i].xx;
    sort(edge, edge + m);
    rep(i, m) {
      int u = findp(edge[i].yy.xx), v = findp(edge[i].yy.yy);
      if (u == v) continue;
      llong c = edge[i].xx;
      ans[u] += childcnt[v] * c;
      ans[v] += childcnt[u] * c;
      join(u, v);
    }

    for (int i = nodecnt; i >= 1; i--) {
      int u = child[i].xx, v = child[i].yy;
      if (u != -1) ans[u] += ans[i];
      if (v != -1) ans[v] += ans[i];
    }
    cout << "Case " << testcase << ":\n";
    rep1(i, n) cout << ans[i] << '\n';
  }


  return 0;
}

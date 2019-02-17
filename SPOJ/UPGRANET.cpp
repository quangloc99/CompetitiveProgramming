/**
 * Author: Tran Quang Loc
 * Idea: find the *maximum* spanning tree of the graph, and for each edge (u, v) with weigh c that not in the spanning tree,
 * find the minimum edge in that path between u, v on the tree. If the found edge has weigh x, then add x - c into the answer.
 *
 * Why maximum spanning tree works? Or we can say, why maximum spanning tree will guarantee the maximum capacity between each pair of verties?
 * Let's prove it by induction. We are now doing Kruskal but the edge list is sorted descending. What we are proving here is
 * at anypoint in our algorithm, each pairs of verties in the same component will have maximum capacity.
 * First step, we can always choose the edge with maximum weigh, so the capacity between those vertices connected by this edge is maximized.
 * And now, at some steop in Kruskal's algorithm, we are processing edge (u, v) with weigh c. There is 2 cases:
 * - u and v is currently in the same component. Because we are processing the edges from highest weigh to lowest, so all the edge in the
 *   component containing u and v is bigget than c, so adding this edge into our tree does not help maximizing the capacity.
 * - u and v is not in the same componet. That means c is maximum lowest cost between one vertex in the component containning u and the
 *   other in the component containning v. We cannot get a better cost because by this algorithm, those bigger edges don't connect this
 *   2 components.
 */
#include <bits/stdc++.h>
using namespace std;

#define cli clog << "\"" << __PRETTY_FUNCTION__ << "\"@" << __LINE__ << ": "/*{{{*/
#define llong long long 
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

#define maxn 101010
#define maxlg 18
int n, m;
vector<pair<llong, int>> gr[maxn];
vector<pair<llong, ii>> el, rest;

int dsu[maxn];
int findp(int u) { return dsu[u] = u == dsu[u] ? u : findp(dsu[u]); }
void join(int u, int v) {
  if (rand() & 1) swap(u, v);
  dsu[findp(u)] = findp(v);
}

int up[maxlg][maxn];
llong minE[maxlg][maxn];
int depth[maxn];
void dfs(int u) {
  rep(i, maxlg - 1) {
    up[i + 1][u] = up[i][up[i][u]];
    minE[i + 1][u] = min(minE[i][u], minE[i][up[i][u]]);
  }
  for (auto i: gr[u]) {
    int v = i.yy;
    if (v == up[0][u]) continue;
    depth[v] = depth[u] + 1;
    up[0][v] = u;
    minE[0][v] = i.xx;
    dfs(v);
  }
}

llong findMin(int u, int v) {
  llong ans = LLONG_MAX;
  if (depth[u] < depth[v]) swap(u, v);
  for (int d = depth[u] - depth[v], i = 0; (1 << i) <= d; ++i)
    if ((d >> i) & 1) {
      ans = min(ans, minE[i][u]);
      u = up[i][u];
    }
  if (u == v) return ans;
  for (int i = maxlg; i--; ) {
    if (up[i][u] == up[i][v]) continue;
    ans = min(ans, min(minE[i][u], minE[i][v]));
    u = up[i][u];
    v = up[i][v];
  }
  return min(ans, min(minE[0][u], minE[0][v]));
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  
  cin >> n >> m;
  el.resize(m);
  for (auto& i: el) cin >> i.yy.xx >> i.yy.yy >> i.xx;
  
  sort(el.rbegin(), el.rend());

  rep1(i, n) dsu[i] = i;
  
  for (auto i: el) {
    int u = i.yy.xx, v = i.yy.yy;
    if (findp(u) == findp(v)) rest.push_back(i);
    else {
      join(u, v);
      gr[u].push_back({i.xx, v});
      gr[v].push_back({i.xx, u});
    }
  }

  depth[1] = 0;
  up[0][1] = 1;
  minE[0][1] = LLONG_MAX;
  dfs(1);
  llong ans = 0;
  for (auto i: rest) {
    //clog << i << endl;
    ans += findMin(i.yy.xx, i.yy.yy) - i.xx;
  }

  cout << ans;

  return 0;
}

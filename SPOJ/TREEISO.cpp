// graph, tree, dp, hashing, diameter, center
/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: it's simple, just need to represent a tree in a unique way, and compare those representations.
 * First thing to do is to root the tree in an unique node. There are few possibilities: center or centroid. Unfortunatly, each tree
 * has at most 2 centers and also 2 centroids, so neigther way, we must find at most 2 representations.
 * With a center, we can dfs from it, and calculate the representation for each subtree. For fast but not less efficiency,
 * representation should be calculated with a hash function. After calculate the representations of all subtree, we need to combine them.
 * One way to do it is to sort thoes hashes, then combine them like computing a hash value for a string.
 * That's all.
 *
 * Note: a hash function should be good. My "old hash function made me fail 6 times, so I need to combine the old one with the
 * number of verties in the subtree.
 */
#include <bits/stdc++.h>
using namespace std;
 
#define cli clog << "\"" << __PRETTY_FUNCTION__ << "\"@" << __LINE__ << ": "
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
}
 
#define maxn 101010
#define rem ((llong)1e12+9939)
int n;
vector<int> gra[maxn], grb[maxn];
 
int bfs(int u, const vector<int>* gr, vector<int>& pre) {
  pre.assign(n + 1, -1);
  pre[u] = u;
  queue<int> qu;
  for (qu.push(u); len(qu); qu.pop()) {
    u = qu.front();
    for (auto v: gr[u]) {
      if (pre[v] != -1) continue;
      pre[v] = u;
      qu.push(v);
    }
  }
  return u;
}
 
ii findCen(const vector<int>* gr) {
  vector<int> pre;
  int u = bfs(1, gr, pre);
  int v = bfs(u, gr, pre);
  int l = 0;
  for (int t = v; t != u; v = pre[v], t = pre[pre[t]]) {
    ++l;
    if (pre[t] == u) break;
    ++l;
  }
  if (l & 1) 
    return {v, pre[v]};
  else return {v, -1};
}
 
unsigned llong rot_hash(const vector<unsigned llong>& v) {
  unsigned llong ans = 0;
  for (auto i: v) {
    ans = (ans * 33) + i;
  }
  return ans;
}
 
vector<unsigned llong> calHash(const vector<int>* gr) {
  ii cen = findCen(gr);
  clog << cen << endl;
  vector<unsigned llong> hs(n + 1, -1);
  vector<int> cnt(n + 1, -1);
 
  function<void(int, int)> dfs = [&](int u, int p) {
    if (hs[u] + 1 != 0) return;
    for (auto v: gr[u]) {
      if (v == p) continue;
      dfs(v, u);
    }
    vector<unsigned llong> childHashes;
    cnt[u] = 1;
    for (auto v: gr[u]) if (v != p) {
      cnt[u] += cnt[v];
      childHashes.push_back(hs[v] * 33 + cnt[v]);
    }
    sort(childHashes.begin(), childHashes.end());
    //clog << u << ' ' << childHashes << endl;
    childHashes.push_back(1);
    hs[u] = rot_hash(childHashes);
  };
 
  dfs(cen.xx, cen.xx);
  //clog << hs << endl;
  unsigned llong ans1 = hs[cen.xx];
  if (cen.yy == -1) return {ans1};
  hs[cen.xx] = hs[cen.yy] = -1;
  dfs(cen.yy, cen.yy);
  unsigned llong ans2 = hs[cen.yy];
  if (ans2 < ans1) swap(ans1, ans2);
  return {ans1, ans2};
}
 
int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n;
    rep1(i, n) {
      gra[i].clear();
      grb[i].clear();
    }
    rep(i, n - 1) {
      int u, v; cin >> u >> v;
      gra[u].push_back(v);
      gra[v].push_back(u);
    }
 
    rep(i, n - 1) {
      int u, v; cin >> u >> v;
      grb[u].push_back(v);
      grb[v].push_back(u);
    }
    auto x = calHash(gra), y = calHash(grb);
    //clog << x << ' ' << y << endl;
    cout << (x == y ? "YES" : "NO") << '\n';
  }
 
  return 0;
}

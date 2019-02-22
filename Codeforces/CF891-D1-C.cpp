/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/55841
 * Though the part that handle the queries in the editorial and in my solution are quite the same, but the rest is really different.
 * So sorry if this is a little too long!
 * Here is the full idea:
 * - It is easy to see that we should process edges by their cost. All edges with the same cost must be processed in the same time.
 * - The main idea here is quite the same as Dquery problem on SPOJ: split a query into events.
 *   In my solution, we split a query by the costs of the edges: if there is an edge with cost w in a query, that query will be assign 
 *   to the event when we are processing all edges with cost w.
 * - After group all the queries, we do the normal Kruskal. Surpose that we processed (joined) all the edges that smaller than w,
 *   and we are now processing the edges with cost w.
 *   + In the editorial, they used dfs/bfs with the graph, whose edges are all the edges with cost w, and verties are the joined verties. 
 *   + So here is my method: I modified my dsu code so it can have more operation:
 *      - mark where I want to save the data  (function clearCache)
 *      - rewind the saved data to the marked point (function rollBack)
 *     and with those operations, everything is ez. Instead of checking by dfs/bfs, first I use the mark operation, then
 *     I just do normal dsu like Krukal but only with the edges in the query, after that I rewind back to the mark point and continue
 *     doing the other query in the same way.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long /*{{{*/
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
#define _pt(x) typename iterator_traits<x>::value_type
template<class u> vector<_pt(u)> tovec(u a, u b) { return vector<_pt(u)>(a, b); }
template<class x> ostream& operator<<(ostream& cout, const vector<x>& a) {
  cout << "[";
  rep(i, len(a)) cout << a[i] << (i == len(a) - 1 ? "" : ", ");
  return cout << "]";
}
template<class u, class v> ostream& operator<<(ostream& cout, const pair<u, v>& a) {
  return cout << a.xx << ": " << a.yy;
}/*}}}*/

struct edge {
  int u, v, w;
  edge() {}
};

#define maxn 501010
int n, m, q;
edge edgeList[maxn];
vector<edge> groupedEdgeList[maxn];
vector<int> query[maxn];
vector<int> queryGroup[maxn];

int dsu[maxn], cacheDsu[maxn];
vector<int> changes;
void assignDsu(int u, int val) {
  if (cacheDsu[u] == -1) {
    changes.push_back(u);
    cacheDsu[u] = dsu[u];
  }
  dsu[u] = val;
}

int findp(int u) {
  if (dsu[u] != u) 
    assignDsu(u, findp(dsu[u]));
  return dsu[u];
}

void join(int u, int v) {
  if (rand() & 1) swap(u, v);
  assignDsu(findp(u), findp(v));
}

void clearCache() {
  for (auto i: changes) cacheDsu[i] = -1;
  changes.clear();
}

void rollBack() {
  for (auto i: changes)dsu[i] = cacheDsu[i];
  clearCache();
}

bool ans[maxn];
int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n >> m;
  rep1(i, m) {
    cin >> edgeList[i].u >> edgeList[i].v >> edgeList[i].w;
    groupedEdgeList[edgeList[i].w].push_back(edgeList[i]);
  }

  cin >> q;
  rep(i, q) {
    int k; cin >> k;
    query[i].resize(k);
    for (auto& f: query[i]) {
      cin >> f;
      int cost = edgeList[f].w;
      if (!len(queryGroup[cost]) or queryGroup[cost].back() != i)
        queryGroup[cost].push_back(i);
    }
    sort(all(query[i]), [](int u, int v) {
        return edgeList[u].w > edgeList[v].w;
      });
    ans[i] = 1;
  }

  rep1(i, n) {
    dsu[i] = i;
    cacheDsu[i] = -1;
  }

  rep(cost, maxn) {
    for (auto f: queryGroup[cost]) {
      for (; ans[f] == 1 and len(query[f]) and edgeList[query[f].back()].w == cost; query[f].pop_back()) {
        edge& e = edgeList[query[f].back()];
        if (findp(e.u) == findp(e.v))
          ans[f] = 0;
        else join(e.u, e.v);
      }
      rollBack();
    }

    for (auto e: groupedEdgeList[cost])
      join(e.u, e.v);
    clearCache();
  }
  
  rep(i, q) cout << (ans[i] ? "YES" : "NO") << '\n';
  return 0;
}

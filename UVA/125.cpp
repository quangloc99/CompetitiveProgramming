/**
 * Author: Tran Quang Loc
 * Idea:
 * - What if the graph is DAG? This subproblem can be solved using dp. Let's dp[u][v] = number of way to go from u to v.
 *   The formula is dp[u][v] = sum of all dp[i][v]  for each adjacency vertex i of u.
 * - To make the graph a DAG, I join all node of a SCC into 1 node (vertex contraction). The formula above can be modify a little:
 *   First just do the above like formula. If dp[i][v] = inf then dp[u][v] will also be inf.
 *   And then, if dp[u][v] != 0 and dp[u][v] != inf, and also u is in an SCC, then dp[u][v] = inf. (if there is a way from u to v, but 
 *   u is in an SCC, then there is inf way from u to v).
 * - To make it simpler to code, just use the node that represent the whole SCC. This can be done using dsu, or just assign that node
 *   when finding the SCC.
 * - To do the dp we somehow need to do topology sort. The good thing is, when doing Kosaraju's algorithm to find SCC, the produced stack
 *   is `semi` topology sorted, and in this code I used that stack.
 * - This solution work in O(n^3). But with the problem constraints, we can see it is O(max(deg(u)) * n^2) = O(30 * n^2).
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

int n, m;
vector<vector<int>> gr, rgr;

vector<int> st;
vector<bool> vis;
void dfs(int u) {
  vis[u] = 1;
  for (auto v: gr[u]) {
    if (!vis[v]) dfs(v);
  }
  st.push_back(u);
}

vector<int> comp;
vector<bool> isSCC;
int revdfs(int u, int compNum) {
  vis[u] = 1;
  comp[u] = compNum;
  int ans = 1;
  for (auto v: rgr[u]) {
    if (!vis[v]) ans += revdfs(v, compNum);
  }
  return ans;
}

vector<vector<int>> ans;

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int testNum = 0;
  while (cin >> m) {
    cout << "matrix for city " << testNum ++ << '\n';
    gr.clear(); rgr.clear();
    n = 0;
    while (m--) {
      int u, v; cin >> u >> v;
      n = max(n, max(u, v) + 1);
      gr.resize(n);
      rgr.resize(n);
      gr[u].push_back(v);
      rgr[v].push_back(u);
    }

    vis.assign(n, 0);
    st.clear();
    rep(i, n) if (!vis[i]) dfs(i);
    assert(len(st) == n);

    vis.assign(n, 0);
    comp.assign(n, 0);
    isSCC.assign(n, 0);
    for (int i = n; i--; ) {
      int u = st[i];
      if (vis[u]) continue;
      if (revdfs(u, u) > 1) {
        isSCC[u] = 1;
      }
    }

    //clog << comp << ' ' << st << endl;

    rep(i, n) if (i != comp[i]) {
      int u = comp[i];
      for (auto t: gr[i]) {
        if (comp[t] == u) continue;
        gr[u].push_back(t);
      }
    }
    ans.assign(n, vector<int>(n, 0));

    rep(i, n) {
      int u = st[i], pu = comp[u];
      if (u != pu) continue;
      if (isSCC[pu]) ans[u][u] = -1;
      else ans[u][u] = 1;

      for (int f = i + 1; f < n; ++f) {
        int v = st[f], pv = comp[v];
        if (v != pv) continue;
        ans[v][u] = 0;
        for (auto _c: gr[v]) {
          int c = comp[_c];
          if (ans[c][u] == -1) {
            ans[v][u] = -1;
            break;
          }
          ans[v][u] += ans[c][u];
        }

        if (isSCC[v] and ans[v][u] > 0) {
          ans[v][u] = -1;
          continue;
        }
      }

      if (ans[u][u] == 1) ans[u][u] = 0;
    }

    rep(i, n) {
      rep(f, n) cout << ans[comp[i]][comp[f]] << (" \n")[f == n - 1];
    }
  }


  return 0;
}

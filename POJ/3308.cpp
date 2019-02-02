/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea:
 *   This one must be solve using max flow/min cuts algorithm. In this code I used Edmond Karp's algorithm because the contrait is not big.
 *   We just need a little changes. We should transform the cost given in the input into the log form, that is, from c into e^(c') (c' = log(c))
 *   So instead of multiply c, we add c', then get the real result back using exp function.
 *   The graph here can be build like the following:
 *   - For each row r with cost c', connect the source to r with cost c'.
 *   - For each column c with cost c', connect c (or numerically, c+m) to the sink with cost c'.
 *   - For each coordinates of paratroop (r, c), connect r to c (or numerically, c + m) with cost +infinity.
 */

//#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <cstring>
#include <climits>
#include <cmath>
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

#define maxn 200
#define eps 1e-8
#define inf (LLONG_MAX)
int vcnt;
long double gr[maxn][maxn];
int source, sink;
int pre[maxn];

bool bfs() {
  queue<int> qu;
  memset(pre, -1, sizeof(pre[0]) * vcnt);
  pre[source] = source;
  for (qu.push(source); len(qu); qu.pop()) {
    int u = qu.front();
    if (u == sink) return true;
    for (int v = 0; v < vcnt; ++v) {
      if (gr[u][v] < eps) continue;
      if (pre[v] != -1) continue;
      pre[v] = u;
      qu.push(v);
    }
  }
  return false;
}

long double maxf;
void incflow(int u) {
  if (pre[u] == u) return;
  maxf = min(maxf, gr[pre[u]][u]);
  incflow(pre[u]);
  //clog << pre[u] << ' ';
  gr[pre[u]][u] -= maxf;
  gr[u][pre[u]] += maxf;
}

long double maxflow() {
  long double ans = 0;
  while (bfs()) {
    maxf = inf;
    incflow(sink);
    //clog << sink << ": " << maxf << endl;
    ans += maxf;
  }
  return ans;
}

int n, m, l;
int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

  int t; cin >> t;
  while (t--) {
    cin >> n >> m >> l;                         // I missed the roles of n and m, but it is not important.
    vcnt = n + m + 2;
    source = vcnt - 1;
    sink = vcnt - 2;
    rep(i, vcnt) rep(f, vcnt) gr[i][f] = 0;
    rep(i, n) {
      long double c; cin >> c;
      gr[source][i] = log(c);
    }
    rep(i, m) {
      long double c; cin >> c;
      gr[i + n][sink] = log(c);
    }
    
    rep(i, l) {
      int u, v; cin >> u >> v;
      --u, --v;
      gr[u][v + n] = inf;
    }
    cout << fixed << setprecision(4) << exp(maxflow()) << '\n';
  }

  return 0;
}

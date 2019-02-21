// graph, scc, 2sat
/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: using 2sat. The sat equation can be built as following:
 * - Each person will be assign to 0 or 1. 1 means they will be on the groom side, and 0 if they will on the other side.
 *   I will use `xh` and `xw` to donate the value will be assigned to x-th husband and x-th wife respectively.
 * - 0-th husband (the groom) must be assign to 1, and we got 1 clause: `0h` = (`0h` or `0h).
 *   So we can build an edge from (not `0h`) to `0h`
 * - u-th wife and u-th husband must not be in the same side, so we got n clauses:
 *      (`uw` xor `uh`) = (`uw` or `uh`) and (not `uh` or not `uw`)
 * - For each of m conditions given in the input with format (u, v), we got 1 clause: not (u and v) = (not u or not v)
 * So that the building-graph part, it is the hardest part of the graph. Then we can check for 2sat normally using scc.
 * Finally, first we should print all node that assigned to 1 in the component that contains `0h` first, then for each other
 * component, we print all node that assigned to 1. Also make sure that when node u is processed then node (not u) will not be processed.
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

#define term(x) ((x) * 2)
#define neg(x) ((x) ^ 1)
#define maxn 200

int n, m;
vector<int> gr[maxn], rgr[maxn];

int toterm(string s) {
  stringstream ss; ss << s.substr(0, len(s) - 1);
  int u; ss >> u;
  return u * 2 + (s[len(s) - 1] == 'w');
}

void addOrTerm(int u, int v) {
  //clog << neg(u) << ' ' << v << endl;
  //clog << neg(v) << ' ' << u << endl;
  gr[neg(u)].push_back(v);
  gr[neg(v)].push_back(u);
  rgr[u].push_back(neg(v));
  rgr[v].push_back(neg(u));
}

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
void revdfs(int u, int c) {
  comp[u] = c;
  for (auto v: rgr[u]) {
    if (comp[v] == -1) revdfs(v, c);
  }
}

void printAns(int u) {
  if (u & 1 and u / 4 != 0) {
    cout << (u / 4) << ("hw")[u / 2 % 2] << ' ';
  }
  vis[u] = 1;
  vis[neg(u)] = 1;
  for (auto v: gr[u])
    if (!vis[v]) printAns(v);
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  while (cin >> n >> m) {
    if (n == 0 and m == 0) break;

    //clog << "TEST " << endl;
    rep(i, 4 * n) {
      gr[i].clear();
      rgr[i].clear();
    }

    gr[neg(term(0))].push_back(term(0));
    rgr[term(0)].push_back(neg(term(0)));

    rep(i, n) {
      addOrTerm(term(i * 2), term(i * 2 + 1));
      addOrTerm(neg(term(i * 2)), neg(term(i * 2 + 1)));
    }
    while (m--) {
      string u, v; cin >> u >> v;
      addOrTerm(neg(term(toterm(u))), neg(term(toterm(v))));
    }

    st.clear();
    vis.assign(4 * n, 0);
    rep(i, 4 * n) if (!vis[i]) dfs(i);
    
    comp.assign(4 * n, -1);
    for (;len(st); st.pop_back()) {
      int u = st.back();
      if (comp[u] != -1) continue;
      revdfs(u, u);
    }
    //clog << comp << endl;

    bool ans = 1;
    rep(i, 2 * n) if (comp[term(i)] == comp[neg(term(i))]) {
      //clog << term(i) << endl;
      ans = 0;
      break;
    }
    if (ans == 0) {
      cout << "bad luck\n";
      continue;
    }
    vis.assign(4 * n, 0);
    printAns(term(0));
    rep(i, 4 * n)
      if (!vis[i]) printAns(i);
    cout << '\n';
  }

  return 0;
}

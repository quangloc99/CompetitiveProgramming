/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea:
 *   First: check if the graph is a SCC. This part is ez.
 *   Second: find all biconnected components of this graph (on undirectional graph). Then check if each components is a simple cycle or not.
 * In my code I use Tarjan's algorithm for both parts.
 * 
 * Important note: beware of this case: the graph has 2 edges: u->v and v->u. This make our undirectional graph a multigraph.
 * The implementaion found online does not handle this case, and I spent a little long time to found it!.
 *
 * Also, I use "throw" inorder to quit recursion faster, but that does not ensure all the *stack in my codes are empty, so
 * for each test they must be cleared.
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
  return cout << "(" << a.xx << ": " << a.yy << ")";
}/*}}}*/

int n, m;
vector<vector<int>> digr;
vector<vector<pair<int, bool>>> gr;

vector<int> num, low;
int counter;
void resetCounter() {
  num.assign(n, -1); low = num;
  counter = 0;
}

vector<int> vertexStack;
vector<bool> inStack;
void resetForSCC() {
  vertexStack.clear();
  inStack.assign(n, 0);
}
void checkSCC(int u) {
  num[u] = low[u] = counter ++;
  vertexStack.push_back(u);
  inStack[u] = 1;
  for (auto v: digr[u]) {
    if (num[v] == -1) {
      checkSCC(v);
      low[u] = min(low[u], low[v]);
    } else if (inStack[v]) {
      low[u] = min(low[u], num[v]);
    }
  }
  if (num[u] == low[u]) {
    int cnt = 0;
    for (int v = -1; len(vertexStack) and v != u; vertexStack.pop_back()) {
      v = vertexStack.back();
      ++cnt;
      inStack[v] = 0;
    }
    if (cnt != n) throw "NO";
  }
}

vector<pair<int, int>> edgeStack;
vector<bool> orientStack;
vector<int> inCount, outCount;
void resetForBiconnectedComponent() {
  edgeStack.clear();
  orientStack.clear();
  inCount.assign(n, 0);
  outCount.assign(n, 0);
}
void popBiconnectedComponent(const pair<int, int>& last = {-2, -2}) {
  vector<int> vertexList;
  for (pair<int, int> edge(-1, -1);
      len(edgeStack) and edge != last;
      edgeStack.pop_back(), orientStack.pop_back()
  ) {
    edge = edgeStack.back();
    if (!orientStack.back()) swap(edge.xx, edge.yy);
    outCount[edge.xx]++;
    inCount[edge.yy]++;
    vertexList.push_back(edge.xx);
    vertexList.push_back(edge.yy);
    if (!orientStack.back()) swap(edge.xx, edge.yy);
  }
  
  for (auto i: vertexList)
    if (inCount[i] != 1 or outCount[i] != 1) throw "NO";
  for (auto i: vertexList) inCount[i] = outCount[i] = 0;
}

void findBiconnectedComponent(int u, int p) {
  num[u] = low[u] = counter++;
  int childCnt = 0;
  for (auto i: gr[u]) {
    int v = i.xx;
    if (num[v] == -1) {
      edgeStack.push_back({u, v});
      orientStack.push_back(i.yy);
      findBiconnectedComponent(v, u);
      childCnt++;
      if (p != u and low[v] >= num[u])
        popBiconnectedComponent({u, v});
      if (p == u and childCnt > 1) 
        popBiconnectedComponent({u, v});
      low[u] = min(low[u], low[v]); 
    } else if ((v != p or i.yy) and num[v] < low[u]) {
      low[u] = num[v];
      edgeStack.push_back({u, v});
      orientStack.push_back(i.yy);
    }
  }
  if (p != u) return;
  popBiconnectedComponent();
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n >> m;
    gr.assign(n, vector<pair<int, bool>>());
    digr.assign(n, vector<int>());
    rep(i, m) {
      int u, v; cin >> u >> v;
      gr[u].push_back({v, 1});
      gr[v].push_back({u, 0});
      digr[u].push_back(v);
    }
    rep(i, n) sort(all(gr[i]), [](pair<int, bool> u, pair<int, bool> v) { return u.yy > v.yy; });
    
    try {
      resetCounter();
      resetForSCC();
      checkSCC(0);
      resetCounter();
      resetForBiconnectedComponent();
      findBiconnectedComponent(0, 0);
      throw "YES";
    } catch (const char* ans) {
      cout << ans << '\n';
    }
  }

  return 0;
}

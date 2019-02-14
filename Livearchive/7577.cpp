/**
 * Author: Tran Quang Loc
 * My clerification: I think the courior just start at some branch, then it delivers
 * the messege to the other branch, and next it automatically jumps to another branch without costs.
 *
 * Editorial: http://www.csc.kth.se/~austrin/icpc/finals2016solutions.pdf
 * The solution in the editorial has really cool observation, but I did not 
 * see it, so I used more generally way: using dp with divide and conquer optimization
 * Idea:
 *   Let's call a[i] = the total shortest length go from the headquater to i and the go back to i.
 *   Let's call num[i] = the number of branches in the group that contains i.
 *   Then the answer is: (python code)
 *     sum([a[i] * (num[i] - 1) for i in range(b)])
 *   Notice that the branch with high a[i] should be in the groups with less branches.
 *   so the idea is we can greedily sort the array a so we can group the branches with high a[i]
 *   together. (it is easy to prove why sorting works).
 *   Now the dp part with D&C optimization. The cost function is:
 *     c(i, j) = sum([a[f] for f in range(i, j)]) * (j - i - 1)
 *   and obviously this function is convex, so the optimization works.
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

#define maxn 50101
int n, b, s, m;
vector<ii> gr[maxn];
vector<ii> rgr[maxn];

vector<int> sssp(int source, const vector<ii>* adj) {
  priority_queue<ii, vector<ii>, greater<ii>> pq;
  vector<int> dis(n + 1, INT_MAX);
  dis[source] = 0;
  for (pq.push({0, source}); len(pq); ) {
    int u = pq.top().yy;
    int d = pq.top().xx;
    pq.pop();
    if (d > dis[u]) continue;
    for (auto i: adj[u]) {
      int v = i.xx;
      int nd = d + i.yy;
      if (nd >= dis[v]) continue;
      dis[v] = nd;
      pq.push({nd, v});
    }
  }
  return dis;
}

int a[maxn];
llong sum[maxn];
llong calC(int l, int r) {
  return (sum[r] - sum[l]) * (llong)(r - l - 1);
}

vector<llong> predp, curdp;

void dAndCdp(int l, int r, int opl, int opr) {
  if (l > r) return ;
  int mid = (l + r) /2;
  pair<llong, int> bst = {LLONG_MAX, -1};
  for (int i = opl; i <= min(opr, mid); ++i) {
    if (predp[i] == LLONG_MAX) continue;
    pair<llong, int> cur = {predp[i] + calC(i, mid), i};
    bst = min(bst, cur);
  }

  curdp[mid] = bst.xx;
  int op = bst.yy;
  dAndCdp(l, mid - 1, opl, op);
  dAndCdp(mid + 1, r, op, opr);
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  while (cin >> n >> b >> s >> m) {
    rep1(i, n) {
      gr[i].clear();
      rgr[i].clear();
    }
    rep(i, m) {
      int u, v, l; cin >> u >> v >> l;
      gr[u].push_back({v, l});
      rgr[v].push_back({u, l});
    }
  
    {
      vector<int> dis;
      dis = sssp(b + 1, gr);
      rep1(i, b) a[i - 1] = dis[i];
      dis = sssp(b + 1, rgr);
      rep1(i, b) a[i - 1] += dis[i];
    }

    sort(a, a + b);
    //cli << tovec(a, a + b) << endl;
    sum[0] = 0;
    rep(i, b) sum[i + 1] = sum[i] + a[i];
    predp.assign(b + 1, LLONG_MAX);
    curdp.assign(b + 1, 0);
    predp[0] = 0;
    rep(i, s) {
      dAndCdp(0, b, 0, b);
      //cli << curdp << endl;
      swap(curdp, predp);
    }
    cout << predp[b] << '\n';
  }

  return 0;
}

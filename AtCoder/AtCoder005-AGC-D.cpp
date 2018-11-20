// Author: Tran Quang Loc (darkkcyan)
// Editorial: https://agc005.contest.atcoder.jp/editorial
// 
// About the part "counting the matching with size k" in the editorial
// it was not very clear to me, so I note here for myself.
// 
// As in the editorial say, the graph is a disjoint union of paths.
// Or, I will call it "chain of paths", because each component can be
// stretch into a line. Ok, so in a chain, I cannot pick 2 consecutive
// edge, because each node only belong to 1 matching. In order to find
// the answer with dp, I connect all the chain into one chain, and then 
// mark every end of chain. That way, we doing dp, when considering picking
// an edge, if the previous edge is end of a chain, then these 2 edges did
// not in a same chain, and we do it in normal way (see the code). Otherwise, we have to
// skip the previous edge. That's all.
#include <bits/stdc++.h>
using namespace std;

#define llong long long/*{{{*/
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

#define rem 924844033
#define maxn 2020

int n, k;
bool endOfChain[maxn * 2] = {0};
llong dp[maxn * 2][maxn] = {0};

vector<llong> dfac(1, 1);
llong fac(int i) {
  while (len(dfac) <= i) dfac.push_back((dfac.back() * len(dfac)) % rem);
  return dfac[i];
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n >> k;

  int l = k * 2;

  int pos = 0;
  endOfChain[pos] = 1;
  rep1(i, min(l, n)) {
    if (i - k < 1 and i + k > n) continue;
    int chainLength = (n - i) / l + 1;
    int last = (chainLength - 1) * l + i;
    int nEdge = chainLength * 2;
    if (i - k < 1) --nEdge;
    if (last + k > n) --nEdge;
    //clog << i << ' ' << last << ' ' << chainLength << ' ' << nEdge << endl;
    pos += nEdge;
    endOfChain[pos] = 1;
  }

  //clog << tovec(endOfChain, endOfChain + pos + 1) << endl;

  dp[0][0] = 1;
  rep1(i, pos) {
    int back = i - 1 - !endOfChain[i - 1];
    // other way to write:
    // if (endOfChain[i - 1]) back = i - 1;
    // else back = i - 2;
    rep(f, n + 1) {
      dp[i][f] = dp[i - 1][f];                // not pick the current edge
      if (f == 0) continue;
      if (back < 0) continue;
      (dp[i][f] += dp[back][f - 1]) %= rem;  // pick the current edge
    }
    //clog << i << ' ' << tovec(dp[i], dp[i] + n + 1) << endl;
  }

  llong* m = dp[pos];                          // the array M in the editorial
  llong ans = 0;
  int sign = 1;
  fac(10);
  //clog << dfac << endl;
  rep(i, n + 1) {
    (ans += sign * m[i] * fac(n - i)) %= rem;
    if (ans < 0) ans += rem;
    //clog << i << ' ' << ans << ' ' << m[i] << endl;
    sign = -sign;
  }
  cout << ans;

  return 0;
}

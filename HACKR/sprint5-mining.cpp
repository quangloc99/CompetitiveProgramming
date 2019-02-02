/**
 * Author: Tran Quang Loc (darkkcyan)
 * The problem setter solves this problem using dp with D&C optimization. I solved it using convex-hull trick.
 * Idea: If we let a mine be a pick up place, then some mines on its left and on its right will move to this mine.
 *       Or we can say, some segments of mines will move to the left and some will move to the right. 
 *       It is easy to see that they are alternatively lying between each other. The first segment will move to the right,
 *       and the second segment will move to the left, and the next one will move to the right... and the last one will move to the left.
 *       So my idea is to divide all mines into 2 * k segments. The odd segments will go to the right and the even ones will go to the left.
 *       From here we can use dp with 2 states: dp[i][j] is the answer of the of the first j mines that required i pick up places.
 *       Because we have 2 kind of segments (move to left and move to right), we need 2 different formulas (in Python, because it is easier to read)
 *       - Move to the right: dp[i][j] = min([dp[i - 1][g] + sum([a[h] * (x[j - 1] - x[h]) for h in range(g + 1, f)]) for g in range(f)])
 *       - Move to the left:  dp[i][j] = min([dp[i - 1][g] + sum([a[h] * (x[h] - x[g]) for h in range(g, f)]) for g in range(1, f + 1)])
 *       Note that x - the coordinates and a - the amounts of gold - are 0-indexed.
 *       Also note that the "move to the left" segments will begin at the last element of "move to the left", because the that element
 *       will be the pickup place.
 *       From those formulas we can transform them into the line forms. It was a little complicated - they will have diffenrent cooficients
 *       and the variables, so read the code plz.
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

struct line {
  llong k, b;
  line() {}
  line(llong ak, llong ab): k(ak), b(ab) {}
  llong operator()(llong x) { return k * x + b; }
};

ostream& operator<<(ostream& cout, const line& a) {
  return cout << make_pair(a.k, a.b);
}

bool check(const line& l1, const line& l2, const line& t) {
  return ((long double)t.b - l2.b) * ((long double)l1.k - l2.k) <= ((long double)t.k - l2.k) * ((long double)l1.b - l2.b);
}

#define inf ((llong)1e17)
#define maxn 5010
int n, k;
llong a[maxn], x[maxn];
llong sa[maxn], sax[maxn];

llong dp[2][maxn];

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n >> k;
  rep(i, n) cin >> x[i] >> a[i];
  sa[0] = sax[0] = 0;
  rep(i, n) {
    sa[i + 1] = sa[i] + a[i];
    sax[i + 1] = sax[i] + a[i] * x[i];
  }
  
  //cli << tovec(sa, sa + n + 1) << endl;
  //cli << tovec(sax, sax + n + 1) << endl;

  dp[1][0] = 0;
  rep1(i, n) dp[1][i] = inf;
  rep(i, 2 * k) {
    clog << endl;
    bool s = i & 1;
    dp[s][0] = inf;
    deque<line> qu;
    if (!s) 
      qu.push_back(line(0, dp[!s][0]));
    rep1(f, n) {
      line nl = s
        ? line(-x[f - 1], dp[!s][f] - sax[f - 1] + x[f - 1] * sa[f - 1])
        : line(-sa[f], dp[!s][f] + sax[f])
      ;
      while (len(qu) > 1 and check(qu[len(qu) - 2], qu.back(), nl))
        qu.pop_back();
      qu.push_back(nl);

      llong arg = s ? sa[f] : x[f - 1];
      while (len(qu) > 1 and qu[0](arg) >= qu[1](arg)) qu.pop_front();
      //cli << tovec(qu) << endl;
      if (s) 
        dp[s][f] = qu[0](arg) + sax[f];
      else
        dp[s][f] = qu[0](arg) + sa[f] * x[f - 1] - sax[f];
      //cli << tovec(qu) << endl;
    }
    //cli << tovec(dp[s], dp[s] + n + 1) << endl;
  }
  cout << dp[1][n];

  return 0;
}

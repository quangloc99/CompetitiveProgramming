/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: http://contest.usaco.org/TESTDATA/MAR08.acquire.htm
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long /*{{{*/
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
  llong operator()(llong x) { return x * k + b; }
};

ostream& operator << (ostream& cout, const line& l) {
  return cout << make_pair(l.k, l.b);
}

bool check(const line& l1, const line& l2, const line& l3) {
  return (l3.b - l2.b) * (l1.k - l2.k) - (l3.k - l2.k) * (l1.b - l2.b) <= 0;
}

#define maxn 1010101
int n;
pair<llong, llong> a[maxn];
llong dp[maxn];

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n;
  rep(i, n) cin >> a[i].xx >> a[i].yy;
  sort(a, a + n);
  int f = 0;
  rep(i, n) {
    while (f > 0 and a[f - 1].yy <= a[i].yy) --f;
    a[f++] = a[i];
  }
  n = f;
  //clog << tovec(a, a + n) << endl;
  deque<line> hull;

  hull.push_front(line(a[0].yy, 0));
  rep1(i, n) {
    //clog << i << endl;
    llong arg = a[i - 1].xx;
    while (len(hull) > 1 and hull[0](arg) >= hull[1](arg))
      hull.pop_front();
    dp[i] = (llong)hull.front()(arg);
    //clog << tovec(hull) << ' ' << hull.front()(arg) << endl;
    line nl(a[i].yy, dp[i]);
    while (len(hull) > 1 and check(hull[len(hull) - 2], hull.back(), nl)) {
      hull.pop_back();
    }
    hull.push_back(nl);
    //clog << tovec(hull) << endl;
  }
  //clog << tovec(dp, dp + n) << endl;
  cout << dp[n];

  return 0;
}

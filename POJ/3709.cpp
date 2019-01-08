/**
 * author: Tran Quang Loc (darkkcyan)
 * Guide: http://h0rnet.hatenablog.com/entry/2016/02/06/155748
 * This first problem I solved using convex-hull trick. I find that the formula to check if a line is removable is hard to
 * remember, so I changed it and used geometry. But still, I think I need to remember that formula, or find a way to remember it.
 */
#include <vector>
#include <queue>
#include <cstdio>
#include <climits>
#include <iostream>
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

#define maxn 500100
#define inf (INT_MAX)
int n, k;
int a[maxn];

struct line {
  llong a, b;
  line(llong _a, llong _b): a(_a), b(_b) {}
  llong operator()(llong x) const { return a * x + b; }
};

long double intersectX(const line& u, const line& v) {
  //clog << u.a << ' ' << u.b << ' ' << v.a << ' ' <<v.b << endl;
  return (u.b - v.b) / (v.a - u.a);
}


llong dp[maxn];
llong s[maxn];
llong process() {
  rep1(i, n) s[i] = s[i - 1] + a[i];
  deque<line> qu;

  dp[0] = 0;
  rep1(i, n) {
    if (i < k) {
      dp[i] = inf;
      continue;
    }
    line nl(-a[i - k + 1], dp[i - k] - s[i - k] + (llong)a[i - k + 1] * (i - k));
    //clog << i << ' ' << nl.a << ' ' << nl.b << endl;
    while (len(qu) > 1) {
      if (qu.back().a == nl.a) {
        if (qu.back().b < nl.b) break;
      } else if (intersectX(nl, qu[len(qu) - 2]) > intersectX(qu.back(), qu[len(qu) - 2])) break;
      qu.pop_back();
    }
    if (len(qu) and qu.back().a == nl.a) {
      qu.back().b = min(qu.back().b, nl.b);
    } else qu.push_back(nl);

    while (len(qu) > 1 and qu[0](i) > qu[1](i)) {
      qu.pop_front();
    }

    dp[i] = s[i] + qu.front()(i);
  }
  //clog << tovec(dp, dp + 1 + n) << endl;
  return dp[n];
}

int main(void) {
  int t; scanf("%d", &t);
  a[0] = 0;
  while (t--) {
    cin >> n >> k;
    rep1(i, n) cin >> a[i];
    printf("%lld\n", process());
  }

  return 0;
}

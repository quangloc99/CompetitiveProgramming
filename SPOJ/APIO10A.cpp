/**
 * Author: Tran Quang Loc (darkkcyan)
 * Note: I followed this tutorial: http://h0rnet.hatenablog.com/entry/2016/02/06/155748, but I find that the formula to check
 * if the last is removable is hard to remember, so I investigated myself. Finally, this formula is just imply that if the 
 * intersection point of the last and pre-last line of the hull is above the new one (if this is upper hull, for lower hull it is under),
 * then it can be remove.
 * To implement this, I used Homogeneous coordinates, because of it easiness, and no floating point computations.
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

template<typename T>
struct p3 {
  T a, b, c;
  p3() {}
  p3(T _a, T _b, T _c) : a(_a), b(_b), c(_c) {}

  p3 cross(const p3& other) {
    return p3(
        b * other.c - c * other.b,
        c * other.a - a * other.c,
        a * other.b - b * other.a
    );
  }

  T dot(const p3& other) {
    return a * other.a + b * other.b + c * other.c;
  }

  T getY(T x) {
    return (a * x + c) / -b;
  }
};

#define sgn(x) ((x) < 0 ? -1 : (x) > 0)
#define line p3<llong>
#define point p3<llong>
#define makeline(k, b) p3<llong>(k, -1, b)
#define makepoint(x, y) p3<llong>(x, y, 1)
#define intersect(a, b) a.cross(b)
#define pointIsUnderLine(line, point) (point.dot(line) <= 0)

#define maxn 1000100
int n;
llong a, b, c;
llong x[maxn];
llong s[maxn];
llong dp[maxn];

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n >> a >> b >> c;
    rep(i, n) cin >> x[i];

    s[0] = 0;
    rep(i, n) s[i + 1] = s[i] + x[i];

    deque<line> hull;
    hull.push_back(makeline(0, 0));
    rep1(i, n) {
      while (len(hull) > 1 and hull[0].getY(s[i]) <= hull[1].getY(s[i])) {
        //clog << hull[0].getY(s[i]) << ' ' << hull[1].getY(s[i]) << endl;
        hull.pop_front();
      }
      //clog << "[";
      //for (auto f: hull) {
        //clog << f.a << ':' << f.c << ' ';
      //}
      //clog << "]" << endl;
      dp[i] = hull.front().getY(s[i]) + a * s[i] * s[i] + b * s[i] + c;
      line nl = makeline(- 2 * a * s[i], dp[i] + a * s[i] * s[i] - b * s[i]);
      //clog << i << ' ' << s[i] <<' ' << dp[i] << ' ' << nl.a << ' ' << nl.c << endl;
      while (len(hull) > 1 and pointIsUnderLine(intersect(hull.back(), hull[len(hull) - 2]), nl))
        hull.pop_back();
      hull.push_back(nl);
    }
    //clog << tovec(dp + 1, dp + n + 1) << endl;
    cout << dp[n] << '\n';
  }


  return 0;
}

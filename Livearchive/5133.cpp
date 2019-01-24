/**
 * Author: Tran Quang Loc (darkkcyan)
 * Solution sketches (problem F): http://www.csc.kth.se/~austrin/icpc/finals2011solutions.pdf 
 * We can solve this one using convex-hull trick but with a self balanced binary search tree (set/multiset).
 * Using that data struct, we are able to find a line with a smaller slope in the set for a given line, add or remove a line in O(logn)
 * So overall we got O(nlogn) solution.
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
  line(llong argk, llong argb): k(argk), b(argb) {}

  llong operator()(llong x)const { return k * x + b; }
};

ostream& operator<<(ostream& cout, const line& x) {
  return cout << make_pair(x.k, x.b);
}

bool operator<(const line& u, const line& v) {
  if (u.k == v.k) return u.b < v.b;
  return u.k < v.k;
}

bool check(const line& l1, const line& l2, const line& t) {
  return (t.b - l2.b) * (l2.k - l1.k) >= (t.k - l2.k) * (l2.b - l1.b);
}

struct mach {
  llong d, p, r, g;
};

#define maxn 101010
int n, d;
llong c;
mach a[maxn];

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  for (int testnum = 1;; ++testnum) {
    cin >> n >> c >> d;
    if (n == 0 and c == 0 and d == 0) break;
    rep(i, n) cin >> a[i].d >> a[i].p >> a[i].r >> a[i].g;
    sort(a, a + n, [](const mach& u, const mach& v) { return u.d < v.d; });
    multiset<line> hull;
    hull.insert(line(0, c));
    
    auto clearTopHull = [&](llong day) {
      while (len(hull) > 1) {
        auto u = hull.begin(), v = u;
        ++v;
        if ((*u)(day) < (*v)(day)) hull.erase(u);
        else break;
      }
    };

    rep(i, n) {
      //clog << i << endl;
      clearTopHull(a[i].d);
      //clog << tovec(hull) << endl;
      llong maxPrice = (*hull.begin())(a[i].d);
      if (maxPrice < a[i].p) continue;
      line nl(
          a[i].g,
          maxPrice - a[i].p + a[i].r - a[i].g * (a[i].d + 1)
      );

      //clog << nl << endl;
      
      bool canBeAdd = 1;
      while (len(hull) > 1) {
        auto u = hull.upper_bound(nl);
        if (u != hull.end()) {
          if ((*u)(a[i].d) > nl(a[i].d)) {
            canBeAdd = false;
            break;
          }
        }
        if (u == hull.begin()) break;
        if (--u == hull.begin()) break;
        //clog << *u << endl;
        auto v = u; -- v;
        if (check(*v, *u, nl)) hull.erase(u);
        else break;
        //clog << tovec(hull) << endl;
      }
      if (canBeAdd) hull.insert(nl);
      //clog << tovec(hull) << endl;
    }
    clearTopHull(d + 1);
    assert(len(hull));
    cout << "Case " << testnum <<  ": " << (*hull.begin())(d + 1) << '\n';
  }

  return 0;
}

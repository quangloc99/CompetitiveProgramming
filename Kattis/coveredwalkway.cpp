/**
 * Author: Tran Quang Loc (darkkcyan)
 * Some info about this problem: https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Kattis/kattis-coveredwalkway.txt
 * This one is very similar to APIO 10-Commando. The most important part is how to get the line function, that is, open the bracket and
 * we can see that the line's angle coefficient is `- 2 * y` (when we are query x), everything else is constant and not depends on each other.
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
  line () {}
  line(llong ak, llong ab) : k(ak), b(ab) {}
  llong cal(llong x) const { return k * x + b; }
};

ostream& operator<<(ostream& cout, const line& a) {
  return cout << make_pair(a.k, a.b);
}

bool check(const line& l1, const line& l2, const line& l3) {
  return (l3.k - l2.k) * (l1.b - l2.b) >= (l3.b - l2.b) * (l1.k - l2.k);
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

  int n; llong c; cin >> n >> c;
  llong prelast, last; cin >> last;
  deque<line> hull;
  hull.push_back(line(- 2 * last, c + last * last));
  rep(i, n - 1) {
    prelast = last;
    cin >> last;
    while (len(hull) > 1 and hull[0].cal(prelast) >= hull[1].cal(prelast))
      hull.pop_front();
    llong minLocalAns = hull[0].cal(prelast) + prelast * prelast;
    line nl(- 2 * last, minLocalAns + c + last * last);

    while (len(hull) > 1 and check(hull[len(hull) - 2], hull.back(), nl))
      hull.pop_back();
    hull.push_back(nl);
    //clog << i + 2 << ' ' << minLocalAns << ' ' <<  tovec(hull) << endl;
  }
  while (len(hull) > 1 and hull[0].cal(last) >= hull[1].cal(last))
    hull.pop_front();
  llong ans = hull[0].cal(last) + last * last;
  cout << ans;

  return 0;
}

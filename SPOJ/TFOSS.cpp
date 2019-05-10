/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: it is ez to know that 2 points that we are finding must lie on convex hull, so the fisrt step is to find the convex hull.
 * The algorithm I used here is monotune chain. Next, we can see that if we are fixing 1 point, and find the other point
 * (the furthest point from the fixed point), we can see that the distance function of the remain points is convex, so
 * we can either: ternary search, or binary search (since the point set is discrete), or using 2 pointer technique. 
 * Here I used 2 pointer technique.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()

#define point pair<llong, llong>
#define sqr(x) ((x) * (x))
#define sgn(x) ((x) < 0 ? -1 : (x) > 0)

point operator- (const point& a, const point& b) {
  return {a.xx - b.xx, a.yy - b.yy};
}

llong dis2(const point& a, const point& b) {
  return sqr(a.xx - b.xx) + sqr(a.yy - b.yy);
}

llong cross(const point& a, const point& b) {
  return a.xx * b.yy - a.yy * b.xx;
}

vector<point> find_hull(const vector<point>& a) {
  vector<point> ans;
  for (auto p: a) {
    while (len(ans) >= 2 and cross(ans.back() - p, ans[len(ans) - 2] - p) <= 0)
      ans.pop_back();
    ans.push_back(p);
  }
  return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest -- ) {
      int n; cin >> n;
      vector<point> a(n);
      rep(i, n) cin >> a[i].xx >> a[i].yy;

      sort(a.begin(), a.end());
      auto hull = find_hull(a);
      sort(a.rbegin(), a.rend());
      auto lower_hull = find_hull(a);
      hull.pop_back();
      lower_hull.pop_back();
      hull.insert(hull.end(), all(lower_hull));

      // for (auto i: hull) clog << i.xx << ' ' << i.yy << endl;

      llong ans = 0;
      int f = 0;
      rep(i, len(hull)) {
        while (1) {
          int t = (f + 1) % len(hull);
          if (dis2(hull[i], hull[f]) > dis2(hull[i], hull[t])) break;
          f = t;
        }
        ans = max(ans, dis2(hull[i], hull[f]));
      }
      cout << ans << '\n';
    }

    return 0;
}

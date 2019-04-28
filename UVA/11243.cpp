#include <bits/stdc++.h>
using namespace std;

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea:
 * It is not too hard to see that we can do binary search the square's side. The only thing now is to check if a answer is right or not.
 * It is also not hard to see that the square must go though 3 points, and 2 of them must lie on opposite side.
 *
 * So what I did here is to bruteforce that 2 points. Since we know the square's side, we can find out the direction/slope of the
 * side that go through these points. After finding the slope, I check if all the points lie inside these 2 lines and 
 * if there projections to 1 of 2 lines have the range less than the square side. If these 2 conditions are corrects, then the 
 * side is satified.
 */
#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
#define sqr(x) ((x) * (x))

#define point pair<long double, long double>

#define eps 1e-8

point operator+ (point a, const point& b) {
  a.xx += b.xx; a.yy += b.yy; return a;
}

point operator- (point a, const point& b) {
  a.xx -= b.xx; a.yy -= b.yy; return a;
}

point operator- (const point& a) { return {-a.xx, -a.yy}; }

point operator* (point a, long double coef) {
  a.xx *= coef; a.yy *= coef; return a;
}

point operator/ (point a, long double coef) {
  a.xx /= coef; a.yy /= coef; return a;
}

long double length(const point& a) {
  return sqrt(sqr(a.xx) + sqr(a.yy));
}

// dot product
long double operator* (const point& a, const point& b) {
  return a.xx * b.xx + a.yy * b.yy;
}

// cross product'z 
long double cross(const point& a, const point& b) {
  return a.xx * b.yy - a.yy * b.xx;
}

point rotate(const point& a, const long double& ang) {
  const long double sina = sin(ang), cosa = cos(ang);
  return {
    a.xx * cosa - a.yy * sina,
    a.xx * sina + a.yy * cosa
  };
}

long double dis2(const point& a, const point& b) {
  return sqr(a.xx - b.xx) + sqr(a.yy - b.yy);
}

inline
long double dis(const point& a, const point& b) {
  return sqrt(dis2(a, b));
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest --) {
      int n; cin >> n;
      vector<point> a(n);
      rep(i, n) cin >> a[i].xx >> a[i].yy;
      if (n <= 1) {
        cout << "0.0\n";
        continue;
      }

      auto check = [&](long double side) -> bool {
        for (auto& u: a) {
          rep(i, n) { 
            auto v = a[i];
            if (u == v) continue;
            long double length_uv = dis(u, v);
            if (length_uv < side - eps) {
              v = (v - u) / length_uv * side + u;
              length_uv = side;
            }
            long double ang = asin(side / length_uv);

            for (auto signed_ang: {ang, -ang}) {
              point dir = rotate(v - u, signed_ang);
              dir = dir;
              bool all_inside = all_of(all(a), [&](const point& p) {
                  if (p == u || p == v) return true;
                  if (cross(dir, p - u) * cross(dir, v - u) < 0) return false;
                  if (cross(dir, p - v) * cross(dir, u - v) < 0) return false;
                  return true;
              });
              if (!all_inside) continue;
              long double max_proj = INT_MIN, min_proj = INT_MAX;
              for (auto& p: a) {
                long double proj = (p - u) * dir;
                max_proj = max(max_proj, proj);
                min_proj = min(min_proj, proj);
              }
              if (max_proj - min_proj < side * length(dir)) {
                return true;
              }
            }
          }
        }
        return false;
      };

      long double l = 0, r = 2000;
      rep(bs_time, 100) {
        long double mid = (l + r) / 2;
        if (check(mid)) r = mid;
        else l = mid;
      }

      cout << fixed << setprecision(2) << l * l << '\n';

    }

    return 0;
}

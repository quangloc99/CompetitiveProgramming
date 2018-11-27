/*
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: let the answer be the original polygon. For each edge on the polyon, 
 * get the intersection between the current answer and the half of the plane, separeted with
 * the current edge, that contains the inner side of the polygon.
 * My geometry code looks weird? I used homogeneous coordinates, so I can find:
 * - The intersections of 2 lines
 * - The line that go through 2 points
 * with the same code ;)
 */
#include <vector>
#include <cstdio>
#include <iostream>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <cmath>
using namespace std;

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

#define ld double
#define eps 1e-8
struct p3 {
  ld x, y, z;
  p3(): z(1) {}
  p3(ld _x, ld _y, ld _z): x(_x), y(_y), z(_z) { }
  p3 stan() const {
    if (abs(z) < eps) return *this;
    return p3(x / z, y / z, 1); 
  }
};

ostream& operator<<(ostream& cout, const p3& a) {
  return cout << "(" << a.x << ", " << a.y << ", " << a.z << ")";
}

p3 operator - (p3 a, p3 b) {
  a = a.stan(); b = b.stan();
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

p3 operator * (const p3& a, const p3& b) {
  return p3(
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x
  );
}

ld operator & (const p3& a, const p3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector<p3> cut(const vector<p3>& poly, const p3& a, const p3& b) {
  vector<p3> ans;
  int pre = len(poly) - 1;
  p3 ab = b * a;
  ld c2 = ab & poly[pre];
  for (int i = 0; i < len(poly); pre = i++) {
    ld c1 = ab & poly[i];
    if (c1 * c2 < -eps) ans.push_back(((poly[i] * poly[pre]) * ab).stan());
    if (c1 > -eps) ans.push_back(poly[i]);
    c2 = c1;
  }
  return ans;
}

ld area(const vector<p3>& a) {
  ld ans = 0;
  for (int pre = len(a) - 1, i = 0; i < len(a); pre = i ++) {
    ans += (a[pre].stan() * a[i].stan()).z;
  }
  return ans / 2;
}

vector<p3> process(vector<p3> a) {
  if (area(a) > 0) reverse(a.begin(), a.end());
  vector<p3> ans;
  ans = a;
  for (int pre = len(a) - 1, i = 0; i < len(a); pre = i++) {
    ans = cut(ans, a[pre], a[i]);
  }
  return ans;
}

int main(void) {
  int test; (void)scanf("%d", &test);
  while (test--) {
    int n; scanf("%d", &n);
    vector<p3> inp(n);
    rep(i, len(inp)) scanf("%lf%lf", &inp[i].x, &inp[i].y);
    printf("%.2lf\n", fabs(area(process(inp))));
  }

  return 0;
}

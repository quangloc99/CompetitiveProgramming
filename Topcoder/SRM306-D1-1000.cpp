/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://www.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm306
 */
#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <fstream>

using namespace std;
#define llong long long 

llong rem;
struct matrix : public vector<vector<llong>> {
  matrix() : vector<vector<llong>>() {}
  matrix(int n, int m) : vector<vector<llong>>(n, vector<llong>(m)) {
  }

  int nrow() const { return (int)size(); }
  int ncol() const { return (int)at(0).size(); }

  static matrix iden(int n) {
    matrix ans(n, n);
    for (int i = n; i--; ) ans[i][i] = 1;
    return ans;
  }

  friend matrix operator* (const matrix& a, const matrix& b) {
    assert(a.ncol() == b.nrow());
    matrix ans(a.nrow(), b.ncol());
    for (int i = a.nrow(); i--; )
      for (int f = b.ncol(); f--; )
        for (int g = a.ncol(); g--; )
          (ans[i][f] += a[i][g] * b[g][f]) %= rem;
    return ans;
  }

  matrix pow(int times) {
    assert(nrow() == ncol());
    matrix ans = matrix::iden(nrow());
    matrix base = *this;
    for (; times > 0; times >>= 1) {
      if (times & 1) ans = ans * base;
      base = base * base;
    }
    return ans;
  }
};

class TourCounting {
    public:
    int countTours(vector<string> g, int k, int m) {
      rem = (llong)m;
      int n = (int)g.size();
      matrix coef(n + 1, n + 1);
      coef[n][n] = 1;
      for (int i = n; i--; )
        for (int f = n; f--; )
          coef[f][i] = g[i][f] == 'Y' ? 1 : 0;

      llong ans = 0;
      for (int i = n; i--; ) {
        matrix tcoef = coef;
        tcoef.back()[i] = 1;
        matrix init_vals;
        init_vals.resize(n + 1, vector<llong>(1));
        init_vals[i][0] = 1;
        llong cur_ans = ((tcoef.pow(k) * init_vals).back()[0] - 1 + rem) % rem;  // -1 because there is not such cycle like (0) or (1) (only 1 elements).
        (ans += cur_ans) %= rem;
        // clog << i << ' ' << cur_ans << endl;
      }
      return (int)ans;
    }
};

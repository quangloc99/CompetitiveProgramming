/**
 * Author: Tran Quang Loc (darkkcyan)
 * The main idea is just dp with matrix multiplication optimization. This solution and the editorial shared the same formula, but
 * the coefficient is slightly different.
 * The editorial started from A filled with 100 first dp states, but in my solution I started from dp[0] = 1. That is:
 * A = (1, 0, 0, 0, 0, ...., 0). The last element is the sum of all dp and initialized with 0, and all other 0 elements is for dp[-1],
 * dp[-2], .. dp[-99].
 * And here is my B matrix: 
 * cnt[1]   cnt[2]   cnt[3]   ....   cnt[100]   0     // this row is for new dp state
 * 1        0        0        ....   0          0
 * 0        1        0        ....   0          0
 * 0        0        1        ....   0          0
 * ..............................................
 * 0        0        0        ....   1          0
 * 1        0        0        ....   0          1     // this row is for the result: new_result = old_result + the_previous_new_dp_state.
 * And the final matrix is B^(x + 1) * A, and I think this is more general than the editorial.
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

#define rem ((llong)1e9+7)

struct matrix : public vector<vector<llong>>{
  matrix(int n, int m) {
    resize(n, vector<llong>(m));
  }

  inline int nrow() const { return (int)size(); }
  inline int ncol() const { return len(at(0)); }

  static matrix iden(int n) {
    matrix ans(n, n);
    rep(i, n) ans[i][i] = 1;
    return ans;
  }

  matrix operator* (const matrix& other) {
    assert(ncol() == other.nrow());
    matrix ans(nrow(), other.ncol());
    rep(i, nrow())
    rep(f, other.ncol())
    rep(g, ncol()) 
      (ans[i][f] += (at(i)[g] * other[g][f]) % rem) %= rem;
    return ans;
  }

  matrix pow(llong n) {
    assert(nrow() == ncol());
    matrix ans = matrix::iden(nrow());
    matrix base = *this;
    for (; n > 0; n >>= 1) {
      if (n & 1) ans = ans * base;
      base = base * base;
    }
    return ans;
  }
};

int n;
llong x;

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> x;
    matrix coef(102, 102);
    rep1(i, 99) coef[i][i - 1] = 1;
    coef[101][101] = 1;
    coef[101][0] = 1;
    rep(i, n) {
      int u; cin >> u;
      coef[0][u - 1] ++;
    }

    // rep(i, 10) {
      // rep(f, 10) clog << coef[i][f] << ' ';
      // clog << endl;
    // }

    matrix base(102, 1);
    base[0][0] = 1;

    base = coef.pow(x + 1) * base;

    // rep1(i, x + 1) {
      // clog << i << ' ';
      // base = coef * base;
      // clog << (base)[101][0] << endl;
    // }

    cout << base[101][0];

    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Solution idea:
 *   Assume that we can go right only. The "go left" part can be done similarly and we just get the max.
 *   Here the formula: 
 *       dp[i][f] = max(dp[i - 1][j] + sum_a[i][f] - sum_a[i][j]) = sum_a[i][f] + max(dp[i - ][j] - sum_a[i][j]) for all j: sum_l[i][f] - sum_l[i][j] <= k
 *  where sum_a[i][f] = ap[i][0] + a[i][1] + ... + a[i][f - 1]  (first f numbers of the i-th row), sum_l is similarly defined.
 * So with this formula with just compute all dp for each row. To compute the following row, we use deque.
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

#define maxn 111
#define maxm 10100
int n, m;
llong k;
llong a[maxn][maxm];
llong l[maxn][maxm];
llong sa[maxn][maxm];  // sum_a
llong sl[maxn][maxm];  // sum_l
llong dp[maxn][maxm];

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  memset(dp, 0, sizeof(dp[0]));
  memset(sa, 0, sizeof(sa[0]));
  memset(sl, 0, sizeof(sl[0]));
  do {
    cin >> n >> m >> k;
    if (n == 0 and m == 0 and k == 0) break;
    //if (!firstTime) cout << '\n';
    //firstTime = false;
    ++n;
    //clog << n << ' ' << m << ' ' << k << endl;
    rep1(i, n) rep(f, m) cin >> a[i][f];
    rep1(i, n) rep(f, m) cin >> l[i][f];

    rep1(i, n) {
      sa[i][0] = 0;
      sl[i][0] = 0;
      rep(f, m) {
        sa[i][f + 1] = sa[i][f] + a[i][f];
        sl[i][f + 1] = sl[i][f] + l[i][f];
      }
      //clog << i + 1 << ' ' << tovec(sa[i + 1], sa[i + 1] + 1 + m) << endl;
      //clog << i + 1 << ' ' << tovec(sl[i + 1], sl[i + 1] + 1 + m) << endl;
    }

    rep1(i, n) {
      deque<pair<llong, llong>> qu;

      rep(f, m + 1) {
        pair<llong, llong> nv(dp[i - 1][f] - sa[i][f], sl[i][f]);
        while (len(qu) and qu.back().xx < nv.xx) qu.pop_back();
        qu.push_back(nv);
        while (len(qu) and sl[i][f] - qu.front().yy > k) qu.pop_front();
        dp[i][f] = qu.front().xx + sa[i][f];
      }
      //clog << i << ' ' << tovec(dp[i] + 1, dp[i] + m + 1) << endl;
      
      qu = deque<pair<llong, llong>>();

      for (int f = m + 1; f--; ) {
        pair<llong, llong> nv(dp[i - 1][f] + sa[i][f], sl[i][f]);
        while (len(qu) and qu.back().xx < nv.xx) qu.pop_back();
        qu.push_back(nv);
        while (len(qu) and qu.front().yy - sl[i][f] > k) qu.pop_front();
        //clog << tovec(qu) << endl;
        dp[i][f] = max(dp[i][f], qu.front().xx - sa[i][f]);
      }
      //clog << i << ' ' << tovec(dp[i] + 1, dp[i] + m + 1) << endl;
    }

    llong ans = 0;
    rep(i, m + 1) ans = max(ans, dp[n][i]);
    cout << ans << '\n';

  }while (1);


  return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * The main idea is to use dp with knuth's optimization. Let's call dp[i][j] is the minimum cost to build the prefix tree from
 * character i-th to j-th + 1 (or [i, j) segment). It is easy to see that
 *    dp[i][j] = min([dp[i][k] + dp[k][j] for k in range(i + 1, j)]) + sum(a[i:j])  # python code
 * and inorder to use knuth optimization, the following must be satisfied:
 * - sum(a[x:t]) + sum(a[y:z]) <= sum(a[x:z]) + sum(a[y:t])
 * - sum(a[y:z]) <= sum([x:t])
 * for all x <= y <= z <= t, and it was really obvious because a[i] > 0 for all i.
 */
#include <bits/stdc++.h>
using namespace std;

#define cli clog << "\"" << __PRETTY_FUNCTION__ << "\"@" << __LINE__ << ": "
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

#define maxn 2020
#define maxlg 11
#define inf ((llong)1e15)
int n;
llong a[maxn];

llong s[maxn];
int opt[maxn][maxn];

llong dp[maxn][maxn];
llong process(int l, int r) {
  if (l >= r) return inf;
  if (dp[l][r] != -1) return dp[l][r];
  //cli << l << ' ' << r << endl;
  if (l == r - 1) {
    opt[l][r] = l;
    return dp[l][r] = s[r] - s[l];
  }
  process(l, r - 1);
  process(l + 1, r);
  dp[l][r] = inf;
  for (int i = opt[l][r - 1]; i <= opt[l + 1][r]; ++i) {
    llong cur = process(l, i) + process(i, r);
    if (cur < dp[l][r]) {
      opt[l][r] = i;
      dp[l][r] = cur;
    }
  }
  dp[l][r] += s[r] - s[l];
  return dp[l][r];
}

string curans;
void print(int l, int r) {
  //clog << l << ' ' << r << ' ' << opt[l][r] << ' ' << curans << endl;
  if (l == r - 1) {
    cout << curans << '\n';
    return ;
  }
  curans.push_back('0');
  print(l, opt[l][r]);
  curans[len(curans) - 1] = '1';
  print(opt[l][r], r);
  curans.pop_back();
}

int main(void) {
  freopen("codes.in", "r", stdin);
  freopen("codes.out", "w", stdout);
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n;
  rep(i, n) cin >> a[i];
  s[0] = 0;
  rep(i, n) s[i + 1] = s[i] + a[i];
  memset(dp, -1, sizeof(dp));
  process(0, n);
  //rep(i, n + 1) clog << i << ' ' << tovec(opt[i], opt[i] + n + 1) << ' ' << tovec(dp[i], dp[i] + n + 1) << endl;
  print(0, n);

  return 0;
}

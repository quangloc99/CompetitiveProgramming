// dp, game theory
/**
 * Author: Tran Quang Loc
 * Offical editorial: https://contest.usaco.org/TESTDATA/NOV09.xoinc.htm
 * Idea: because the sum of all the coin is fixed, so instead of maximize the value, the player can maximize their score difference
 *       (even if it negative).
 *       in this code, cal(pos, pre) is the maximum difference score if the current player is playing when the stack has coins from
 *       C[pos] to C[n] and `pre` is the number of coin the the previous player pick. The formula is simple:
 *         cal(i, f) = max(c[i] + c[i + 1] + ... + c[i + g - 1] - cal(i + g, g) for 1 <= g <= 2 * f
 *       but because of the overlapping between cal(i, f) and cal(i, f - 1), so the formula can be transformed into:
 *         cal(i, f) = max(cal(i, f - 1), (max(c[i] + c[i + 1] + ... + c[i + g - 1] - cal(i + g, g) for 2 * f - 1 <= g <= 2 * f))
 *       and by doing so we got O(n^2) solution.
 */
#include <bits/stdc++.h>
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
 
#define maxn 5050
int n;
int a[maxn];
llong s[maxn];
 
llong dp[maxn][maxn];
 
llong get(int pos, int length) {
  return s[min(n, pos + length)] - s[max(pos, 0)];
}
 
llong cal(int pos, int pre) {
  if (pre <= 0) return INT_MIN;
  if (dp[pos][pre] != -1) return dp[pos][pre];
  if (pos > n) return 0;
  dp[pos][pre] = cal(pos, pre - 1);
  for (int i = 2 * pre - 1; i <= 2 * pre; ++i) {
    dp[pos][pre] = max(dp[pos][pre], get(pos - 1, i) - cal(pos + i, i));
  }
  return dp[pos][pre];
}
 
int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n;
  rep1(i, n) cin >> a[i];
  s[0] = 0;
  rep(i, n) s[i + 1] = s[i] + a[i + 1];
  
  memset(dp, -1, sizeof(dp));
  cout << (s[n] + cal(1, 1)) / 2;

 
  return 0;
}
 

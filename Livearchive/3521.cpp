/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/22712
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

llong n, k;

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    while (cin >> n >> k) {
      llong ans = max(n - k, 0LL) * k;
      n = min(n, k);
      llong i;
      for (i = 1; i * i <= k and i <= n; ++i) {
        ans += k % i;
      }

      while (i <= n) {
        llong next_i = min(k / (k / i) + 1, n + 1);
        llong cnt = next_i - i;
        llong sum = cnt * (next_i - 1 + i) / 2 * (k / i);
        ans += cnt * k - sum;
        i = next_i;
      }
      cout << ans << '\n';
    }

    return 0;
}

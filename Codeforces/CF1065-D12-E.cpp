/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/62411
 * Well this problems involed maths and this solution implimented exactly the same idea as in the editorials, so I think writing the 
 * idea here is not necessary and it will be ugly :).
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

#define rem ((llong)998244353)
#define maxn 201010
llong n, m, lenA;
llong b[maxn];

llong mpow(llong base, llong d) {
  llong ans = 1;
  for (; d > 0; d >>= 1) {
    if (d & 1) (ans *= base) %= rem;
    (base *= base) %= rem;
  }
  return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m >> lenA;
    b[0] = 0;
    rep1(i, m) cin >> b[i];
    ++m;

    llong r2 = mpow(2, rem - 2);
    llong prev = 1;
    rep1(i, m - 1) {
      llong cur = mpow(lenA, b[i] - b[i - 1]);
      (cur = cur * (cur + 1) % rem * r2) %= rem;
      (cur *= prev) %= rem;
      prev = cur;
    }
    llong ans = (prev * mpow(lenA, n - 2 * b[m - 1])) % rem;
    cout << ans;

    return 0;
}

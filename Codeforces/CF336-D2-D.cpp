/**
 * NOTE: I missunderstood the role of `n` and `m`, so in this code, their roles were swapped.
 * Author: Tran Quang Loc (darkkcyan)
 * This solution used different idea than the editorial.
 * Idea:
 * From the statement we can have O(n^2) solution: let's call dp[n][m][g] be the result of the problem with parameters n, m, g respectively.
 * (Still note that I missed understood the roles).
 * So: dp[n][m][0] = dp[n][m - 1][0] + dp[n - 1][m][1] + dp[n][m - 1][1]   and    dp[n][m][1] = dp[n][m - 1][1]
 * And hence: dp[n][m][0] = dp[n][m - 1][0] + dp[n - 1][m - 1][0] + dp[n][m - 2][0]
 * So we can solve for g = 0 only.
 * After generating some answer (with bruteforce, not the above fomula), I saw that dp[n][m][0] = dp[n - 1][m][0] + dp[n][m - 1][0]
 * for big n and m. And that was nice, because it turns out be a Pascal triangle. So for small n and m, I pregenerated them
 * (with some laws and O(n^2) dp fomulas). After that I used the above observation to calculate the final result.
 * For clearer understanding, see the code and also see the data I generated below the codes.
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

#define maxn 301010
#define rem ((llong)1e9+7)

llong mpow(llong b, llong ex) {
  llong ans = 1;
  for (; ex > 0; ex >>= 1) {
    if (ex & 1) (ans *= b) %= rem;
    (b *= b) %= rem;
  }
  return ans;
}

llong fac[maxn], rfac[maxn];
llong comb(int n, int k) {
  return fac[n] * rfac[k] % rem * rfac[n - k] % rem;
}

int g;
int n, m;  /// note: n and m's role are swapped.

vector<vector<llong>> dp;

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    fac[0] = rfac[0] = 1;
    rep1(i, maxn - 1) {
      fac[i] = (fac[i - 1] * i) % rem;
      rfac[i] = mpow(fac[i], rem - 2);
    }

    cin >> m >> n >> g;
    if (g == 1) {
      if (m == 0) {
        cout << (n == 1);
        exit(0);
      }
      --m;
      g = 0;
    }

    dp.resize(max(n + 1, 4));
    dp[0].resize(m + 1);
    dp[1].resize(m + 1);
    dp[2].resize(m + 1);
    for (int i = 3; i < len(dp); ++i) dp[i].resize(min(3, m + 1));

    // formulas times!!!
    rep1(i, m) dp[0][i] = i & 1;
    dp[1][0] = 0;
    rep1(i, m) dp[1][i] = (i + 1) / 2 + (i & 1);

    for (int i = 2; i < len(dp); ++i) {
      dp[i][0] = 1;
      dp[i][1] = i;
      for (int f = 2; f < len(dp[i]); ++f) {
        if (i == 2) dp[i][f] = (dp[i][f - 1] + dp[i - 1][f - 1] + dp[i - 2][f]) % rem;
        else dp[i][f] = (dp[i - 1][f] + dp[i][f - 1]) % rem;
      }
    }

    if (m < len(dp[n])) {
      cout << dp[n][m] << '\n';
      exit(0);
    }

    llong ans = 0;
    for (int f = 3; f <= m; ++f) {
      llong times = comb(n + m - f - 2 - 1, n - 3);
      // clog << 2 << ' ' << f << ' ' << times << ' ' << dp[2][f] << endl;
      (ans += times * dp[2][f]) %= rem;
    }

    for (int i = 3; i <= n; ++i) {
      llong times = comb(n + m - i - 3, m - 3);
      // clog << i << ' ' << 2 << ' ' << times << ' ' << dp[i][2] << endl;
      (ans += times * dp[i][2]) %= rem;
    }
    cout << ans;

    return 0;
}

/*
Still, m and n was swapped.
| n\m | 0 | 1 |  2 |  3  |  4  |   5  |   6  |   7  |   8   |   9   |
|----:|:-:|:-:|:--:|:---:|:---:|:----:|:----:|:----:|:-----:|:-----:|
|   0 |   | 1 |  0 |  1  |  0  |   1  |   0  |   1  |   0   |   1   |
|   1 | 0 | 2 |  1 |  3  |  2  |   4  |   3  |   5  |   4   |   6   |
|   2 | 1 | 2 |  4 |  6  |  9  |  12  |  16  |  20  |   25  |   30  |
|   3 | 1 | 3 |  7 |  13 |  22 |  34  |  50  |  70  |   95  |  125  |
|   4 | 1 | 4 | 11 |  24 |  46 |  80  |  130 |  200 |  295  |  420  |
|   5 | 1 | 5 | 16 |  40 |  86 |  166 |  296 |  496 |  791  |  1211 |
|   6 | 1 | 6 | 22 |  62 | 148 |  314 |  610 | 1106 |  1897 |  3108 |
|   7 | 1 | 7 | 29 |  91 | 239 |  553 | 1163 | 2269 |  4166 |  7274 |
|   8 | 1 | 8 | 37 | 128 | 367 |  920 | 2083 | 4352 |  8518 | 15792 |
|   9 | 1 | 9 | 46 | 174 | 541 | 1461 | 3544 | 7896 | 16414 | 32206 |
*/

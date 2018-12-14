/** 
 * @Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://www.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm431 
 * My dp state is a little different, has 5 dimension
 *  dp[2][20][10][10][2]
 * The first param indicated the current processing digit.
 * I may define its dimension up to 1000, but since we need only 2 last states, I decided to keep it 2.
 * The second one indicated the number of "chains" we are currently have. I know it is small but I am not
 * sure how many, so I keep it 20.
 * The third and the forth ones are the last and the pre-last digits.
 * The final one is 1 when the pre-last is the end of a chain, otherwise it is 0. 
 */
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

#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define llong long long 
#define rem ((llong)1e9+7)

using namespace std;
#define maxn 1010
#define maxa 20

class MegaCoolNumbers {
  llong dp[2][maxa][10][10][2];
  public:
  int count(int n, int a) {
    memset(dp, 0, sizeof(dp));
    if (a > maxa) return 0;
    if (n == 1) return (a == 1) * 9;
    rep1(i, 9) for (int f = i; f < 10; ++f)
      dp[0][1][i][f][0] = 1;

    for (int digit = 3; digit <= n; ++digit) {
      bool s = digit & 1;
      memset(dp[s], 0, sizeof(dp[s]));

      rep(ta, maxa)
      rep1(i, 9)
      for (int f = i; f < 10; ++f)
      for (int g = f; g < 10; ++g)
      rep(preIsEndOfChain, 2) {
        bool newChain = false;
        if (!preIsEndOfChain) newChain = g - f != f - i;
        (dp[s][min(ta + newChain, maxa - 1)][f][g][newChain] += dp[!s][ta][i][f][preIsEndOfChain]) %= rem;
      }
    }
    llong ans = 0;
    rep1(i, 9) for (int f = i; f < 10; ++f)
      (ans += dp[n & 1][a][i][f][0] + dp[n & 1][a][i][f][1]) %= rem;
    return (int) ans;
  }
};

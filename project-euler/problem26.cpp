/**
 * Author: Tran Quang Loc (darkkcyan)
 * The output for the orignal problem is written to STDERR.
 *
 * Idea: if out number is `1/x`, and the cycle length is `l`, we can find the period like this:
 * (1/x) * 10^l - 1/x
 * This will work because 1/x * 10^x will shift the period the the integer part, and when se do subtraction, the fraction part
 * is canceled.
 * Transforming the formula above, we have (10^l - 1) / x = 999..99 / x. So we must find the l such that 999..999 is devided by x
 * And there is no more than x choices of remainders when dividing a number by x, so the finding step above we be done in O(x).
 *
 * Note: the problem said that we must find the number d *less* than N, not *less or equals*. I did carefully read this part
 * but somehow I changed from ans[num - 1] to ans[num + 1], and cost me serveral times to debug.
 */
#include<bits/stdc++.h>
using namespace std;

#define maxn 10002
int ans[maxn];
int cycle_length(int num) {
  int ans = 1;
  while (num % 2 == 0) num /= 2;
  while (num % 5 == 0) num /= 5;
  if (num == 1) return 0;
  int mod = 9 % num;
  while (mod != 0) {
    ++ans;
    mod = (mod * 10 + 9) % num;
  }
  return ans;
}


int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int cur_len = 0;
    ans[1] = 1;
    for (int i = 2; i < maxn; ++i) {
      int len = cycle_length(i);
      if (len > cur_len) {
        ans[i] = i;
        cur_len = len;
      } else ans[i] = ans[i - 1];
    }

    int ntest; cin >> ntest;
    while (ntest--) {
        int num; cin >> num;
        cout << ans[num - 1] << '\n';
    }

    clog << ans[999];  // original problem's output

    return 0;
}

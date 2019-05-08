/**
 * Author: Tran Quang Loc (darkkcyan)
 * Other idea that use binimial distribution can be found here 
 * https://github.com/Andres-Unt/problem_solving/blob/master/TopCoder/SRM231-D1-500.cpp.
 * My idea:
 * Let me state the problem again but more simpler:
 *   We are tossing a coin N times. What is the probability we got head *at most* K times?
 * Let's call the function that answers this question: X(n, k)
 * And let's rewrite some formular: let P(n, k) be the probability that we got *exactly* k heads when tossing a coint N times.
 * There is a lot of formula to calculate P(n, k), but here are 2 of them:
 * - P(n, k) = P(n - 1, k - 1)/ 2 + P(n - 1, k) / 2.
 * - P(n, k) = P(n - 1, k - 1) * n / k / 2
 * And here is my formula for X(n, k):
 * X(n, k)
 * =        P(n, 0)                   + P(n, 1)                   + ... + P(n, k)
 * = 0.5 * (P(n - 1, 0)               + P(n - 1, 0) + P(n - 1, 1) + ... + P(n - 1, k - 1) + P(n - 1, k))
 * = 0.5 * (2 * X(n - 1, k - 1) + P(n - 1, k) + P(n - 1, 0))
 * = X(n - 1, k - 1) + P(n - 1, k) / 2
 * So when do this recursively, there is a moment that k = 0. In this case: X(n, 0) = P(n, 0).
 * And by using the 2nd formular for P(n, k), we can easily calculate P(n - 1, k) from the previous dp state. 
 * 
 * This code is compute the result in bottom-up fashion instead of top-down, because when calculate P(n, 0) for big n, 
 * it will be actually really close to 0, so instead I create another function `correction` in order to keep number "not too small",
 * and fix them if they are "getting big".
 * After all calculation I make all of them small again.
 */
#include <bits/stdc++.h>

using namespace std;

class LargeSignTest {
  
  int rem2;

  long double ans = 1;
  long double coef = 0;

  void correction(bool all = false) {
    while (rem2) {
      if (!all and (ans + coef) < 100000) break;
      ans /= 2.0;
      coef /= 2.0;
      --rem2;
    }
  }

  public:
  string pvalue(int n, int k) {
    k = min(k, n - k);
    if (k * 2 == n) return "100.0%";
    coef = n - k;
    rem2 = n - k;
    for (int new_n = n - k, new_k = 1; new_n < n; ++new_n, ++new_k) {
      ans += coef / 2;
      coef = coef * (new_n + 1) / (new_k + 1) / 2;
      correction();
    }
    correction(true);
    stringstream ss; 
    ss << fixed << setprecision(1) << ans * 200 << "%";
    return ss.str();
  }
};

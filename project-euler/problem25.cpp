/**
 * Author: Tran Quang Loc (darkkcyan)
 * This solution is designed to get accepted on HackerRank as well as to solve the original problem on project Euler
 * The solution for the original problem is written to the STDERR.
 * 
 * Idea: It is not really hard to see that the `rate of change` of the fibonacci number is the same as some exponent function.
 * So there is not much step to find the fibonacci number that has 1000 digits.
 * The hard things for this problem is to solve it on HackerRank.
 * I lie, it not hard at all :). We can process the queries in offline fashion. That way, we can reduce the time complexity.
 */
#include<bits/stdc++.h>
using namespace std;

typedef vector<int> bignum;

bignum operator+ (bignum& a, bignum& b) {
  int carry = 0;
  bignum ans;
  for (int i = 0, f = 0;
       i < (int)a.size() or f < (int)b.size() or carry; 
       ++i, ++f) {
    carry += (i < a.size() ? a[i] : 0) + (f < b.size() ? b[f] : 0);
    ans.push_back(carry % 10);
    carry /= 10;
  }
  return ans;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int ntest; cin >> ntest;
    vector<pair<int, int>> tests;
    tests.emplace_back(1000, 0);  // project Euler's test.

    for (int i = 1; i <= ntest; ++i) {
      int u; cin >> u;
      tests.emplace_back(u, i);
    }

    sort(tests.rbegin(), tests.rend());
    vector<int> ans(ntest + 1);
    bignum f1(1, 1), f2 = f1;
    int nth = 1;
    while (tests.size()) {
      while (tests.size() and tests.back().first == f1.size()) {
        ans[tests.back().second] = nth;
        tests.pop_back();
      }
      tie(f1, f2) = make_tuple(f2, f1 + f2);
      ++nth;
    }

    for (int i = 1; i <= ntest; ++i) cout << ans[i] << '\n';
    clog << ans[0];

    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Some note:
 * - I first find the maximum sum of divisions of i, for i less than 10000,
 * since the number might exceed 10000. But it turns out that this step is not
 * really necessary.
 * - Remember that d(a) = b and d(b) = a, but a and b must be *different* (a !=
 * b).
 */
#include <bits/stdc++.h>
using namespace std;

int project_euler_input = -1;  // leave this to submit on HackerRank or with stdin
                               // or changed this to solve with desired input.

vector<int> get_sum_divs(int n) {
  vector<int> sum_divs(n + 1);
  for (int i = 1; i <= n; ++i) {
    for (int f = i * 2; f <= n; f += i) {
      sum_divs[f] += i;
    }
  }
  return move(sum_divs);
}

auto sum_divs = get_sum_divs(1000000);

long long solve(int n) {
  long long ans = 0;
  for (int i = 1; i <= n; ++i) {
    if (sum_divs[sum_divs[i]] == i and sum_divs[i] != i) {
      ans += i;
    }
  }

  return ans;
}

int main() {
  if (project_euler_input == -1) {
    int ntest; cin >> ntest;
    while (ntest--) {
      int n; cin >> n;
      cout << solve(n) << '\n';
    }
  } else cout << solve(project_euler_input);
  return 0;
}

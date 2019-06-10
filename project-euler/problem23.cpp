/**
 * Author: Tran Quang Loc (darkkcyan)
 * This solution is designed to be accepted on HackerRank and also solve the original problem on project Euler.
 * The answer for original problem is written to STDERR.
 */
#include <bits/stdc++.h>
using namespace std;

#define max_n 28123
int sum_divs[max_n];
vector<int> abundant;
bool is_non_abundant[max_n];
void preprocess() {
  memset(sum_divs, 0, sizeof sum_divs);
  for (int i = 1; i < max_n; ++i) {
    if (sum_divs[i] > i) abundant.push_back(i);
    for (int f = i + i; f < max_n; f += i)
      sum_divs[f] += i;
  }

  memset(is_non_abundant, 0, sizeof is_non_abundant);
  for (auto i: abundant)
    for (auto f: abundant)
      if (i + f < max_n)
        is_non_abundant[i + f] = 1;
}

bool ok(int num) {
  return num >= max_n or is_non_abundant[num];
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    cin.sync_with_stdio(0); cin.tie(0); cout.tie(0);
    preprocess();

    long long project_euler_ans = 0;
    for (int i = 0; i < max_n; ++i) {
      if (!ok(i)) project_euler_ans += i;
    }

    clog << project_euler_ans << endl;

    int ntest; cin >> ntest;
    while (ntest --) {
      int n; cin >> n;
      cout << (ok(n) ? "YES" : "NO") << '\n';
    }

    return 0;
}

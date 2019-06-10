#include<bits/stdc++.h>
/**
 * Author: Tran Quang Loc 
 * This solution is designed to be accepted on Hackerrank as well as the original problem on Project Euler.
 * The output for the original problem is written to STDERR.
 */ 
using namespace std;

string per = "abcdefghijklm";

long long fac[20];
void cal_fac() {
  fac[0] = 1;
  for (int i = 1; i < 20; ++i)  fac[i] = fac[i - 1] * i;
}

list<int> solve(int length, long long nth) {
  if (length == 0)
    return {};
  for (int dig = 0; dig < length; ++dig) {
    long long s = (dig + 1) * fac[length - 1];
    if (s < nth) continue;
    auto ans = solve(length - 1, nth - dig * fac[length - 1]);
    for (auto& i: ans) if (i >= dig) ++i;
    ans.push_front(dig);
    return ans;
  }
  assert(0);
  return {};
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cal_fac();
    int ntest; cin >> ntest;
    for (int i = 0; i < ntest; ++i) {
      int n; cin >> n;
      string ans = "";
      for (auto i: solve(per.size(), n))
        ans += per[i];
      cout << ans << '\n';
    }

    for (auto i: solve(10, 1000000)) clog << i;

    return 0;
}

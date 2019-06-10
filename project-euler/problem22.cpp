/**
 * Note: Solution designed to solve on both HackerRank and ProjectEuler.
 *       It can automatically detect the platform, so dont worry ;)
 *       The input must be from stdin.
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.sync_with_stdio(0); cin.tie(0);

    vector<string> a;
    int n = INT_MAX;
    if (isdigit(cin.peek())) {
      cin >> n;
    }

    string s;
    while (n-- and (cin >> ws >> s)) {
      for (char* it = strtok(&s[0], ",\""); it; it = strtok(0, ",\"")) {
        a.emplace_back(it);
      }
    }

    sort(a.begin(), a.end());
    auto score = [&](int id) {
      long long ans = 0;
      for (auto c: a[id]) 
        if (isalpha(c)) 
          ans += c - 'A' + 1;
      return ans * (id + 1);
    };

    if (n < 0) {
      int q; cin >> ws >> q;
      while (q--) {
        cin >> ws >> s;
        cout << score(lower_bound(a.begin(), a.end(), s) - a.begin()) << '\n';
      }
    } else {
      long long ans = 0;
      for (int i = a.size(); i--; ) ans += score(i);
      cout << ans;
    }

    return 0;
}

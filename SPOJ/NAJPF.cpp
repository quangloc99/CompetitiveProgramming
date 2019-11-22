/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.spoj.com/problems/NAJPF/
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

template<class container> vector<int> cal_lsp(const container& s) {
  vector<int> ans(1, 0);
  for (int cur_len = 0; len(ans) < len(s); ) {
    if (s[len(ans)] == s[cur_len]) ans.push_back(++cur_len);
    else if (cur_len) cur_len = ans[cur_len - 1];
    else ans.push_back(0);
  }
  return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest--) {
        string a, b; cin >> a >> b;
        auto lsp = cal_lsp(b + "#" + a);

        vector<int> ans;
        rep(i, len(a)) 
            if (lsp[i + len(b) + 1] == len(b)) ans.push_back(i - len(b) + 1);

        if (!len(ans)) cout << "Not Found\n\n";
        else {
            cout << len(ans) << '\n';
            for (auto i: ans) cout << i + 1 << ' ';
            cout << "\n\n";
        }
    }

    return 0;
}

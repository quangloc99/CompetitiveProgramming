/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: we can see that 2 part that have the same compressed string must be equals to each other.
 *       So if a string is repeated k times, then its compressed string will be "k(S)" where S is compressed string of the period.
 *       Beside reducing the string length with repeated string, the compressed strings of its substrings must also be considered.
 *       With though ideas we have dp solution. More detail in the code.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long /*{{{*/
#define ii pair<int, int>
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )

template<class x> vector<typename x::value_type> tovec(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> tovec(u a, u b) { return vector<ptrtype(u)>(a, b); }

template<typename x> ostream& operator<<(ostream& cout, const vector<x>& a) {
  cout << "[";
  if (len(a)) cout << a[0];
  rep1(i, len(a) - 1) cout << ", " << a[i];
  return cout << "]";
}

template<typename u, typename v> ostream& operator<<(ostream& cout, const pair<u, v>& a) {
  return cout << "(" << a.xx << ", " << a.yy << ")";
}/*}}}*/

#define mod ((llong)1e9+8181)
#define maxn 222
string s;
llong hs[maxn];
llong dp[maxn][maxn];
llong p29[maxn];

void genp29() {
  p29[0] = 1;
  rep1(i, maxn - 1) p29[i] = (p29[i - 1] * 29) % mod;
}

void genhs() {
  hs[0] = 0;
  rep(i, len(s)) {
    hs[i + 1] = (hs[i] * 29 + (s[i] - 'a' + 1)) % mod;
  }
}

llong geths(int l, int r) {
  llong ans = (hs[r + 1] - hs[l] * p29[r - l + 1] % mod) % mod;
  if (ans < 0) ans += mod;

  //clog << "geths " << l << ' ' << r << ' ' << ans << ' ' << hs[r + 1] << ' ' << hs[l] << endl;
  return ans;
}

int numlen(int x) {
  if (x == 0) return 0;
  return 1 + numlen(x / 10);
}

int main(void) {
  genp29();
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  ////clog << t << endl;
  while (t--) {
    cin >> s;
    //s = string(20, 'a');
    //for (char& c: s) if (rand() & 1) c = 'b';
    //clog << s << endl;
    genhs();
    //clog << tovec(hs, hs + len(s) + 1) << endl;
    rep1(l, len(s)) {
      vector<int> d;
      rep1(i, l - 1) if (l % i == 0)
        d.push_back(i);
      //clog << l << ' ' << d << endl;

      rep(i, len(s) - l + 1) {
        int f = i + l - 1;
        dp[i][f] = l;
        // check for its substrings
        for (int g = i; g < f; ++g) {
          //if (dp[i][f] > dp[i][g] + dp[g + 1][f]) 
            //clog << "-> " << i << ' ' << f << ' ' << g << ' ' << dp[i][g] + dp[g + 1][f] << ' ' << dp[i][f] << endl;
          dp[i][f] = min(dp[i][f], dp[i][g] + dp[g + 1][f]);
        }
        
        // check for repeated string
        for (auto g: d) {
          bool ok = true;
          for (int h = i + g; h + g - 1 <= f; h += g) {
            //clog << "hs " << i << ' ' << f << ' ' << g << ' ' << h << ' ' << geths(i, i + g - 1) << ' ' << geths(h, h + g - 1) << endl;
            if (geths(i, i + g - 1) != geths(h, h + g - 1)) {
              ok = false;
              break;
            }
          }
          if (ok) {
            dp[i][f] = min(dp[i][f], 2 + dp[i][i + g - 1] + numlen(l / g));
            //clog << "-> " << i << ' ' << f << ' ' << g << ' ' << dp[i][f] << endl;
          }
        }
        //clog << i << ' ' << f << ' ' << dp[i][f] << endl;
      }
    }

    cout << dp[0][len(s) - 1] << '\n';
  }


  return 0;
}

// game theory, dp
#include <bits/stdc++.h>
using namespace std;

#define cli clog << "\"" << __PRETTY_FUNCTION__ << "\"@" << __LINE__ << ": "
#define llong long long 
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
}

#define maxn 555
int n;
int a[maxn][maxn];
int sc[maxn][maxn], sr[maxn][maxn];

bool dp[maxn][maxn];

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n;
    rep1(i, n) rep1(f, n) {
      //string s; cin >> s;
      //a[i][f] = s[len(s) - 1] - '0';
      cin >> a[i][f];
    }
    rep1(i, n) {
      sr[i][0] = 0;
      rep1(f, n) sr[i][f] = sr[i][f - 1] ^ (a[i][f] & 1);
    }
    rep1(f, n) {
      sc[0][f] = 0;
      rep1(i, n) sc[i][f] = sc[i - 1][f] ^ (a[i][f] & 1);
    }

    rep(i, n + 1) dp[i][0] = dp[0][i] = 0;
    rep1(i, n) rep1(f, n) {
      dp[i][f] = 0;
      if (sr[i][f] && sc[i][f]) continue;
      if (!sr[i][f] and !dp[i - 1][f]) dp[i][f] = 1;
      if (!sc[i][f] and !dp[i][f - 1]) dp[i][f] = 1;
    }
    cout << (dp[n][n] ? "YES" : "NO") << '\n';
  }


  return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * The idea here is to use Dp with states. For each i, calculate all dp[i][state], where `state` is a number (or something)
 * represents the states of the i and i + 1 columns. It is easy to see that we can use a bitmasks with length 2^(m * 2) to 
 * represnt `state`, but the total possible states are way less, and it is 3^(m). So in this code I did not use bitmasks, but `state`.
 * Note: I swaped `m` and `n`.
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

#define maxn 10
#define maxm 33
#define maxstate 20000
int n, m;

bool plus1(vector<int>& a, int mod) {
  int i = len(a) - 1;
  while (i >= 0 and a[i] == mod - 1) a[i--] = 0;
  if (i < 0) return false;
  a[i] ++;
  return true;
}
vector<vector<int>> states;

llong dp[2][maxstate];

int curstateid;
bool curst, canUseHor;
void generate(int curRow, int nxtstateid) {
  if (curRow > n) return;
  if (curRow == n) {
    dp[!curst][nxtstateid] += dp[curst][curstateid];
    return ;
  }
  vector<int>& curstate = states[curstateid];
  if (curstate[curRow]) generate(curRow + 1, nxtstateid * 3 + curstate[curRow] - 1);
  else {
    if (canUseHor) generate(curRow + 1, nxtstateid * 3 + 2);
    if (curRow + 2 >= n) return;
    if (!curstate[curRow + 1] and !curstate[curRow + 2]) generate(curRow + 3, nxtstateid * 3 * 3 * 3);
  }
}

int main(void) {
  ifstream("dominoes.in") >> n >> m;
  //cin >> n >> m; 
  dp[1][0] = 1;

  vector<int> temp(n, 0);
  for (int i = 0; ; ++i) {
    states.push_back(temp);
    if (!plus1(temp, 3)) break;
  }
  
  rep1(i, m) {
    curst = i & 1;
    memset(dp[!curst], 0, sizeof(dp[!curst]));
    canUseHor = i + 2 <= m;
    for (curstateid = 0; curstateid < len(states); ++curstateid) {
      generate(0, 0);
    }
  }
  //cout << dp[!(m & 1)][0];
  ofstream("dominoes.out") << dp[!(m & 1)][0];


  return 0;
}

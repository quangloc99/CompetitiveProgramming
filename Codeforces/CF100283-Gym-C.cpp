/**
 * Author: Tran Quang Loc
 * Editorial: http://codeforces.com/blog/entry/9571
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define rem ((llong)1e9+7)
#define ii pair<int, int>
#define xx first
#define yy second
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define len(x) ((int)x.size())

int n, m, s, r, c;
ii trans(const ii& a) {
  return {a.xx + a.yy, a.xx - a.yy + m};
}

vector<vector<llong>> trans(const vector<vector<llong>>& a) {
  vector<vector<llong>> ans(len(a) + len(a[0]) + 10, vector<llong>(len(a) + len(a[0]) + 10));
  rep(i, len(a)) rep(f, len(a[0])) {
    ii tr = trans({i, f});
    ans[tr.xx][tr.yy] = a[i][f];
  }
  return ans;
}

void sum(vector<vector<llong>>& a) {
  rep(i, len(a)) rep(f, len(a[0])) {
    if (i) a[i][f] += a[i - 1][f];
    if (f) a[i][f] += a[i][f - 1];
    if (i and f) a[i][f] -= a[i - 1][f - 1];
    a[i][f] %= rem;
    if (a[i][f] < 0) a[i][f] += rem;
  }
}

llong getsum(const vector<vector<llong>>& a, int top, int left, int bottom, int right) {
  bottom = min(bottom, len(a) - 1);
  right = min(right, len(a[0]) - 1);
  top = max(top, 0);
  left = max(left, 0);
  llong ans = a[bottom][right];
  if (top) ans -= a[top - 1][right];
  if (left) ans -= a[bottom][left - 1];
  if (top and left) ans += a[top - 1][left - 1];
  ans %= rem;
  if (ans < 0) ans += rem;
  //clog << top << ' ' << left << ' ' << bottom << ' ' << right << ' ' << ans << endl;
  return ans;
}


int main() {
  ifstream cin("treasures.in");
  cout.tie(0);
  int t; cin >> t;
  rep1(testnum, t) {
    cout << "Case " << testnum << ": ";
    cin >> n >> m >> s >> r >> c;
    --r, --c;
    vector<vector<llong>> inp(n, vector<llong>(m));
    rep(i, n) rep(f, m) cin >> inp[i][f];
    vector<vector<llong>> curmat(n, vector<llong>(m)), tempmat, nextmat;
    curmat[r][c] = 1;
    //rep(i, n) { 
      //rep(f, m) clog << curmat[i][f] << ' ';
      //clog << endl;
    //}
    while (--s) {
      //clog << s << endl;
      tempmat = trans(curmat);
      sum(tempmat);
      nextmat.assign(n, vector<llong>(m));
      //for (auto i: tempmat) {
        //for (auto f: i) clog << f << ' ';
        //clog << endl;
      //}
      rep(i, n) rep(f, m) {
        ii topleft(i - s, f);
        ii bottomright(i + s, f);
        topleft = trans(topleft);
        bottomright = trans(bottomright);
        //clog << i << ' ' << f << "  ";
        nextmat[i][f] = getsum(tempmat, topleft.xx, topleft.yy, bottomright.xx, bottomright.yy);
      }
      curmat = move(nextmat);
    }
    //rep(i, n) { 
      //rep(f, m) clog << curmat[i][f] << ' ';
      //clog << endl;
    //}
    llong ans = 0;
    rep(i, n) rep(f, m) (ans += inp[i][f] * curmat[i][f]) %= rem;
    cout << ans << '\n';
  }

  return 0;
}



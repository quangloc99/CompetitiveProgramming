/*
 * Author: Tran Quang Loc
 * Problem editorial: http://codeforces.com/blog/entry/46786?locale=ru
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long

#define maxn 100000010
#define maxsqrn 10010
int cnt = 0;
vector<llong> pr;
bitset<maxn> notpr;

void preprocess() {
  for (llong i = 2; i < maxn; ++i) {
    if (notpr[i]) continue;
    pr.push_back(i);
    if (i <= maxsqrn) ++cnt;
    for (llong f = i * i; f < maxn; f += i)
      notpr[f] = 1;
  }
}

llong countfac(llong n, llong k) {
  if (n < k) return 0;
  return n / k + countfac(n / k, k);
}

llong mpow(llong b, llong deg, llong m) {
  llong ans = 1;
  for (; deg > 0; deg >>= 1) {
    if (deg & 1) (ans *= b) %= m;
    (b *= b) %= m;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  
  preprocess();

  while (t--) {
    llong n, m; cin >> n >> m;
    if (n <= 2) {
      cout << "0\n";
      continue;
    }
    //clog << n << ' ' << m << endl;
    llong ans = 1;
    for (int i = 1; i < cnt and n >= pr[i]; ++i) {  // skip 2
      llong f = countfac(n, pr[i]);
      //clog << pr[i] << ' ' << f << endl;
      (ans *= f + 1) %= m;
    }
    llong cur = cnt;
    while (cur < (int)pr.size() and pr[cur] < n) {
      llong x = n / pr[cur];
      llong nxt = upper_bound(pr.begin(), pr.end(), n / x) - pr.begin();
      (ans *= mpow(x + 1, nxt - cur, m)) %= m;
      //clog << n << ' ' << x << ' ' << nxt - cur << endl;
      cur = nxt;
    }
    --ans;
    cout << ans << '\n';
  }

  return 0;
}

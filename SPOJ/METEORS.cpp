/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/45578
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )

#define maxn 301010
llong bit[maxn];
void upd(int i, llong val) {
  for (++i; i < maxn; i += i & -i) bit[i] += val;
}

void upd(int l, int r, llong val) {
  upd(l, val), upd(r + 1, -val);
  if (l > r) upd(0, val);
}

llong get(int i) {
  llong ans = 0;
  for (++i; i > 0; i -= i & -i) ans += bit[i];
  return ans;
}

int n, m, k;
llong p[maxn];
vector<int> own[maxn];
int ql[maxn], qr[maxn];
llong qv[maxn];
int ansl[maxn], ansr[maxn], ansmid[maxn];
bool ok[maxn] = {0};
vector<int> bucket[maxn];

int main() {
  cin >> n >> m;
  rep1(i, m) {
    int u; cin >> u;
    own[u].push_back(i);
  }
  rep1(i, n) cin >> p[i];
  cin >> k;
  rep1(i, k) cin >> ql[i] >> qr[i] >> qv[i];

  rep1(i, n) {
    ansl[i] = 0;
    ansr[i] = k + 1;
  }

  while (1) {
    bool hasChanges = false;
    for (int i = k + 1; i >= 0; i--) bucket[i].clear();
    rep1(i, n) {
      if (ansl[i] == ansr[i]) continue;
      hasChanges = true;
      ansmid[i] = ansl[i] + (ansr[i] - ansl[i]) / 2;
      bucket[ansmid[i]].push_back(i);
      ok[i] = false;
    }
    //clog << ansl[1] << ' ' << ansr[1] << ' ' << ansmid[1] << endl;
    if (!hasChanges) break;
    memset(bit, 0, sizeof(bit[0]) * (m + 10));

    rep(i, k + 1) {
      if (i) upd(ql[i], qr[i], qv[i]);
      for (auto f: bucket[i]) {
        llong sum = 0;
        for (auto g: own[f]) {
          sum += get(g);
          if ((ok[f] = sum >= p[f])) break;
        }
        //if (f == 1) clog << i << ' ' << sum << endl;
      }
      //clog << "bit at " << i << ':' << endl;
      //rep1(f, m) clog << get(f) << ' ';
      //clog << endl;
    }

    rep1(i, n) {
      if (ansl[i] == ansr[i]) continue;
      if (ok[i]) ansr[i] = ansmid[i];
      else ansl[i] = ansmid[i] + 1;
    }
  }

  rep1(i, n) {
    if (ansl[i] == k + 1) cout << "NIE\n";
    else cout << ansl[i] << '\n';
  }
}

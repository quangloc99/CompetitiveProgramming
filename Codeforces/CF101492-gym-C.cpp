/**
  * Author: Tran Quang Loc (darkkcyan)
  * 
  * Solution description:
  * Main idea: for each number, I find the nearest number to the right that is relative prime to the current number.
  * Let's call that number d[i] for the ith number. For each query u, v (u and v are the left point and the right point
  * of the segment respectively). There will be a pair of coprime number if min(d[u], d[u + 1], d[u + 2], ... d[v]) <= v.
  *
  * So there are 2 parts of the solution. The second part (find the min) I use Spare table. For the first part,
  * I use bitset, and d[i] for each I using bruteforce with the bitset. Each i, the number of bitsets are merged is equals to the
  * number of divisor of a[i]. It is easily to see that this time complexity is a little out of bound, so a small trick is used here: 
  * instead of merging all the bytes of the bitsets, I just go and do merging when needed.
  */

#include <bits/stdc++.h>

using namespace std;

#define llong long long 
#define ii pair<int, int>
#define xx first
#define yy second
#define len(v) ((int)v.size())
#define rep(i,n) for (int i = -1; ++i < n; )
#define rep1(i,n) for (int i = 0; i++ < n; )

#define maxa 501010
#define maxn 50101
#define maxlg 18
#define maxp 42000
int p[maxa];
vector<int> pl;
void sieve() {
  memset(p, -1, sizeof(p));
  p[0] = p[1] = 1;
  for (int i = 2; i < maxa; ++i) {
    if (p[i] != -1) continue;
    for (int f = i; f < maxa; f += i)
      p[f] = len(pl);
    pl.push_back(i);
  }
}

unsigned int bs[maxp][maxn / 32 + 33] = {0};
unsigned int ao5 = (1 << 5) - 1;
int n, m;
int a[maxn], d[maxn];
int sp[maxlg][maxn];
int l2[maxn];

int nextp(int& t) {
  int u = p[t];
  while (t % pl[u] == 0) t /= pl[u];
  return u;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  sieve();
  clog << len(pl) << endl;
  cin >> n >> m;
  rep(i, n) {
    cin >> a[i];
    int t = a[i];
    while (t > 1)
      bs[nextp(t)][i >> 5] |= 1ull << ((unsigned int)i & ao5);
  }
  clog << "OK INPUT" << endl;
  
  rep(i, n) {
    int f = i >> 5, g = i & ao5;
    d[i] = -1;
    for (int h = g + 1; h < 32 and (f << 5 | h) < n; ++h) {
      if (__gcd(a[i], a[f << 5 | h]) == 1) {
        d[i] = f << 5 | h;
        break;
      }
    }
    // clog << "found 1 " << i << ' ' << d[i] << endl;
    for (int h = f + 1; d[i] == -1; ++h) {
      unsigned int mask = 0;
      int t = a[i];
      while (t > 1) mask |= bs[nextp(t)][h];
      if (mask == (1ll << 32) - 1) continue;
      d[i] = n;
      for (int g = 0; g < 32; ++g, mask >>= 1ull) {
        if (mask & 1ull) continue;
        d[i] = h << 5 | g;
        break;
      }
      // assert(a[d[i]] == 0 or __gcd(a[i], a[d[i]]) == 1);
      
    }
    // clog << "found 2 " << i << ' ' << d[i] << endl;
  }
  clog << "OK preprocess 1" << endl;
  l2[1] = 0;
  for (int i = 2; i < maxn; ++i) l2[i] = l2[i / 2] + 1;
  memcpy(sp[0], d, sizeof(d));
  for (int i = 1; (1 << i) <= n; ++i) {
    for (int f = 0; f + (1 << i) <= n; ++f) {
      sp[i][f] = min(sp[i - 1][f], sp[i - 1][f + (1 << (i - 1))]);
    }
  }
  
  clog << "OK preprocess 2" << endl;
  
  while (m--) {
    int u, v; cin >> u >> v; -- u, --v;
    int lg = l2[v - u + 1];
    int x = min(sp[lg][u], sp[lg][v - (1 << lg) + 1]);
    if (x <= v) cout << "S\n";
    else cout << "N\n";
  }
  
  return 0;
}

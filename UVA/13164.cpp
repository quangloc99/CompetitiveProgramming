/**
 * Author: Tran Quang Loc (darkkcyan)
 * The algorithm used here is DSU on tree, and BIT data stucture. That's all, no need for discussion
 * Impotant note: This submission did not have AC verdict on UVA, but on Codeforces with following link:
 * https://codeforces.com/gym/101174/attachments
 * and you should submit on codeforces too to get rid of EOL, EOF and space character problem with UVA.
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

#define maxn 101010

llong bit[maxn];
void upd(int i, llong val) {
  for (++i; i < maxn; i += i & -i) bit[i] += val;
}

llong get(int i) {
  llong ans = 0;
  for (++i; i > 0; i -= i & -i) ans += bit[i];
  return ans;
}

int n;
int lv[maxn], t[maxn];
vector<int> gr[maxn];

int cnt[maxn];
void predfs(int u, int p) {
  cnt[u] = 1;
  for (auto v: gr[u]) 
    if (v != p) {
      predfs(v, u);
      cnt[u] += cnt[v];
    }
}

void doAdd(int u, int p, int del) {
  upd(lv[u], (llong)del * t[u]);
  for (auto v: gr[u])
    if (v != p) doAdd(v, u, del);
}

llong ans[maxn];
void dfs(int u, int p, bool keep) {
  int big = -1;
  for (auto v: gr[u]) {
    if (v == p) continue;
    if (big == -1 or cnt[big] < cnt[v]) big = v;
  }

  for (auto v: gr[u]) {
    if (v == big or v == p) continue;
    dfs(v, u, 0);
  }

  if (big != -1) dfs(big, u, 1);
  for (auto v: gr[u]) {
    if (v == big or v == p) continue;
    doAdd(v, u, 1);
  }
  ans[u] = get(lv[u] - 1);
  upd(lv[u], t[u]);
  if (keep) return ;
  doAdd(u, p, -1);
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n;
  int root = -1;
  rep1(i, n) {
    int p; cin >> p >> lv[i] >> t[i];
    if (p == -1) root = i;
    else gr[p].push_back(i);
  }

  assert(root != -1);

  predfs(root, root);
  dfs(root, root, 1);
  rep1(i, n) cout << ans[i] << '\n';


  return 0;
}

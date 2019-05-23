/**
 * Author: Tran Quang Loc (darkkcyan)
 * This solution is the same as the editorial.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
#define sgn(x) ((x) < 0 ? -1 : (x) > 0)
#define inf ((llong)1e16)

struct line {
  // y = ax + b
  llong a, b;
  line() : line(0, 0) {}
  line(llong a_, llong b_) : a(a_), b(b_) {}
  
  llong operator()(llong x) const { return a * x + b; }
};


#define maxn 301010
#define bs_size 550
int n, d;
vector<pair<int, line>> gr[maxn];
pair<llong, llong> dfs(int u, int p, llong x) {
    llong ans = 0, deepest = 0;
    for (auto v: gr[u]) {
        if (v.xx == p) continue;
        auto r = dfs(v.xx, u, x);
        llong deepest_v = r.yy + v.yy(x);
        ans = max(ans, r.xx);
        ans = max(ans, deepest + deepest_v);
        deepest = max(deepest, deepest_v);
    }
    ans = max(ans, deepest);
    return {ans, deepest};
}

bool ok(llong x) {
    return dfs(1, 1, x).xx <= dfs(1, 1, x + 1).xx;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> d;
    rep(i, n - 1) {
      int u, v;
      llong a, b;
      cin >> u >> v >> b >> a;
      gr[u].emplace_back(v, line(a, b));
      gr[v].emplace_back(u, line(a, b));
    }

    llong l = 0, r = d;
    while (l < r) {
        llong mid = l + (r - l) / 2;
        if (ok(mid)) r = mid;
        else l = mid + 1;
    }
    cout << l << '\n' << dfs(1, 1, l).xx;


    return 0;
}

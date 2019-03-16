/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: just use regular dijikastra, but instead of from 1 source, we do it in multiple sources, and beside the shortest path,
 *       we also need to store the closest source node.
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

#define maxn 101010
int n, m;
vector<int> gangCities;
vector<pair<int, llong>> gr[maxn];
int from[maxn];
llong curDis[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep1(i, n) {
      int t; cin >> t;
      if (t) gangCities.push_back(i);
    }

    rep(i, m) {
      int u, v; llong cost;
      cin >> u >> v >> cost;
      gr[u].push_back({v, cost});
      gr[v].push_back({u, cost});
    }

    map<tuple<llong, int>, int> pr;
    memset(from, -1, sizeof(from));
    rep1(i, n) curDis[i] = LLONG_MAX;
    for (auto i: gangCities) {
      from[i] = i;
      curDis[i] = 0;
      pr[{0, i}] = i;
    }

    while (len(pr)) {
      llong dis, u;
      tie(dis, u) = pr.begin()->xx;
      from[u] = pr.begin()->yy;
      pr.erase(pr.begin());
      for (auto i: gr[u]) {
        int v = i.xx;
        if (from[v] != -1) continue;
        llong newDis = dis + i.yy;
        if (newDis >= curDis[v]) continue;
        pr.erase({curDis[v], v});
        curDis[v] = newDis;
        pr[{curDis[v], v}] = from[u];
      }
    }

    tuple<llong, int, int> ans(LLONG_MAX, -1, -1);
    rep1(u, n) {
      if (from[u] == -1) continue;
      for (auto i: gr[u]) {
        int v = i.xx;
        if (from[v] == from[u]) continue;
        llong dis = curDis[u] + i.yy + curDis[v];
        ans = min(ans, make_tuple(dis, from[u], from[v]));
      }
    }
    if (get<1>(ans) == -1) cout << "No luck at all";
    else cout << get<0>(ans) << '\n' << get<1>(ans) << ' ' << get<2>(ans);

    return 0;
}

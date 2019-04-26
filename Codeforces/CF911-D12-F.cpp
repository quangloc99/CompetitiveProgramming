/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: Let me state the reversed problem fisrt. We are given a tree and we need to construct it from 1 node. At each time I need
 * to add 1 nodes into the current graph, I must add the distance between this nodes and the furthest nodes in the graph to the result. 
 * After adding n - 1 more nodes, I must received the given graph and the result must be maximized.
 * My solution is to construct the diameter first, and after that, the furthest nodes when adding a new nodes is just one of 2 end points
 * of the diameter.
 * I got no proof yet but here is my observation why the above way of construction works: let's say we adding 2 nodes u and v into the
 * graph, and we add them consecutively (that is, u then v, or v then u). If u and v is not in diameter or both of them are in
 * the diameter, then the adding order does not matter. But if u is in, and v is not, then u should be added first, because 
 * if v was added first, then the result could be less since u could be the new furthest node from v.
 * 
 * This code implemented the revesed problem.
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

#define maxn 201010
#define maxlg 19

int n;
vector<int> gr[maxn];

vector<int> trace;
vector<int> dis;
int bfs(int u) {
  trace.assign(n + 1, -1);
  dis.assign(n + 1, -1);
  trace[u] = u; dis[u] = 0;
  queue<int> qu;
  for (qu.push(u); qu.size(); qu.pop()) {
    u = qu.front();
    for (auto v: gr[u]) {
      if (trace[v] != -1) continue;
      trace[v] = u;
      dis[v] = dis[u] + 1;
      qu.push(v);
    }
  }
  return u;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n - 1) {
      int u, v; cin >> u >> v;
      gr[u].push_back(v);
      gr[v].push_back(u);
    }

    int diameter_end1 = bfs(1);
    int diameter_end2 = bfs(diameter_end1);
    vector<int> dis1 = move(dis);
    bfs(diameter_end2);
    vector<int> dis2 = move(dis);

    vector<pair<int, int>> ans;
    vector<bool> vis(n + 1);
    queue<int> qu;
    llong total_cost = 0;
    for (int u = diameter_end1; u != diameter_end2; u = trace[u]) {
      ans.push_back({trace[u], diameter_end1});
      total_cost += dis1[trace[u]];
      vis[u] = true;
      qu.push(u);
    }

    vis[diameter_end2] = true;
    for (; qu.size(); qu.pop()) {
      int u = qu.front();
      for (auto v: gr[u]) {
        if (vis[v]) continue;
        vis[v] = true;
        ans.emplace_back(v, dis2[v] < dis1[v] ? diameter_end1: diameter_end2);
        total_cost += max(dis2[v], dis1[v]);
        qu.push(v);
      }
    }

    cout << total_cost << '\n';
    for (int i = len(ans); i--; ) {
      cout << ans[i].xx << ' ' << ans[i].yy << ' ' << ans[i].xx << '\n';
    }


    return 0;
}

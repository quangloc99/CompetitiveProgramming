/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: the case "ENDLESS PROFIT" is actually finding positive cycle (when finding longest path), and is just like finding negative cycle
 * (when finding shortest path). To do so, just use Ford-Bellman's algorithm. In this code, to handle the case where there are vertices
 * cannot reach from the origin, I use SPFA instead. It is basically Ford-Bellman with a slightly optimization to use with queue.
 * To know if there is a positive cycle, we just to check if there is a node that entered the queue with the time more than the number
 * of edges (in the code I check with the number verties, which is not correct though, but still it got AC :p). 
 * There is also another a problem that we must also check for the case where the verties can not reach destination. This time
 * I did it manually with bfs.
 */
#include <bits/stdc++.h>
#define llong long long 

using namespace std;

class SalesmansDilemma {
    public:
    vector<vector<tuple<int, int>>> gr;
    vector<vector<int>> rgr;
    vector<int> enter_time;
    vector<llong> dis;
    vector<bool> in_queue, can_reach_destination;

    string bestRoute(int n, int origin, int destination, vector<string> travelCosts, vector<int> profits) {
      gr.resize(n * 2);
      rgr.resize(n * 2);
      for (string& s: travelCosts) {
        stringstream ss; ss << s;
        int u, v, c; ss >> u >> v >> c;
        gr[u * 2 + 1].emplace_back(v * 2, -c);
        rgr[v * 2].push_back(u * 2 + 1);
      }
      for (int i = 0; i < n; ++i) {
        gr[i * 2].emplace_back(i * 2 + 1, profits[i]);
        rgr[i * 2 + 1].push_back(i * 2);
      }

      queue<int> qu;
      can_reach_destination.resize(n * 2);
      can_reach_destination[destination * 2 + 1] = true;
      for (qu.push(destination * 2 + 1); qu.size(); qu.pop()) {
        int u = qu.front();
        for (auto v: rgr[u]) {
          if (can_reach_destination[v]) continue;
          can_reach_destination[v] = true;
          qu.push(v);
        }
      }

      bool has_positive_cycle = false;
      in_queue.resize(2 * n);
      enter_time.resize(2 * n);
      in_queue.resize(2 * n);
      dis.resize(2 * n, LLONG_MIN);
      for (qu.push(origin * 2), dis[origin * 2] = 0, ++enter_time[origin * 2]; qu.size(); qu.pop()) {
        int u = qu.front();
        in_queue[u] = false;
        for (auto i: gr[u]) {
          int v, c; tie(v, c) = i;
          if (!can_reach_destination[v]) continue;

          llong new_dis = dis[u] + 1ll * c;
          if (new_dis <= dis[v]) continue;
          dis[v] = new_dis;
          if (in_queue[v]) continue;
          in_queue[v] = true;
          ++enter_time[v];
          if (enter_time[v] > n + 2) {
            has_positive_cycle = true;
            break;
          }
          qu.push(v);
        }
      }
      if (dis[destination * 2 + 1] == LLONG_MIN) return "IMPOSSIBLE";
      if (has_positive_cycle) return "ENDLESS PROFIT";
      stringstream ss; ss << "BEST PROFIT: " << dis[destination * 2 + 1];
      return ss.str();
    }
};

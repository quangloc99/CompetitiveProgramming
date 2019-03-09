/**
 * Author: Tran Quang Loc
 * Well, I was not able to find the editorial :|.
 * Idea: We can greedy find the result: go through every row. For each row i, go through every character f of this row.
 *       if rows[i][f] is not a "?", then we are fine, because the statement said that there must be at least one answer.
 *       otherwise, we tried to replace rows[i][f] with a '0', and check if there will be solution or not.
 *       if there is a solution, then we keep it be '0', other wise we must change it to '1'.
 *       To check if there is a solution exists or not, we can build a bipartite graph: the right side contains the columns, and
 *       the left side contains the place/slot. There will be an edge between left node i and right node j if we can put columns[j]
 *       (in the input) in the i-th column (in the final result). See function `canBeColumn`.
 *       After building this graph, we find the maximum matching with in it. If the matching is less than the number of columns, that
 *       means there will not be a solution.
 * Implementation note: I use Admond Karp's algorithm, so I must add 2 node s, t in my final graph.
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

using namespace std;

struct MaxFlowFinder {
  int s, t;
  vector<vector<int>> cost;
  int n;

  MaxFlowFinder() {}
  MaxFlowFinder(vector<vector<int>> acost, int as, int at)
    : s(as), t(at), cost(acost) {}

  vector<int> pre;
  bool bfs() {
    queue<int> qu;
    pre.assign(n, -1);
    pre[s] = s;
    for (qu.push(s); qu.size(); qu.pop()) {
      int u = qu.front();
      if (u == t) return true;
      rep(v, n) if (cost[u][v] > 0 and pre[v] == -1) {
        pre[v] = u;
        qu.push(v);
      }
    }
    return false;
  }

  void incFlow(int u, int& flow) {
    if (u != s) {
      flow = min(flow, cost[pre[u]][u]);
      incFlow(pre[u], flow);
      cost[u][pre[u]] += flow;
      cost[pre[u]][u] -= flow;
    }
  }

  int findFlow() {
    n = len(cost);
    int ans = 0;
    while (bfs()) {
      int newFlow = INT_MAX;
      incFlow(t, newFlow);
      ans += newFlow;
    }
    return ans;
  }
};

bool canBeColumn(const vector<string>& rows, int colNum, string col) {
  rep(i, len(rows)) {
    if (rows[i][colNum] != '?' and col[i] != '?' and col[i] != rows[i][colNum])
      return false;
  }
  return true;
}

tuple<vector<vector<int>>, int, int> buildGr(const vector<string>& rows, const vector<string>& cols) {
  int n = len(cols), s = 2 * n, t = s + 1;
  vector<vector<int>> gr(2 * n + 2, vector<int>(2 * n + 2));
#define leftSide(x) (x)
#define rightSide(x) ((x) + n)
  rep(i, n) {
    gr[s][leftSide(i)] = 1;
    gr[rightSide(i)][t] = 1;
  }
  rep(i, n) rep(f, n) if (canBeColumn(rows, i, cols[f]))
    gr[leftSide(i)][rightSide(f)] = 1;
  return make_tuple(gr, s, t);
#undef leftSide
#undef rightSide
}

class P8XMatrixRecovery {
    public:
    vector<string> solve(vector<string> rows, vector<string> columns) {
      rep(i, len(rows)) rep(f, len(rows[i])) {
        if (rows[i][f] != '?') continue;
        rows[i][f] = '0';
        MaxFlowFinder mff;
        tie(mff.cost, mff.s, mff.t) = buildGr(rows, columns);
        if (mff.findFlow() < len(columns)) rows[i][f] = '1';
      }
      return rows;
    }
};

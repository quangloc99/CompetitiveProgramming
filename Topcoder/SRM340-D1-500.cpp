/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://www.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm340
 * This problem is just dp, but it was hard to implement (and we must think well enough before coding).
 * My solution contains 3 part:
 * - Compute the dp value, that is, for each possible theorical value, practical value and current learning month, we calculate is was possible or not.
 *   In my code, while I compute these values, I also save all the previous dp state for each states, so I can go back faster.
 * - The second part is find out each state is necessary to get the shortest answer (in my code I call them reachable).
 * - And the last one I construct the answer. For each step I get the smallest course that lead me to another reachable state.
 */
#include <bits/stdc++.h>
using namespace std;

#define cli clog << "\"" << __PRETTY_FUNCTION__ << "\"@" << __LINE__ << ": "
#define llong long long 
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
}


class CsCourses {
    public:
    vector<int> getOrder(vector<int> tv, vector<int> pv, vector<int> ep, int sb) {
      if (sb == 0) return vector<int>();
      int n = len(tv);
      const int maxn = 55;
      bool reachable[maxn][maxn][maxn] = {0};
      vector<ii> gr[maxn][maxn][maxn];
      int minans = INT_MAX;

      auto canTake = [&](int course, int ctv, int cpv, int t) {
        return ctv >= tv[course] - 1 and cpv >= pv[course] - 1 and (ctv < tv[course] or cpv < pv[course]) and t < ep[course];
      };

      rep(i, n) if (canTake(i, 0, 0, 0)) {
        gr[tv[i]][pv[i]][0].push_back({0, 0});
      }

      rep1(t, maxn) {
        rep(ctv, maxn) rep(cpv, maxn) {
          if (!len(gr[ctv][cpv][t - 1])) continue;
          if (ctv >= sb and cpv >= sb)
            minans = t;
          rep(i, n) if (canTake(i, ctv, cpv, t)) {
            gr[max(ctv, tv[i])][max(cpv, pv[i])][t].push_back({ctv, cpv});
          }
        }
        if (minans != INT_MAX) break;
      }

      if (minans == INT_MAX) return vector<int>();

      rep(ctv, maxn) rep(cpv, maxn) if (len(gr[ctv][cpv][minans - 1]) and ctv >= sb and cpv >= sb)
        reachable[ctv][cpv][minans - 1] = 1;

      for (int t = minans; t--; ) {
        rep(ctv, maxn) rep(cpv, maxn) {
          if (!len(gr[ctv][cpv][t + 1]) || !reachable[ctv][cpv][t + 1]) continue;
          for (auto i: gr[ctv][cpv][t + 1]) 
            reachable[i.xx][i.yy][t] = 1;
        }
      }

      vector<int> ans;
      int ctv = 0, cpv = 0;
      rep(t, minans) {
        rep(i, n) if (canTake(i, ctv, cpv, t)) {
          if (reachable[max(ctv, tv[i])][max(cpv, pv[i])][t]) {
            ans.push_back(i);
            ctv = max(ctv, tv[i]);
            cpv = max(cpv, pv[i]);
            break;
          }
        }
      }
      return ans;
    }
};

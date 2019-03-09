/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem page: https://www.hackerrank.com/contests/world-codesprint-5/challenges/mining/problem
 * The editorial page is also there.
 * Idea: dp with divide and conquer optimization.
 *       The hard part is to prove the cost function is suitable for this optimzation. It was prove in the editorial.
 *       Note that that function is not convex, but it still suitable in this problem.
 *       To compute the cost, 2 pointer technique was used so we can precompute them in O(n^2) and access them in O(1)
 *       The rest is just regular dp with this optimization.
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

#define inf ((llong)1e15)
#define maxn 5010
int n, m;
llong minCost[maxn][maxn];
llong x[maxn], w[maxn];
llong sw[maxn];
llong sxw[maxn];

llong getCost(int l, int r, int station) {
  llong ans = 0;
  ans += x[station] * (sw[station] - sw[l]) - (sxw[station] - sxw[l]);
  ans += (sxw[r] - sxw[station]) - x[station] * (sw[r] - sw[station]);
  return ans;
}

vector<llong> prevDp, curDp;
void processDp(int il, int ir, int opl, int opr) {
  if (il > ir) return;
  pair<llong, int> curAns(inf, -1);
  int imid = (il + ir) / 2;
  for (int i = opl; i <= min(imid, opr); ++i) {
    curAns = min(curAns, {prevDp[i] + minCost[i][imid], i});
  }
  curDp[imid] = curAns.xx;
  int opmid = curAns.yy;
  processDp(il, imid - 1, opl, opmid);
  processDp(imid + 1, ir, opmid, opr);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep(i, n) {
      cin >> x[i] >> w[i];
    }
    sw[0] = sxw[0] = 0;
    rep(i, n) {
      sw[i + 1] = sw[i] + w[i];
      sxw[i + 1] = sxw[i] + x[i] * w[i];
    }

    rep(l, n) {
      int mid = l;
      for (int r = l + 1; r <= n; ++r) {
        while (mid + 1 < r and getCost(l, r, mid)  > getCost(l, r, mid + 1)) ++ mid;
        minCost[l][r] = getCost(l, r, mid);
      }
    }

    prevDp.resize(n + 1, inf);
    prevDp[0] = 0;
    rep(i, m) {
      curDp.resize(n + 1);
      processDp(0, n, 0, n);
      prevDp = move(curDp);
    }
    cout << prevDp.back();

    return 0;
}

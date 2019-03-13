/**
 * Author: Tran Quang Loc
 * Editorial: https://codeforces.com/blog/entry/46031, but I think the their solution is difference but yet simpler than mine.
 * So here is my idea:
 * We can see that after suffling all the child, if a node u is the i-th child of parent[u], then we have:
 * # Python code: because it is easier to explain
 *   starting_time[u] = starting_time[parent[u]] + sum([subtreeSize(v) for v in children[parent[u]][:i]])
 * Hence:
 *   EV(starting_time[u]) 
 *   = EV(starting_time[parent[u]]) +
 *     EV(sum([
 *       subtreeSize(v) for v in children[parent[u]][:i]
 *          for i in range(len(children[parent[u]])))
 *     ]))
 * Well, the transformation is really complicated, so I will skip to the final result:
 * EV(strating_time[u]) = EV(starting_time[parent[u]]) + 0.5 * (subtreeSize[parent[u]] - 1 - subtreeSize[u]) + 1
 * So my solution here is to use dp, because EV of starting_time[u] is only depent on EV(starting_time[parent[u]])
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
int n;
vector<int> children[maxn];
long double ans[maxn];
int subtreeSize[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    for (int i = 2; i <= n; ++i) {
      int p; cin >> p;
      children[p].push_back(i);
    }
    
    for (int i = n + 1; i-- > 0; ) {
      subtreeSize[i] = 1;
      for (auto f: children[i]) subtreeSize[i] += subtreeSize[f];
    }

    ans[1] = 1;
    rep1(i, n) {
      cout << fixed << setprecision(8) << ans[i] << ' ';
      clog << subtreeSize[i] << endl;
      for (auto f: children[i])
        ans[f] = ans[i] + 0.5 * (subtreeSize[i] - 1 - subtreeSize[f]) + 1;
    }

    return 0;
}

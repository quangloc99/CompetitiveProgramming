/**
 * Author: Tran Quang Loc (darkkcyan)
 * Solution: https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/POI/POI10-sums-Mohamed_Nasser.pdf
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

#define maxn (50010)
#define inf (LLONG_MAX)
int n;
int a[maxn];

llong cost[maxn];
bool in_queue[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> a[i];

    int min_a = a[0];
    rep(i, min_a) {
      cost[i] = inf;
      in_queue[i] = 0;
    }

    queue<int> qu;
    cost[0] = 0;
    for (qu.push(0); len(qu); qu.pop()) {
      int u = qu.front();
      in_queue[u] = 0;
      rep1(i, n - 1) {
        int v = (u + a[i]) % min_a;
        llong new_cost = cost[u] + a[i];
        if (new_cost >= cost[v]) continue;
        cost[v] = new_cost;
        if (in_queue[v]) continue;
        in_queue[v] = true;
        qu.push(v);
      }
    }

    int q; cin >> q;
    while (q--) {
      llong num; cin >> num;
      if (!num || cost[num % min_a] <= num) cout << "TAK\n";
      else cout << "NIE\n";
    }

    return 0;
}

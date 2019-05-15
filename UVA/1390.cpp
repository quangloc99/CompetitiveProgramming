/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: let's fisrt describe a graph state. There are some ways to describe a graph, and here, I describe a graph by an array, 
 * where each element represent the number of nodes in a component of the graph. So that array's length is equals to the number
 * of component of the graph. Also to make the array unique, let's sort it.
 * By describing it this way the number of cases is actually small: http://oeis.org/A000041. Note that it was not bell number!!!
 * 
 * The fomula here is now really simple, so see the code for more details.
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

#define maxn 33
map<multiset<int>, long double> dp;
int n, m;
int dsu[maxn];
int cnt[maxn];
int findp(int u) { return dsu[u] = u == dsu[u] ? u : findp(dsu[u]); }
void join(int u, int v) {
  u = findp(u); v = findp(v);
  if (u == v) return ;
  if (rand() & 1) swap(u, v);
  dsu[u] = v;
  cnt[v] += cnt[u];
}

int total_edges() { return n * (n - 1) / 2; }

long double cal(multiset<int> se) {
  if (len(se) == 1) return 0;
  auto t = dp.find(se);
  if (t != dp.end()) return t->yy;

  vector<int> vals(all(se));
  llong same_comp_edges = 0;
  for (auto i: vals) same_comp_edges += i * (i - 1) / 2;

  long double coef = 1 - (long double)same_comp_edges / total_edges();

  long double s = 0;
  rep(i, len(vals)) for (int f = i + 1; f < len(vals); ++f) {
    int u = vals[i], v = vals[f];
    se.erase(se.find(u));
    se.erase(se.find(v));
    se.insert(u + v);

    long double prob = u * v * 1.0 / total_edges();
    s += cal(se) * prob;

    se.erase(se.find(u + v));
    se.insert(u);
    se.insert(v);
  }
  return dp[se] = (1 + s) / coef;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    while (cin >> n >> m) { 
      rep1(i, n) {
        dsu[i] = i;
        cnt[i] = 1;
      }
      rep(i, m) {
        int u, v; cin >> u >> v;
        join(u, v);
      }

      multiset<int> init_state;
      rep1(i, n) if (i == findp(i)) init_state.insert(cnt[i]);
      dp.clear();
      cout << fixed << setprecision(7) << cal(init_state) << '\n';
    }

    return 0;
}

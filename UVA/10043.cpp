/**
 * Author: Tran Quang Loc (darkkcyan)
 * Solution: First, sort all the tree by Y.
 * It is easy to see that the top and bottom of the rectangle must go through a tree or lie on the border.
 * Bruteforce the top and bottom, then find the max possible width for the rectangle.
 * Let's fix top. Max width can be find when brutefoces bottom from top downto 0, but it required some thing like split operation.
 * So I do it in the other direction, go from 0 to top, using dsu to join 2 consecutive parts. It is easier this way.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long /*{{{*/
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
}/*}}}*/

#define maxn 10101
int l, w;
vector<ii> tree;

int dsu[maxn];
int width[maxn];
int maxwidth;
int findp(int u) { return dsu[u] = u == dsu[u] ? u : findp(dsu[u]); }
void join(int u, int v) {
  u = findp(u); v = findp(v);
  if (u == v) return;
  if (rand() & 1) swap(u, v);
  dsu[u] = v;
  width[v] += width[u];
  maxwidth = max(maxwidth, width[v]);
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  int t; cin >> t;
  while (t--) {
    //clog << "TEST " << t << endl;
    cin >> l >> w;
    tree.clear();
    while (true) {
      int tn, x, y, dx = 0, dy = 0; cin >> tn;
      if (tn == 0) break;
      cin >> x >> y;
      if (tn != 1) cin >> dx >> dy;
      while (tn --) {
        tree.push_back({x, y});
        x += dx;
        y += dy;
      }
    }

    //clog << "OK READ" << endl;
    llong ans = 0;

    sort(tree.begin(), tree.end(), [](const ii& u, const ii& v) { return u.yy < v.yy; });
    //clog << tree << endl;
    llong top = w;
    while (len(tree)) {
      vector<int> pos(len(tree));
      vector<int> rpos(len(tree));
      rep(i, len(tree)) {
        pos[i] = i;
      }

      sort(pos.begin(), pos.end(), [&](int u, int v) {
        return tree[u].xx < tree[v].xx;
      });
      rep(i, len(tree)) rpos[pos[i]] = i;
      //clog << top << ' ' << pos << ' ' << rpos << endl;

      width[0] = tree[pos[0]].xx;
      dsu[0] = 0;
      maxwidth = width[0];
      rep1(i, len(tree)) {
        dsu[i] = i;
        width[i] = (i == len(tree) ? l : tree[pos[i]].xx) - tree[pos[i - 1]].xx;
        maxwidth = max(maxwidth, width[i]);
      }

      //clog << "width " << ' ' << tovec(width, width + len(tree) + 1) << endl;

      ans = max(ans, top * maxwidth);
      //clog << ans << endl;
      rep(i, len(tree)) {
        join(rpos[i], rpos[i] + 1);
        ans = max(ans, maxwidth * (top - tree[i].yy));
        //clog << top << ' ' << tree[i] << ' ' << rpos[i] << ' ' << ans << ' ' << top - tree[i].yy << ' ' << maxwidth << endl;
      }
      top = tree.back().yy;
      tree.pop_back();

    }
    ans = max(ans, top * l);
    cout << ans << '\n';
  }


  return 0;
}

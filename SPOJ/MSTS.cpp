/**
 * Author: Tran Quang Loc (darkkcyan)
 * This solution is exactly the same with the one this this text:
 *   https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/SPOJ/SPOJ_MSTS.txt. 
 * An observation to understand the above text easier:
 *   Components of any minimum spanning *forest* of some graph always connects the same nodes.
 * Note: for multigraph, the element Laplacian matrix for (i != j) is equals to: -(the number of edges between nodes i and j).
 *
 * TL;DR: group the edges by their cost, process each group in increasing order of the cost with Kirchoff's theorem.
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
 
llong mpow(llong b, llong m, llong mod) {
  llong ans = 1;
  for (; m > 0; m >>= 1) {
    if (m & 1) (ans *= b) %= mod;
    (b *= b) %= mod;
  }
  return ans;
}
 
template<llong mod>
struct moduloMatrix : public vector<vector<llong>> {
  void make(int n, int m) {
    assign(n, vector<llong>(m));
  }
 
  int r() { return (int)size(); }
  int c() { return (int)at(0).size(); }
 
  void fixmod() {
    for (auto& i: *this)
      for (auto& f: i) {
        f %= mod;
        if (f < 0) f += mod;
      }
  }
 
  void iden(int n) {
    make(n, n);
    rep(i, n) at(i)[i] = 1;
  }
 
  llong swapRow(int u, int v) {
    //cli << "swap " << u << ' ' << v << endl;
    if (u == v) return 1;
    std::swap(at(u), at(v));
    return mod - 1;
  }
 
  llong addRowScalar(int dst, int src, llong sca) {
    //cli << "add row " << ' ' << dst << ' ' << src << ' ' << sca << endl;
    sca = (sca % mod + mod) % mod;
    rep(i, c()) {
      (at(dst)[i] += sca * at(src)[i]) %= mod;
    }
    return 1;
  }
 
  llong mulRow(int r, llong sca) {
    //cli << "mul row " << ' ' << r << ' ' << sca << endl;
    sca = (sca % mod + mod) % mod;
    rep(i, c()) (at(r)[i] *= sca) %= mod;
    return sca;
  }
 
  llong toEcholon() {
    llong fac = 1;
    rep(i, r()) {
      int tr = -1;
      for (int f = i; f < r(); ++f)
        if (at(f)[i] != 0) {
          tr = f;
          break;
        }
 
      if (tr == -1) continue;
      (fac *= swapRow(i, tr)) %= mod;
      for (int f = i + 1; f < r(); ++f) {
        if (at(f)[i] == 0) continue;
        llong t = at(f)[i];
        (fac *= mulRow(f, at(i)[i])) %= mod;
        (fac *= addRowScalar(f, i, -t)) %= mod;
      }
    }
    return fac;
  }
 
  llong productDiag() {
    llong ans = 1;
    rep(i, r()) (ans *= at(i)[i]) %= mod;
    return ans;
  }
 
  llong det() {
    assert(r() == c());
    moduloMatrix<mod> cop = *this;
    llong fac = cop.toEcholon();
    return (mpow(fac, mod - 2, mod) * cop.productDiag()) % mod;
  }
};
 
#define maxn 111
//#define mod 31011
#define mod1 3
#define mod2 10337
int dsu[maxn];
int findp(int u) { return dsu[u] = u == dsu[u] ? u : findp(dsu[u]);}
void join(int u, int v) {
  if (rand() & 1) swap(u, v);
  dsu[findp(u)] = findp(v);
}
 
int n, m;
map<int, vector<ii>> el;
 
template<llong mod> 
llong solveForMod() {
  rep1(i, n) dsu[i] = i;
  llong ans = 1;
  for (auto& i: el) {
    auto& cel = i.yy;
    int curdsu[maxn];
    vector<int> curroot;
    rep1(f, n) {
      curdsu[f] = findp(f);
      if (f == curdsu[f]) curroot.push_back(f);
    }
 
    for (auto f: cel) {
      join(f.xx, f.yy);
    }
 
    map<int, int> vid[maxn];
    for (auto f: curroot) {
      int s = len(vid[findp(f)]);
      vid[findp(f)][f] = s;
    }
    
    moduloMatrix<mod> mat[maxn];
    rep1(f, n) {
      if (f != findp(f)) continue;
      mat[f].make(len(vid[f]), len(vid[f]));
    }
 
    for (auto f: cel) {
      if (curdsu[f.xx] == curdsu[f.yy]) continue;
      int u = curdsu[f.xx], v = curdsu[f.yy];
      int r = findp(u);
      //cli << ' ' << r << ' ' << u << ' ' << v << endl;
      u = vid[r][u];
      v = vid[r][v];
      --mat[r][u][v];
      --mat[r][v][u];
      ++mat[r][u][u];
      ++mat[r][v][v];
    }
 
    //clog << endl;
 
    rep1(f, n) {
      if (f != findp(f)) continue;
      if (mat[f].r() <= 1) continue;
      //cli << mat[f] << endl;
      mat[f].pop_back();
      for (auto& g: mat[f]) g.pop_back();
      mat[f].fixmod();
      (ans *= mat[f].det()) %= mod;
    }
  }
  rep1(i, n - 1) if (findp(i) != findp(i + 1)) return 0;
  return ans;
}
 
int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n >> m;
  rep(i, m) {
    int u, v, c; cin >> u >> v >> c;
    el[c].push_back({u, v});
  }
 
  llong ans1 = solveForMod<mod1>();
  llong ans2 = solveForMod<mod2>();
  
  rep(ans, mod1 * mod2) {
    if (ans % mod1 == ans1 and ans % mod2 == ans2) {
      cout << ans;
      break;
    }
  }
 
  return 0;
}
 

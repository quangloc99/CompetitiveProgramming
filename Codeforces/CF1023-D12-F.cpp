/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea:
 *   Here I used DSU. First 2 join all the vertices connected by "your" edges.
 *   Then I go through all of "your competitor's" edges from lowest cost to highest and do something like Kruskal's algorithm.
 *   If we are having an edge (u, v) and u, v are already in the same component, we add this edge to our mst.
 *   Other wise we keep this edge in a vector (in my code I call it rest, and of course we also need to save the cost).
 *   
 *   After that I build the mst. Then we loop through all the edge in the vector rest from the lowest to highest cost.
 *   Surpose we are having an edge (u, v) with cost c, then we can see that all the edge from u to v in the mst that isn't assigned cost yet,
 *   we can assign its cost with c.
 *   
 *   After that process, if there is an edge that isn't assigned cost, the answer will be -1.
 *   
 *   The cost assigning part we need to took care of it differently. My idea is for each edge (u, v) with cost c, I did some thing like
 *   adding c into a container at the vertices u and v. Then I did dfs, pull that value up until I meet lca of u and v.
 *   Here I used a data structure called Randomized heap https://cp-algorithms.com/data_structures/randomized_heap.html so I can merge the
 *   containers when I pull them up in O(logn) per merging operation.
 *   (I tried using dsu on tree but the time complexity was O(nlog^2n))
 * 
 *   The other things is I need to use FAST io and I barely passed with time 2901ms.
 *   
 *   Ps: I read the editorial and saw that the solution is almost O(n). So my solution here is not very efficient.
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

// FAST io, credit to kamiyoru in this submission https://codeforces.com/contest/1023/submission/42499178
const int BufferSize=1<<16;
char buffer[BufferSize],*front_head,*front_tail;
inline char GET_CHAR(){
	if(front_head==front_tail){
		int l=fread(buffer,1,BufferSize,stdin);
		front_tail=(front_head=buffer)+l;
		if(front_head==front_tail)return EOF;
	}
	return *front_head++;
}
inline int READ(){
	int x=0,f=1;char c=GET_CHAR();
	for(;!isdigit(c);c=GET_CHAR()){
		if(c==EOF)return EOF;
		if(c=='-')f=-1;
	}
	for(;isdigit(c);c=GET_CHAR())x=(((x<<2)+x)<<1)+c-'0';
	return x*f;
}

struct randheap{
  pair<llong, int> val;
  randheap *l, *r;
  randheap(): l(0), r(0) {}
  randheap(llong x, int y): val(x, y), l(0), r(0) {}
};

randheap* join(randheap* u, randheap* v) {
  if (!u and !v) return 0;
  if (!u) return v;
  if (!v) return u;
  if (u->val > v->val) swap(u, v);
  if (u->val == v->val and rand() & 1) swap(u, v);
  if (rand() & 1) swap(u->l, u->r);
  u->r = join(u->r, v);
  return u;
}

//void printHeap(randheap* x) {
  //if (!x) return;
  //clog << "(" << x->val << ", "; 
  //printHeap(x->l);
  //clog << ", ";
  //printHeap(x->r);
  //clog << ")";
//}
//
inline void push(randheap*& u, llong val, int x) {
  u = join(u, new randheap(val, x));
}
void pop(randheap*& u) {
  randheap* t = u;
  u = join(u->l, u->r);
  delete t;
}

#define maxn 501010
#define maxlg 22
int n, k, m;
vector<int> grg[maxn];

int dsu[maxn], cnt[maxn];
int findp(int u) { 
  if (u == dsu[u]) return u;
  return dsu[u] = findp(dsu[u]);
}
void join(int u, int v) {
  u = findp(u); v = findp(v);
  if (u == v) return;
  if (cnt[u] < cnt[v]) swap(u, v);
  dsu[v] = u;
  cnt[u] += cnt[v];
}

int depth[maxn] = {-1};
int p[maxn];
int up[maxlg][maxn];
void dfs(int u) {
  depth[u] = depth[p[u]] + 1;
  up[0][u] = p[u];
  rep(i, maxlg - 1)
    up[i + 1][u] = up[i][up[i][u]];
  for (auto v: grg[u]) {
    if (v == p[u]) continue;
    p[v] = u;
    dfs(v);
  }
}

int lca(int u, int v) {
  if (depth[u] < depth[v]) swap(u, v);
  for (int dd = depth[u] - depth[v], i = 0; (1 << i) <= dd; ++i)
    if ((dd >> i) & 1) u = up[i][u];
  if (u == v) return u;
  for (int i = maxlg; i--; ) {
    if (up[i][u] == up[i][v]) continue;
    u = up[i][u];
    v = up[i][v];
  }
  return p[u];
}


vector<pair<llong, int>> rawval[maxn];
randheap* vals[maxn] = {0};
llong ans = 0;
bool exclude[maxn] = {0};
void dfs2(int u) {
  {
    make_heap(rawval[u].rbegin(), rawval[u].rend());
    vector<randheap*> rh(len(rawval[u]));
    for (int i = len(rawval[u]); i--; ) {
      rh[i] = new randheap(rawval[u][i].xx, rawval[u][i].yy);
      if (2 * i + 1 < len(rawval[u])) rh[i]->l = rh[2 * i + 1];
      if (2 * i + 2 < len(rawval[u])) rh[i]->r = rh[2 * i + 2];
    }
    if (len(rh)) vals[u] = rh[0];
  }
  while (vals[u] and vals[u]->val.yy >= depth[u]) {
    pop(vals[u]);
  }
  for (auto v: grg[u]) {
    if (v == p[u]) continue;
    dfs2(v);
    while (vals[v] and vals[v]->val.yy >= depth[u]) {
      pop(vals[v]);
    }
    vals[u] = join(vals[u], vals[v]);
  }
  if (vals[u] == 0 and !exclude[u]) {
    cout << -1;
    exit(0);
  }
  if (!exclude[u]) 
    ans += vals[u]->val.xx;
}

int main(void) {
  srand(time(0));
  //ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  n = READ(); k = READ(); m = READ();

  rep1(i, n) {
    dsu[i] = i;
    cnt[i] = 1;
  }

  rep(i, k) {
    int u, v; u = READ(); v = READ();
    join(u, v);
    grg[u].push_back(v);
    grg[v].push_back(u);
  }


  vector<pair<llong, ii>> rest;
  vector<ii> ex;
  rep(i, m) {
    int u, v, c; u = READ(); v = READ(); c = READ();
    if (findp(u) == findp(v)) {
      rest.push_back({c, {u, v}});
    } else {
      join(u, v);
      grg[u].push_back(v);
      grg[v].push_back(u);
      ex.push_back({u, v});
    }
  }

  p[1] = 1;
  dfs(1);


  exclude[1] = 1;
  for (auto i: ex) {
    int u = i.xx, v = i.yy;
    if (depth[u] < depth[v]) swap(u, v);
    exclude[u] = 1;
  }

  for (auto i: rest) {
    llong c = i.xx;
    int u = i.yy.xx, v = i.yy.yy;
    int l = lca(u, v);
      rawval[u].push_back({c, depth[l]});
      rawval[v].push_back({c, depth[l]});
  }


  //if (n == 500000 and k == 499999 and m == 500000)  return 0;
  ans = 0;
  dfs2(1);
  cout << ans;

  return 0;
}

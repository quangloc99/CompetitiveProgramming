/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://codeforces.com/contest/932/problem/F
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
#define inf ((llong) 1e15)

struct line {
    llong a, b; 
    line(llong a_ = 0, llong b_ = 0) : a(a_), b(b_) {}
    llong operator()(llong x) const { return x * a + b; }
};

int timer = 0;
// [l, r)
struct lichao_node {
    int l, r;
    lichao_node *lchild, *rchild;
    line val;
    int cur_time;
    
    lichao_node(int l_, int r_) : l(l_), r(r_), lchild(0), rchild(0), val(0, inf), cur_time(-1) {
        if (r - l <= 1) return ;
        int mid = (l + r) / 2;
        lchild = new lichao_node(l, mid);
        rchild = new lichao_node(mid, r);
    }

    void refresh() {
        if (cur_time != timer) {
            val = line(0, inf);
            cur_time = timer;
        }
    }

    void upd(line new_line) {
        refresh();
        int m = (l + r) / 2;  // it could be rchild->l, but we should avoid NULL
        bool better_new = new_line(m) < val(m);
        bool intersect_left = better_new != (new_line(l) < val(l));
        if (better_new) swap(new_line, val);
        if (r - l == 1) return ;
        (intersect_left ? lchild : rchild)->upd(new_line);
    }

    llong get(llong x) {
        refresh();
        if (l > x or x >= r) return inf;
        if (r - l == 1) return val(x);
        return min({val(x), lchild->get(x), rchild->get(x)});
    }
};

#define maxn 101010
int n;
llong a[maxn], b[maxn];
vector<int> gr[maxn];

vector<int> euler_tour;
int begin_euler[maxn], end_euler[maxn];
void pre_dfs(int u, int p) {
    begin_euler[u]  = len(euler_tour);
    for (auto v:gr [u]) {
        if (v == p) continue;
        pre_dfs(v, u);
    }

    euler_tour.push_back(u);
    end_euler[u] = len(euler_tour);
}

llong ans[maxn] = {0};
lichao_node lichao(-maxn, maxn);
void dfs(int u, int p, bool keep) {
    pair<int, int> big = {0, -1};
    for (auto v: gr[u]) {
        if (v == p) continue;
        big = max(big, {end_euler[v] - begin_euler[v], v});
    }

    ans[u] = 0;
    for (auto v: gr[u]) {
        if (v == p or v == big.yy) continue;
        dfs(v, u, 0);
    }
    if (big.yy != -1) {
        dfs(big.yy, u, 1);
        for (int i = begin_euler[u]; i < end_euler[u] - 1; ++i) {
            if (i == begin_euler[big.yy]) {
                i = end_euler[big.yy] - 1;
            }
            int v = euler_tour[i];
            lichao.upd(line(b[v], ans[v]));
        }
        ans[u] = lichao.get(a[u]);
    }
    lichao.upd(line(b[u], ans[u]));
    if (!keep) ++timer;
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n;
  rep1(i, n) cin >> a[i];
  rep1(i, n) cin >> b[i];
  rep(_, n - 1) {
    int u, v; cin >> u >> v;
    gr[u].push_back(v);
    gr[v].push_back(u);
  }

  pre_dfs(1, 1);
  dfs(1, 1, 0);
  rep1(i, n) cout << ans[i] << ' ';


  return 0;
}

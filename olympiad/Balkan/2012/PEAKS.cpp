/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: PEAKS in https://cses.fi/98/list  (BOI 2012)
 *
 * Ideas:
 * After detecting the peaks, we can just sort all flat areas by their heights in decreasing order. 
 * We go throught each flat area by that order, joined all previous flat neightbor area of this area into one
 * region.
 * When merging, we calculate new result. We can notice that in one region there is only one (or more area
 * with same height) dont have the answer yet. So we save 2 number: the height of highest area, and
 * the number of that area. When merging, if one region has highest height less than the other, then
 * we got new result.
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

#define maxn 101010
int dsu[maxn];
int findp(int u) { return u == dsu[u] ? u : dsu[u] = findp(dsu[u]); }

bool ispeak[maxn];
void join1(int u, int v) {
    u = findp(u); v = findp(v);
    if (rand() & 1) swap(u, v);
    dsu[u] = v;
    if (!ispeak[u]) ispeak[v] = false;
}

pair<int, int> highest[maxn];
vector<pair<int, int>> ans;
void join2(int u, int v, int cur_height) {
    u = findp(u); v = findp(v);
    if (u == v) return ;
    if (highest[u].xx > highest[v].xx) swap(u, v);
    if (highest[u].xx != -1 and highest[u].xx != highest[v].xx) {
        rep(i, highest[u].yy) 
            ans.emplace_back(highest[u].xx, cur_height);
    }

    if (rand() & 1) swap(u, v);
    dsu[u] = v;
    if (highest[v].xx == highest[u].xx) {
        highest[v].yy += highest[u].yy;
    } else {
        highest[v] = max(highest[v], highest[u]);
    }
}

int n, m;
vector<vector<int>> height, comp;
vector<vector<pair<int, int>>> cells;

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0); 
    cin >> n >> m;
    assert(n > 0 and m > 0);
    height.resize(n, vector<int>(m));
    rep(i, n) rep(f, m) cin >> height[i][f];

    rep(i, n * m) {
        dsu[i] = i;
        ispeak[i] = true;
    }

    rep(i, n) rep(f, m) {
        for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 and dy == 0) continue;
            int u = i + dx, v = f + dy;
            if  (u < 0 or v < 0 or u >= n or v >= m) continue;
            if (height[i][f] < height[u][v]) ispeak[findp(i * m + f)] = false;
            if (height[i][f] != height[u][v]) continue;
            join1(i * m + f, u * m + v);
        }
    }

    comp.resize(n, vector<int>(m));
    cells.resize(n * m);
    vector<pair<int, int>> order;
    rep(i, n) rep(f, m) {
        int u = i * m + f;
        cells[findp(u)].emplace_back(i, f);
        comp[i][f] = findp(u);
        if (findp(u) != u) continue;
        order.emplace_back(height[i][f], u);
        dsu[u] = u;
        highest[u] = ispeak[u] ? make_pair(height[i][f], 1) : make_pair(-1, 0);
        // if (ispeak[u]) clog << i << ' ' << f << ' ' << height[i][f] << endl;
    }
    
    sort(order.rbegin(), order.rend());
    
    for (auto i: order) {
        int u = i.yy;
        // clog << "== " << i.xx << ' ' << i.yy << endl;
        for (auto cell: cells[u]) {
            int x = cell.xx, y = cell.yy;
            for (int dx = -1; dx <= 1; ++dx)
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx, ny = y + dy;
                if (nx < 0 or ny < 0 or nx >= n or ny >= m) continue;
                if (height[nx][ny] <= height[x][y]) continue;
                join2(u, comp[nx][ny], i.xx);
            }
        }
    }

    rep(i, highest[findp(comp[0][0])].yy) 
        ans.emplace_back(highest[findp(comp[0][0])].xx, 0);
    sort(ans.rbegin(), ans.rend());
    cout << len(ans) << '\n';
    for (auto i: ans) cout << i.xx << ' ' << (i.xx == ans[0].xx ? 0 : i.yy) << '\n';

    return 0;
}

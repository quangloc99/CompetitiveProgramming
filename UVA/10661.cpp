/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: It is not hard too see that the problem gave us a graph and ask us to solve the graph-coloring problem, or more over,
 * to find the chromatic number. The idea is to search for the solution by bruteforcing until we find the smallest one. 
 * In order to reduce the runtime, we should find a bound, and in this solution, the bound is size of the large clique in the graph.
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
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

#define maxn 22
int n, m;
vector<int> gr[maxn];
int grmask[maxn];

bool is_clique(int mask) {
    rep(i, n) {
        if ((~mask >> i) & 1) continue;
        if ((grmask[i] & mask) != (mask ^ (1 << i))) return false;
    }
    return true;
}

int col[maxn];
bool can_paint(int n_col, int u) {
    if (u >= n) return true;
    int picked = 0;
    for (auto v: gr[u]) 
        picked |= (1 << col[v]);
    rep(i, n_col) {
        if ((picked >> i) & 1) continue;
        col[u] = i;
        if (can_paint(n_col, u + 1))
            return true;
    }
    return false;
}


int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest--) {
        cin >> n >> m;
        rep(i, n) {
            gr[i].clear();
            grmask[i] = 0;
        }
        rep(i, m) {
            char u, v; cin >> u >> v;
            u -= 'A'; v -= 'A';
            grmask[u] |= 1 << v;
            grmask[v] |= 1 << u;
            if (u < v) swap(u, v);
            gr[u].push_back(v);
        }

        int ans = 0;
        rep(i, (1 << n)) {
            if (is_clique(i)) 
                ans = max(ans, __builtin_popcount(i));
        }

        while (!can_paint(ans, 0)) ++ans;
        cout << ans << '\n';
    }

    return 0;
}

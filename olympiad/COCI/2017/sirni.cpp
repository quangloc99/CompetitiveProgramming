/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/COCI17_sirni
 * Official solution: http://hsin.hr/coci/archive/2016_2017/, it is the 6-th contest.
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

// #define rand __rand
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()

template<class T>
using min_priority_queue = priority_queue<T, vector<T>, greater<T>>;

#define maxn 101010
#define maxval ((llong)1e7+10)
int n;
int a[maxn];

bool has_div[maxval] = {0};

int dsu[maxn];
int n_set;
int findp(int u) { return u == dsu[u] ? u : dsu[u] = findp(dsu[u]); }
void join(int u, int v) {
    u = findp(u);
    v = findp(v);
    if (u == v) return ;
    if (rand() & 1) swap(u, v);
    dsu[u] = v;
    --n_set;
}

int lower_b[maxval] = {0};
vector<pair<int, int>> group[maxval];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> a[i];
    sort(a, a + n);
    
    n = (int)(unique(a, a + n) - a);

    int f = 0;
    rep(i, n) {
        for (; f <= a[i]; ++f) lower_b[f] = i;
    }
    for (; f < maxval; ++f) lower_b[f] = n;

    llong cnt = 0;
    rep(i, n) {
        if (has_div[a[i]]) continue;
        for (int k = 1; k * a[i] <= a[n - 1]; ++k) {
            has_div[k * a[i]] = 1;
            int t = lower_b[k * a[i]];
            if (t != n and a[t] == a[i]) ++t;
            if (t == n) break;
            group[a[t] % a[i]].emplace_back(i, t);
            // k = a[t] / a[i];
            ++cnt;
        }
    }

    n_set = n;
    rep1(i, n) dsu[i] = i;

    llong ans = 0;
    for (int cost = 0; cost < maxval and n_set != 1; ++cost) {
        for (auto i: group[cost]) {
            if (n_set == 1) break;
            int src = i.xx, cur = i.yy;
            if (findp(src) != findp(cur)) {
                ans += cost;
                join(src, cur);
            }

            int div = a[cur] / a[src];
            ++cur;
            if (cur == n or a[cur] / a[src] != div) continue;
            group[a[cur] % a[src]].emplace_back(src, cur);
        }
    }
    cout << ans;

    return 0;
}

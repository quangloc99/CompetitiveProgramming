// vim: foldmethod=marker

// Author: Tran Quang Loc (darkkcyan)
// Problem: https://oj.uz/problem/view/APIO17_merchant
// Idea:
// Do binary search the result. Let's call it ans. How to check if ans was able to archive.
// We can see that ans is also the money to spent 1 minute walking such that we end up with 
// non-negative profit. So for our ans, we set each edge to minus its value times ans, and
// check if we have non-negative cycle in graph. This can "almost" done with Bellman-ford's
// algorithm (in my code it is FPSA). The twist is, it can only detect negative cycle. To
// check for zero-sum cycle, we can take all the edges that contributes to our result 
// (that is: d(v) == d(u) + w(u, v), then we take edge u, v). Then we check wheter the graph formed 
// with those edges has a cycle or not.

// Sat Feb 15 23:04:31 MSK 2020: start 
// Sat Feb 15 23:15:57 MSK 2020: done coding
// Sat Feb 15 23:26:12 MSK 2020: ok samples.
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

/*{{{*/
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int debug = 1;
#define DB(...) stringstream CONCAT(ss, __LINE__); CONCAT(ss, __LINE__) << __VA_ARGS__; debug_block CONCAT(dbbl, __LINE__)(CONCAT(ss, __LINE__).str())
#else
int debug = 0;
#define DB(...)
#endif
int __db_level = 0;
#define clog if (debug) cerr << string(__db_level * 2, ' ')
struct debug_block {
    string name;
    debug_block(const string& name_): name(name_) { clog << "{ " << name << endl; ++__db_level; }
    ~debug_block() { --__db_level; clog << "} " << name << endl; }
};
#define deb(...) "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]"
#define debln(...)  clog << "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]" << endl
#define _ << ", " <<
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return out << "(" << p.first _ p.second << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return out << "(" << get<0>(t) _ get<1>(t) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return out << "(" << get<0>(t) _ get<1>(t) _ get<2>(t) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out _ container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }
/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

const int maxn = 111;
const int maxk = 1010;
const llong inf = (llong)1e12;
int n, m, k;
llong buy_cost[maxn][maxk], sell_cost[maxn][maxk];
llong dist[maxn][maxn];
llong profit[maxn][maxn];

llong cur_ans[maxn];
bool in_queue[maxn];
int cnt_vis[maxn];
int dfs_state[maxn];

inline llong edge_dist(int u, int v, llong ans) {
    if (dist[u][v] == 0) return 0;
    return dist[u][v] * min(LLONG_MAX / dist[u][v], ans);
}

bool detect_zero_cycle(int u, llong ans) {
    dfs_state[u] = 1;
    rep1(v, n) {
        if (dfs_state[v] == 2) continue;
        llong new_dist = cur_ans[u] - edge_dist(u, v, ans) + profit[u][v];
        if (new_dist != cur_ans[v]) continue;
        if (dfs_state[v] == 1) return true;
        if (detect_zero_cycle(v, ans)) return true;
    }
    dfs_state[u] = 2;
    return false;
}

bool check(llong ans) {
    rep1(i, n) {
        cur_ans[i] = -inf;
        cnt_vis[i] = 0;
        in_queue[i] = false;
        dfs_state[i] = 0;
    }

    queue<int> qu;
    rep1(i, n) {
        if (cnt_vis[i]) continue;
        for (qu.emplace(i), cur_ans[i] = 0, in_queue[i] = 1; len(qu); qu.pop()) {
            int u = qu.front();
            ++cnt_vis[u];
            in_queue[u] = false;

            if (cnt_vis[u] > n) return true;
            rep1(v, n) {
                llong new_dist = cur_ans[u] - edge_dist(u, v, ans) + profit[u][v];
                if (new_dist <= cur_ans[v]) continue;
                cur_ans[v] = new_dist;
                if (!in_queue[v]) {
                    qu.emplace(v);
                    in_queue[v] = true;
                }
            }
        }

        if (detect_zero_cycle(i, ans)) return true;
    }
    return false;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    rep1(i, n) {
        dist[i][i] = 0;
        for (int f = i + 1; f <= n; ++f)
            dist[i][f] = dist[f][i] = inf;
    }

    rep1(i, n) rep(f, k) {
        cin >> buy_cost[i][f] >> sell_cost[i][f];
    }

    rep(i, m) {
        int u, v; llong d;
        cin >> u >> v >> d;
        dist[u][v] = min(dist[u][v], d);
    }

    rep1(mid, n) rep1(u, n) rep1(v, n) {
        dist[u][v] = min(dist[u][mid] + dist[mid][v], dist[u][v]);
    }

    memset(profit, 0, sizeof(profit));
    rep(thing, k) {
        rep1(u, n) {
            if (buy_cost[u][thing] == -1) continue;
            rep1(v, n) {
                if (sell_cost[v][thing] == -1) continue;
                profit[u][v] = max(profit[u][v], sell_cost[v][thing] - buy_cost[u][thing]);
            }
        }
    }

    rep1(i, n) profit[i][i] = -inf;
    
    llong low = 0, high = 1;
    while (check(high)) {
        low = high;
        high <<= 1;
    }

    while (low < high) {
        llong mid = low + (high - low + 1) / 2;
        if (check(mid)) low = mid;
        else high = mid - 1;
    }
    cout << low << endl;

    return 0;
}

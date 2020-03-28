// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: http://usaco.org/index.php?page=viewproblem2&cpid=698
// Idea:
// - Observersion: swapping 1 subsequence is just swapping a the following pairs:
//   (a[i1], a[j1]), (a[i2], a[j2]), .. (a[ik], a[jk])
//   where i1 < i2 < i3 < ... < ik < jk < j_(k - 1) < ... < j2 < j1
//
// - Just do dp in O(n^4):
//   dp[l][r][beg][end] = the LIS from l to r begins with beg and ends with end.
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

#define CONCAT_(x, y) x##y/*{{{*/
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int __db_level = 0;
bool __db_same_line = false;
#define clog cerr << string(!__db_same_line ? __db_level * 2 : 0, ' ')
struct debug_block {
    function<void()> fn;
    void print_name() { __db_same_line = true; fn(); clog << endl; __db_same_line = false;  }
    debug_block(function<void()> fn_): fn(fn_) { clog << "{ "; print_name(); ++__db_level; }
    ~debug_block() { --__db_level; clog << "} "; print_name(); }
};
#define DB(args...) debug_block CONCAT(dbbl, __LINE__)([=]{ clog << args; })
#define deb(...)  if (1) { (clog << "[" #__VA_ARGS__ "] = [" << __VA_ARGS__) << "]"; if (!__db_same_line) clog << endl; }
#else
#define clog if (0) cerr
#define DB(...)
#define deb(...)
#endif
template<class T>
ostream& operator,(ostream& out, const T& thing) { return out << ", " << thing; }
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return (out << "(" << p.first, p.second) << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return (out << "(" << get<0>(t), get<1>(t)) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return (out << "(" << get<0>(t), get<1>(t), get<2>(t)) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out, container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

const int maxn = 51;
const int inf = 10000000;
int n;
int a[maxn];
int dp[maxn][maxn][maxn][maxn] = {0};

void maximize(int& x, int y) {
    if (x < y) x = y;
}

int main(void) {
#ifndef LOCAL
    freopen("subrev.in", "r", stdin);
    freopen("subrev.out", "w", stdout);
#endif
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> a[i];

    deb($v(a, a + n));

    rep(l, maxn) rep(r, maxn) rep(beg, maxn) rep(end, maxn) 
        dp[l][r][beg][end] = -inf;

    rep(i, n) {
        dp[i][i][a[i]][a[i]] = 1;
    }

    for (int length = 2; length <= n; ++length) {
        rep(l, n - length + 1) {
            int r = l + length - 1;
            DB(""; deb(l, r)); 
            int u = a[l];
            int v = a[r];
            maximize(dp[l][r][min(u, v)][max(u, v)], 2); 
            rep1(beg, maxn - 1) {
                for (int end = beg; end < maxn; ++end) {
                    int prev = dp[l + 1][r - 1][beg][end];
                    // if (length == 2) prev = 0;  
                    maximize(dp[l][r][beg][end], prev);
                    maximize(dp[l][r][beg][end], dp[l + 1][r][beg][end]);
                    maximize(dp[l][r][beg][end], dp[l][r - 1][beg][end]);

                    if (v <= beg and end <= u) 
                        maximize(dp[l][r][v][u], prev + 2);
                    if (v <= beg)
                        maximize(dp[l][r][v][end], prev + 1);
                    if (end <= u)
                        maximize(dp[l][r][beg][u], prev + 1);

                    if (u <= beg)
                        maximize(dp[l][r][u][end], dp[l + 1][r][beg][end] + 1);
                    
                    if (end <= v)
                        maximize(dp[l][r][beg][v], dp[l][r - 1][beg][end] + 1);
                }
            }
            rep1(beg, maxn - 1) {
                for (int end = beg; end < maxn; ++end) {
                    if (dp[l][r][beg][end] <= 0) continue;
                    deb(beg, end, dp[l][r][beg][end]);
                }
            }
        }
    }

    int ans = 0;
    rep1(beg, maxn - 1) 
        for (int end = beg; end < maxn; ++end) 
            maximize(ans, dp[0][n - 1][beg][end]);
    cout << ans;

    return 0;
}

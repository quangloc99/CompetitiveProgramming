// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://csacademy.com/contest/ioi-2016-training-round-1/task/polygon_partition
// This code is exact the same as in the editorial.
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

const llong mod = (llong)1e9 + 7;
const int maxn = 200;

inline bool intersected(int u, int v, int x, int y) {
    if (u == x and v == y) return true;
    if (u == x or v == y) return false;
    return (u < x and x < v) != (u < y and y < v);
}

template<bool do_log = false>
llong brute(int n, int k) {
    set<set<pair<int, int>>> slices;

    set<pair<int, int>> cur_slice;
    function<void(int)> backtrack = [&](int i) -> void {
        if (i == k - 1) {
            slices.insert(cur_slice);
            return ;
        }
        rep(u, n) for (int v = u + 2; v < n; ++v) {
            if (u == 0 and v == n - 1) continue;
            bool ok = true;
            for (auto [x, y]: cur_slice) {
                if (intersected(u, v, x, y)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            // deb(i, u, v); 
            cur_slice.emplace(u, v);
            backtrack(i + 1);
            cur_slice.erase({u, v});
        }
    };
    backtrack(0);
    if (do_log) {
        for (auto i: slices) 
            deb($v(i));
    }
    return len(slices);
}

llong mpow(llong base, llong exp) {
    llong ans = 1;
    for (; exp > 0; exp >>= 1) {
        if (exp & 1) (ans *= base) %= mod;
        (base *= base) %= mod;
    }
    return ans;
}

llong dp[maxn][maxn];
llong cal(int n, int k);

llong cal_exclude1(int n, int k) {
    if (n == 3 and k == 0) return 1;
    return (cal(n - 1, k) + cal(n - 1, k - 1)) % mod;
}

llong cal_(int n, int k) {
    if (k >= n - 2) return 0;
    if (k == 0) return 1;
    if (n < 3 or k < 0) return 0;
    llong& ans = dp[n][k];
    if (ans != -1) return ans;
    ans = cal_exclude1(n, k);
    for (int lcnt = 3; lcnt <= n - 1; ++lcnt) {
        int rcnt = n - lcnt + 2;
        rep(l_lines, k) {
            int r_lines = k - 1 - l_lines;
            llong u = cal(lcnt, l_lines);
            llong v = cal_exclude1(rcnt, r_lines);
            deb(lcnt, rcnt, l_lines, r_lines, u, v);
            (ans += u * v) %= mod;
        }
    }
    return ans;
}

llong cal(int n, int k) {
    DB(""; deb(n, k));
    llong ans = cal_(n, k);
    deb(ans);
    return ans;
}


int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, k;
    cin >> n >> k; 
    
    memset(dp, -1, sizeof(dp));
    cout << cal(n, k - 1);

    return 0;
}

// vim: foldmethod=marker
/**
 * BEFORE READING: ignore lines with DB() and debln(), since they are just for debugging.
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://csacademy.com/contest/junior-challenge-2017-day-2/task/ultimateorbs
 * See the editorial.
 */
// Sat Jan 11 18:30:53 MSK 2020: START coding
// Sat Jan 11 18:38:51 MSK 2020: DONE coding
// Sat Jan 11 18:43:43 MSK 2020: DONE debugging
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

#define maxn 1010101
#define maxlgn 20
int n;
llong d;
llong a[maxn];
llong sum[maxn];
llong* max_st[maxlgn][maxn];

template<typename T> bool cmp_ptr(const T* u, const T* v) { return *u < *v; }

llong get_sum(int l, int r) {
    return sum[r] - sum[l]; 
}

llong* get_max(int l, int r) {
    int l2 = (int)log2(r - l);
    return max(max_st[l2][l], max_st[l2][r - (1 << l2)], cmp_ptr<llong>);
}

// I am assumming that this part is correct but having stack overflow.
// vector<int> ans; 
// void solve(int l = 0, int r = n) { 
    // if (r <= l) return ; 
    // DB(deb(l _ r)); 
    // int mx = (int)(get_max(l, r) - a); 
    // debln(mx); 
    // debln(a[mx]); 
    // if (get_sum(l, mx) + d >= a[mx]) solve(l, mx); 
    // ans.push_back(mx); 
    // if (get_sum(mx + 1, r) + d >= a[mx]) solve(mx + 1, r); 
// } 

vector<int> ans;
void solve() {
    queue<pair<int, int>> qu;
    for (qu.emplace(0, n); len(qu); qu.pop()) {
        auto [l, r] = qu.front();
        if (r <= l) continue;
        int mx = (int)(get_max(l, r) - a); 
        ans.push_back(mx); 
        if (get_sum(l, mx) + d >= a[mx]) qu.emplace(l, mx); 
        if (get_sum(mx + 1, r) + d >= a[mx]) qu.emplace(mx + 1, r); 
    }
    sort(ans.begin(), ans.end());
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> d;
    rep(i, n) cin >> a[i];
    sum[0] = 0;
    rep(i, n) sum[i + 1] = sum[i] + a[i];

    debln($v(a, a + n));

    rep(i, n) max_st[0][i] = a + i;
    rep(lv, maxlgn - 1) {
        rep(i, n - (2 << lv) + 1) {
            max_st[lv + 1][i] = max(max_st[lv][i], max_st[lv][i + (1 << lv)], cmp_ptr<llong>);
        }
    }

    solve();
    for (auto i: ans) cout << i + 1 << ' ';

    return 0;
}

// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: http://usaco.org/index.php?page=viewproblem2&cpid=194
// Idea:
// 
// We have n characters, but we actually has n + 1 positions to consider. Let's change an "(" into +1, and
// ")" into -1. For each n + 1 position, we calculate the prefix sum till that position. One of the condition
// that make the substring [l, r] balanced is that the prefix till the position "before" l and the position "after" r
// must be equals. Let's just forget that the prefixsum of till all position between l and r must not less than 
// prefix sum till the endpoints, we can easily calculate how many of them using map. For k strings, we just need 
// to make the key of the map to be a vector.
//
// The solution is obviously go from left to right and use that map to update the result. To remove the position
// before the current position, between which and the current, there is a smaller prefix sum, we just need to use
// the "classic" stack. So firstly, when we add 1 position into map, we maintain a stack, let's call it "added", that
// contains all added position. For each string, we also maintains a stack for matching balanced pair of bracket.
// We just do it like normal, when having "(", we push the position into the stack, but when having ")", not only
// we pop a position out of the stack, but all position in added that after that position must also be removed.
// When there is no position in the string's stack, we set it to -1 and do the above.
//
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

#define CONCAT_(x, y) x##y
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
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }

// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

const int maxk = 11;
const int maxn = 50010;
int k, n;
string s[maxk];
vector<int> val[maxn];

map<vector<int>, int> cnt;
vector<int> stk[maxk];
vector<int> added;

int main(void) {
#ifndef LOCAL
    freopen("cbs.in", "r", stdin);
    freopen("cbs.out", "w", stdout);
#endif
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> k >> n;
    rep(i, k) cin >> s[i];

    val[0] = vector<int>(k);
    rep1(f, n) {
        val[f].resize(k);
        rep(i, k) 
            val[f][i] = val[f - 1][i] + (s[i][f - 1] == '(' ? 1 : -1);
    }

    llong ans = 0;
    rep(i, n + 1) {
        bool next_increasing = i < n, prev_decreasing = i > 0;
        if (next_increasing) rep(f, k) {
            if (s[f][i] == '(') continue;
            next_increasing = false;
            break;
        }

        if (prev_decreasing) rep(f, k) {
            if (s[f][i - 1] == ')') continue;
            prev_decreasing = false;
            break;
        }

        if (prev_decreasing) {
            ans += cnt[val[i]];
        } 

        if (i < n) rep(f, k) {
            if (s[f][i] == '(') {
                stk[f].push_back(i);
            } else {
                int pos = -1;
                if (stk[f].size()) {
                    pos = stk[f].back();
                    stk[f].pop_back();
                }
                while (len(added) and added.back() > pos) {
                    cnt[val[added.back()]]--;
                    added.pop_back();
                }
            }
        }

        if (next_increasing) {
            cnt[val[i]]++;
            added.push_back(i);
        }
    }
    cout << ans;

    return 0;
}

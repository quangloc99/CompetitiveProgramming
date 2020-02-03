// vim: foldmethod=marker
/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://training.olinfo.it/contests/oii2018/tasks/cena/description
 * Solution:
 * We just need to use the constraint "not more than 1000" to solve problem in O(1000S).
 * Because of that constraint, we just need to visit each element of the array s no more than 1000
 * times. 
 * The idea of course: for each position l in the array s, we need to find the minimum r such that
 * we has a valid assign in for the subarray s[l..r]. So my solution is just dp[i][x] = the minimum r
 * mentions above such that we got a valid assignment if we start at the x-th element of the array p
 * that equals to s[i]. Again there is no more than 1000 values of x.
 * To calculate the array fast, I also calculate the array nxt[i][x] = the next position of the array
 * s from i that is equals to x. And finally, I drop the seconds dimension of the dp and just 
 * write the new values on top of the old ones.
 *
 * NOTE: this solution passed with a few optimizations. See the below.
 */
// Mon Feb  3 20:55:09 MSK 2020: START coding
// Mon Feb  3 21:03:15 MSK 2020: DONE coding
// Mon Feb  3 21:06:56 MSK 2020: OK samples
// #pragma GCC optimize ("O3") 
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

#define maxn 100010
#define max_val 100
int nxt_s[max_val][maxn];
int cur_max[maxn];

int pos[max_val][maxn];
int pos_len[max_val];
bool has[max_val];

long long conta(int S, int s[], int P, int p[]) {

    for (int i = 0; i < P; ++i) has[p[i]] = 1;
    vector<int> need_vals;
    need_vals.reserve(100);
    for (int i = 0; i < max_val; ++i) if(has[i]) need_vals.push_back(i);

    for (auto v: need_vals) nxt_s[v][S] = nxt_s[v][S - 1] = S;
    for (int i = S - 1; i-- > 0; ) {
        for (auto x: need_vals) nxt_s[x][i] = nxt_s[x][i + 1];
        nxt_s[s[i + 1]][i] = i + 1;
    }

    cur_max[S] = S;
    memset(pos_len, 0, sizeof(pos_len));
    int last = p[P - 1];
    for (int i = 0; i < S; ++i) {
        pos[s[i]][pos_len[s[i]] ++] = i;
        cur_max[i] = (s[i] == last) ? i : S;
    }

    for (int i = P - 1; i-- > 0; ) {
        int u = p[i],v = p[i + 1];

        int* cur_pos = pos[u];   // without these 2 lines, the solution won't pass. This reduce the number
        int* cur_nxt = nxt_s[v]; // of the indexing operations.

        for (int l = 0; l < pos_len[u]; ++l) {
            int f = cur_pos[l];
            cur_max[f] = cur_max[cur_nxt[f]];
        }
    }

    llong ans = 0;
    int prev = -1;
    rep(i, pos_len[p[0]]) {
        int u = pos[p[0]][i];
        llong a_side = u - prev;
        llong b_side = S - cur_max[u];
        ans += a_side * b_side;
        prev = u;
    } 
    return ans;
}

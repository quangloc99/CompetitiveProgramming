// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://www.infoarena.ro/problema/unique
// Short statement:
//   Given an array. Find the length of a subarray that its contains every number from 1 to the maximum number in it.
//
// Idea:
// - Well if we look at the whole array, if there is no missing value, then we are done, the result it its length.
//   Bug if not, we need to remove some values. And the "must-be-removed" value must be the maximum. So we remove the
//   maximum and repeat this process in the left and the right parts.
// - How to check for missing value you might ask. Well I think that there must be some sort of data structure to do
//   this. But if we don't go top-down, but bottom-up, our data structure become very easy to implement. We just need a
//   data structure that do these things: add an element, count the number of differents element, and remove all elements.
//   The bottom-up algorithm is the following:
//   + If the first half is bigger than the second half, then we calculate the result for the second half first, 
//     then remove all the elements from our data structure. After that we calculate the result for the first
//     half, but now we keep all the elements from its. And finally we re-add all elements of the second half in.
//   + If the second half is bigger, we do the same but the first half first, then second half.
//   + There will be a missing element if the biggest element is not equals to the number of differents values in
//     our ds.
//   This is the well-known "small to large" trick. Or you if see a tree strucutre here, then it is actually DSU on tree.
//   It works in O(nlogn).
// 
// Also the problem has a kind of strict time limit. I have to use fastio. The good thing is all of the number here is
// relatively small.
//
// Fri Feb 21 21:31:31 MSK 2020: start coding;
// Fri Feb 21 21:44:15 MSK 2020: done coding
// Fri Feb 21 22:27:41 MSK 2020: AC
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
#define clog cerr << string(max(__db_level * 2, 0), ' ')
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
ostream& operator<<(ostream& out, const pair<U, V>& p) { return out << "(" << p.first, p.second << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return out << "(" << get<0>(t), get<1>(t) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return out << "(" << get<0>(t), get<1>(t), get<2>(t) << ")"; }
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
inline int read_int() {
    char c;
    while (!isdigit(c = getchar()));
    int ans = c - '0';
    while (isdigit(c = getchar()))
        ans = ans * 10 + c - '0';
    return ans;
}

const int maxn = 100010;
const int lg_maxn = 20;

int n;
int a[maxn];
int l2[maxn];
void cal_l2() {
    l2[1] = 0;
    for (int i = 2; i < maxn; ++i)
        l2[i] = l2[i / 2] + 1;
}

bool cmp_int_ptr(int *u, int *v) { return *u < *v; }

int* sp_table[lg_maxn][maxn];
void build_sp_table() {
    for (int level = 0; (2 << level) <= n; ++level) {
        rep(i, n - (2 << level) + 1) {
            sp_table[level + 1][i] = max(
                    sp_table[level][i], sp_table[level][i + (1 << level)], cmp_int_ptr
            );
        }
    }
}

int* get_max(int l, int r) { // [l, r)
    int level = l2[r - l];
    return max(sp_table[level][l], sp_table[level][r - (1 << level)], cmp_int_ptr);
}

bool has[maxn] = {0};
int having_values[maxn];
int count_values = 0;
void add(int num) {
    if (has[num]) return ;
    has[num] = 1;
    having_values[count_values++] = num;
}

void reset() {
    while (count_values--) has[having_values[count_values]] = false;
    count_values = 0;
}

int process(int l, int r, bool keep) { // [l; r)
    if (l >= r) return 0;
    if (r - l == 1) {
        if (keep) add(a[l]);
        return a[l] == 1;
    }

    int ans = 0;
    int max_pos = (int)(get_max(l, r) - a);
    int left_length = max_pos - l;
    int right_length = r - max_pos - 1;
    if (left_length > right_length) {
        ans = max(ans, process(max_pos + 1, r, false));
        ans = max(ans, process(l, max_pos, true));
        for (int i = max_pos; i < r; ++i) add(a[i]);
    } else {
        ans = max(ans, process(l, max_pos, false));
        ans = max(ans, process(max_pos + 1, r, true));
        for (int i = l; i <= max_pos; ++i) add(a[i]);
    }
    if (a[max_pos] == count_values) ans = r - l;
    if (!keep) {
        reset();
    }
    return ans;
}

int main(void) {
#ifndef LOCAL
    freopen("unique.in", "r", stdin);
    freopen("unique.out", "w", stdout);
#endif
    cal_l2();
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    rep(i, maxn) sp_table[0][i] = i + a;
    int ntest = read_int();
    while (ntest--) {
        n = read_int();
        rep(i, n) {
            a[i] = read_int();
        }
        build_sp_table();
        cout << process(0, n, ntest == 0) << '\n';
    }

    return 0;
}

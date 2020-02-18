// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://dmoj.ca/problem/coci14c7p6
// Ideas:
// - If there is no blank space, just check if original and destination are equals.
// - If there is a less 1 black space, do the following:
// + For each shelf we gather all books from its that both contains in the original and destination shelf.
//   We can notice that if there is 1 blank space in this shelf, then we just need to use the 2nd operations
//   to place books around. We can also see that instead of separately pick and placing books, we can just
//   remove some of the books and place them back in the right order. Because of the, the rest of the books must
//   have the right relative order at the destination. So we need to find maximum number of UNREMOVED books.
//   This part is also simple: we just number the books by its position in the destination and find the LIS.
//   Also if there is no blank space in the shelf (and no books that need to be moved to other shelf), then
//   we also need to increase our result by 1 to borrow 1 blank space.
//
// + For the books that must be moved to another shelf, for each pair of shelves (u, v), we can make an edge
//   from u to v if we need to move some book from u to v. The edge can have weight, but that not important.
//   First, our answer for this case must be equals to the number of books that need to be moved. Then 
//   there is also some cost in order to borrow blank space if there is no space to move book in one connected
//   component. This number is equals to the number of (not strongly) connected comonents, that has more than 1 nodes
//   and there is no blank space. I can proof that if there is a least 1 blank space in the component, then it is possible
//   to move books around without borrowing space by induction, but you can try yourself.
//
// Tue Feb 18 19:32:10 MSK 2020: START coding
// Tue Feb 18 19:44:56 MSK 2020: DONE coding (question mark)???
// Tue Feb 18 20:10:07 MSK 2020: PAUSE
// Tue Feb 18 20:13:22 MSK 2020: CONTINUE
// Tue Feb 18 20:24:07 MSK 2020: AC
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

int find_lis(const vector<int>& a) {
    vector<int> lowest_tail;
    for (auto i: a) {  
        int pos = (int)(upper_bound(lowest_tail.begin(), lowest_tail.end(), i) - lowest_tail.begin());
        if (len(lowest_tail) == pos) lowest_tail.push_back(i);
        else lowest_tail[pos] = min(lowest_tail[pos], i);
    }
    return len(lowest_tail);
}

const int maxn = 1024;
const int maxk = maxn * maxn;
int n, m;
int shelf[maxk], pos[maxk];

vector<int> org_row[maxn];
bool row_has_zero[maxn] = {0};

int dsu[maxn], cnt[maxn];
int find_set(int u) { return dsu[u] == u ? u : dsu[u] = find_set(dsu[u]); }
void join(int u, int v) {
    if (rand() & 1) swap(u, v);
    u = find_set(u), v = find_set(v);
    if (u == v) return ;
    dsu[u] = v;
    cnt[v] += cnt[u];
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    bool has_zero = false;
    rep(i, n) rep(f, m) {
        int u; cin >> u;
        if (u == 0) {
            has_zero = true;
            continue;
        }
        shelf[u] = i;
        pos[u] = f;
    }

    if (!has_zero) {
        rep(i, n) rep(f, m) {
            int u; cin >> u;
            if (shelf[u] != i or pos[u] != f) {
                cout << -1;
                return 0;
            }
        }
        cout << 0 << endl;
        return 0;
    }

    llong ans = 0;
    rep(i, n) {
        dsu[i] = i;
        cnt[i] = 1;
    }
    rep(i, n) rep(f, m) {
        int u; cin >> u;
        if (u == 0) {
            row_has_zero[i] = 1;
            continue;
        }

        if (shelf[u] == i) org_row[i].push_back(pos[u]);
        else {
            ++ans;
            debln(i _ shelf[u]);
            join(i, shelf[u]);
        }
    }

    rep(i, n) {  
        if (row_has_zero[i])   
            row_has_zero[find_set(i)] = 1;  
    }  

    rep(i, n) {  
        if (i != find_set(i)) continue;  
        if (!row_has_zero[i] and cnt[i] > 1) ++ans;    
    }  

    rep(i, n) {
        int lis = find_lis(org_row[i]);
        if (lis == len(org_row[i])) continue;
        ans += len(org_row[i]) - lis;
        if (len(org_row[i]) == m) ++ans; 
    }
    cout << ans;

    return 0;
}

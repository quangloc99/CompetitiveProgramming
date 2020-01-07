// vim: foldmethod=marker
/**
 * BEFORE READING THE CODE: ignore the line with DB() and debln since they are just for debugging.
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.spoj.com/problems/BACKUP/
 * Editorial: http://apio-olympiad.org/2007/
 * This solution is the same as the editorial's.
 *
 * Also here is my explaination to handle the part when we pick the left/right most
 * link, since I find it difficult to understand at first and it is not really clear
 * in the editorial.
 *
 * - Let's number the current links 1, 2, 3, 4, 5, ....
 * - Suppose we picked the link 1. Then after this if we are trying to pick the link 2
 *   and remove the link 1 (as in the editorial, we tried to picked the neighbors
 *   then we remove the middle). But since the link 1 has only 1 neighbors, then
 *   picking link 2 is not benifical, since the cost is worst but the number of link is
 *   not increased. So after picking link 1, we just need to merge is with link 2 and leave them.
 * - But what about the case where we pick link 2 then link 4, .... then this is the part
 *   where we try to pick link 3. If we pick link 3, then we merge it with link 4 and the
 *   virtual link (1, 2). This time when we pick the virtual link (1, 2, 3, 4), we actually pick 
 *   link 2 and 4, and the number of links does increase.
 */

// Tue Jan  7 16:55:46 MSK 2020: START coding
// Tue Jan  7 17:08:56 MSK 2020: DONE coding
// Tue Jan  7 17:16:29 MSK 2020: OK sample
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

#define maxn 101010

struct link {
    int beg, end;
    llong cost;
    link() {}
    link(int b, int e, llong c): beg(b), end(e), cost(c) {}
};

ostream& operator<<(ostream& out, const link& l) {
    return out << "link(" << l.beg _ l.end _ l.cost << ")";
}

bool operator<(const link& lhs, const link& rhs) {
    if (lhs.cost == rhs.cost) return lhs.beg < rhs.beg;
    return lhs.cost < rhs.cost;
}
int n, k;
llong a[maxn];

link link_beg[maxn], link_end[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    while (ntest--) {
        cin >> n >> k;
        rep1(i, n) cin >> a[i];

        set<link> prio;
        rep1(i, n - 1) {
            auto it = prio.emplace(i, i + 1, a[i + 1] - a[i]).first;
            link_beg[i] = link_end[i + 1] = *it;
        }
        link_beg[0] = link_end[1] = link(0, 1, 0);
        link_beg[n] = link_end[n + 1] = link(n, n + 1, 0);

        llong ans = 0;
        while (k--) {
            DB(deb(k));
            int beg = prio.begin()->beg, end = prio.begin()->end;
            llong cost = prio.begin()->cost;
            debln(*prio.begin());
            debln(a[beg]);
            debln(a[end]);
            prio.erase(prio.begin());

            ans += cost;
            
            auto u = link_end[beg], v = link_beg[end];
            
            llong new_cost = u.cost + v.cost - cost;
            prio.erase(u); prio.erase(v);
            link new_link(u.beg, v.end, new_cost);
            link_beg[new_link.beg] = link_end[new_link.end] = new_link;
            if (new_link.beg >= 1 and new_link.end <= n)
                prio.insert(new_link);
            debln(ans);
        }
        cout << ans << '\n';
    }

    return 0;
}

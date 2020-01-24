// vim: foldmethod=marker
/**
 * IGNORE LINES WITH "DB()" AND "debln()", they are just for debugging.
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: http://usaco.org/index.php?page=viewproblem2&cpid=211
 * Solution and testdata: http://usaco.org/index.php?page=dec12problems
 *
 * Well this solution uses the idea in the editorial.
 */
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

struct majority_state {
    int value = 0;
    int count = 0;

    void accept(int new_value) {
        if (count == 0) value = new_value;
        if (value == new_value) ++count;
        else --count;
    }

    friend bool operator==(const majority_state& u, const majority_state& v) {
        return u.value == v.value and u.count == v.count;
    }

    friend ostream& operator<<(ostream& out, const majority_state& u) {
        return out << "{" << u.value _ u.count << "}";
    }
};

majority_state max_cows(vector<int> gangs, majority_state cur_state = majority_state()) {
    DB("find max cows");
    debln(gangs);
    debln(cur_state);
    auto comp = [&](int u, int v) { return gangs[u] == gangs[v] ? u < v : gangs[u] < gangs[v]; };
    set<int, decltype(comp)> prio(comp);

    rep1(i, len(gangs) - 1) if (gangs[i])
        prio.insert(i);

    while (len(prio)) {
        int last = *prio.rbegin();
        prio.erase(--prio.end());
        --gangs[last];
        cur_state.accept(last);
        if (gangs[last]) prio.insert(last);

        debln(last);
        debln(gangs);
        debln(cur_state);
    }
    rep(i, gangs[0]) cur_state.accept(0);
    return cur_state;
}

int main(void) {
#ifndef LOCAL_DEBUG
    freopen("gangs.in", "r", stdin);
    freopen("gangs.out", "w", stdout);
#endif
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, m; cin >> n >> m;
    vector<int> gangs(m);
    for (auto &i: gangs) cin >> i;

    auto max_state = max_cows(gangs);
    debln(max_state);

    if (max_state.value != 0 or max_state.count < 1) {
        cout << "NO";
        return 0;
    }

    cout << "YES\n" << max_state.count << '\n';

    majority_state cur_state;
    rep(i, n) {
        DB(i);
        auto saved_state = cur_state;
        rep(gang_num, m) {
            if (gangs[gang_num] == 0) continue;
            gangs[gang_num]--;
            cur_state.accept(gang_num);

            auto new_state = max_cows(gangs, cur_state);
            debln(gang_num _ new_state _ max_state);
            if (new_state == max_state) {
                cout << gang_num + 1 << '\n';
                break;
            }
            ++gangs[gang_num];
            cur_state = saved_state;
        }
    }
    return 0;
}

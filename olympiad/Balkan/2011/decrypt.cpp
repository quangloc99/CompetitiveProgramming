// vim: foldmethod=marker
/**
 * IGNORE LINES WITH DB() and debln() since they are just for debugging.
 *
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://dunjudge.me/analysis/problems/573/
 * Solution: 
 * It is easy to notice that the array R has period of 6.
 * It is also easy to deduct the numbers when there is collision. After we find out the first 3 numbers
 * of R, the finding part was straight forward. 
 * To quickly find collision, just send random queries. The editorial said that there will no more than
 * 32 queries to have collision, but sadly without a proof.
 */
#include <bits/stdc++.h>
#include "decrypt.h"

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

struct query_t {
    int inp, r_mask, out;
    query_t() {}
    query_t(int i, int m, int o): inp(i), r_mask(m), out(o) {}
};

ostream& operator<< (ostream& out, const query_t& qr) {
    return out << "query_t(inp = " << qr.inp _ "r_mask = " << qr.r_mask _ "out = " << qr.out << ")";
}

int r_mask[3];
query_t do_ask(int input) {
    static int cnt = 0;
    DB("ASK " << cnt++);
    query_t ans(input, r_mask[0], query(input));
    debln(ans);
    int new_mask = r_mask[0] ^ r_mask[1];
    r_mask[0] = r_mask[1];
    r_mask[1] = r_mask[2];
    r_mask[2] = new_mask;
    return ans;
}

void decrypt() {
    rep(i, 3) r_mask[i] = 1 << i;

    set<int> unknown_input;
    rep(i, 256) unknown_input.insert(i);
    vector<query_t> asked_by_r[8];
    vector<vector<query_t>> asked_by_output(256);

    vector<int> extended_r(8, -1);
	vector<int> M(256, -1), rev_M(256, -1);

    function<void(int, int)> process_mask;

    auto assign = [&](int inp, int out) {
        if (M[inp] != -1) return ;
        DB("assign");
        debln(inp _ out);
        M[inp] = out;
        rev_M[out] = inp;
        unknown_input.erase(inp);

        for (auto qr: asked_by_output[out]) {
            process_mask(qr.r_mask, inp ^ qr.inp);
        }
    };

    process_mask = [&](int mask, int r) {
        DB("process " << bitset<3>(mask) _ r);
        if (extended_r[mask] != -1) return ;
        extended_r[mask] = r;
        int mask_val = extended_r[mask];
        for (auto qr: asked_by_r[mask]) {
            debln(qr);
            assign(qr.inp ^ mask_val, qr.out);
        }
        asked_by_r[mask].clear();
        rep(i, 8) {
            if (extended_r[i] == -1 or i == mask) continue;
            process_mask(i ^ mask, extended_r[i] ^ extended_r[mask]);
        }
    };
    
    while (unknown_input.size() or extended_r[1] == -1 or extended_r[2] == -1 or extended_r[4] == -1) {
        debln(len(unknown_input));
        int cur_mask = r_mask[0];
        DB("Loop " << bitset<3>(cur_mask));
        int cur_inp;
        if (extended_r[cur_mask] == -1) {
            cur_inp = rand(256);
        } else {
            cur_inp = *unknown_input.begin() ^ extended_r[cur_mask];
        }
        auto qr = do_ask(cur_inp);
        asked_by_output[qr.out].push_back(qr);

        if (extended_r[cur_mask] == -1) {
            asked_by_r[cur_mask].emplace_back(qr);
            debln(rev_M[qr.out]);
            if (rev_M[qr.out] != -1) {
                clog << "rec " << endl;
                process_mask(cur_mask, rev_M[qr.out] ^ qr.inp);
            }
        } else {
            assign(qr.inp ^ extended_r[qr.r_mask], qr.out);
        }
        debln(len(asked_by_output[qr.out]));
        for (auto asked_qr: asked_by_output[qr.out]) {
            if (asked_qr.r_mask == qr.r_mask) continue;
            int u = asked_qr.r_mask ^ qr.r_mask;
            int v = asked_qr.inp ^ qr.inp;
            process_mask(u, v);
        }
    }

	int R[3] = {extended_r[1], extended_r[2], extended_r[4]};

	solution(R, &M[0]);
	return;
}

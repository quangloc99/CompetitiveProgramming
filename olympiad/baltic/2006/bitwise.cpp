// vim: foldmethod=marker

// BEFORE READIND THE CODE: ignore though lines with DB and debln, they are just for debugging.
/**
 *
 * Author: Tran Quang Loc (darkkcyan)
 * Problem `BITWISE` in here: https://cses.fi/109/list/
 * Editorial can be found here:
 * https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/Baltic/official/boi2006_solutions/bitwise_sol.pdf
 *
 * This solution is somewhat the same as the editorial. If you find the editorial is not clear
 * please see the code with the comment below, since explaining here will be lengthy.
 */
// Wed Jan  8 12:13:25 MSK 2020: START coding
// Wed Jan  8 12:29:08 MSK 2020: DONE coding
// Wed Jan  8 12:45:19 MSK 2020: AC
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

const int max_bit = 40;
int n, p;
vector<vector<pair<llong, llong>>> expr;

bool ok(vector<pair<llong, llong>> group, llong mask) {
    for (int cur_bit = max_bit; cur_bit--; ) {
#ifdef LOCAL_DEBUG
        debug = cur_bit <= 6;
#endif
        bool pick = (mask >> cur_bit) & 1;
        DB(deb(cur_bit _ pick));
        pair<llong, llong>* has_choice = 0;

        int cnt[2] = {0, 0};
        int cnt_has_choice = 0;

        for (auto& i: group) {
            DB(deb(i));
            llong lower_bit = (i.first >> cur_bit) & 1ll;
            llong upper_bit = (i.second >> cur_bit) & 1ll;
            debln(lower_bit);
            debln(upper_bit);
            if (lower_bit != upper_bit) {
                has_choice = &i;
                ++cnt_has_choice;
            }
            else cnt[upper_bit]++;
        }
        debln(cnt[0] _ cnt[1] _ cnt_has_choice);
        if (cnt_has_choice) debln(*has_choice);

        if (pick) {
            if (cnt_has_choice >= 2) return true;   // in this case, we can just make the first has_choice = 10000...
                                                    // and the second has choice                           = 01111...
                                                    // so OR of the rest will be 1111....
            if (cnt_has_choice == 1) {
                if (cnt[1]) return true;            // same as above, but because we already has 1xxxx,
                                                    // we just need to adjust like the second has_choice  
                                                    
                assert(has_choice);
                has_choice->first = 1ll << cur_bit; // well the fate of the answer is depend on this choice, since
                                                    // it's the only one with ability to make this group has `cur_bit` in the result.
                                                    // After choosing the result we of course need to adjust the range.

            } else {
                if (!cnt[1]) return false;          // this is obvious
            }
        } else {
            if (cnt_has_choice) return true;        // just make 1 of the has_choice equals to 011111....
        }
    }
    return true;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> p;
    rep(i, p) {
        int k;
        cin >> k;
        expr.emplace_back(k);
    }

    {
        auto cur_expr = expr.begin();
        int f = 0;
        rep(i, n) {
            if (f >= (int)cur_expr->size()) {
                f= 0;
                ++cur_expr;
            }
            cin >> cur_expr->at(f).first >> cur_expr->at(f).second;
            ++f;
        }
    }

    llong ans = 0;
    for (int cur_bit = max_bit; cur_bit--; ) {
        DB(deb(cur_bit));
        bool all_ok = true;
        for (auto& i: expr) {
            DB("");
            debln(i);
            if (!ok(i, ans | (1ll << cur_bit))) {
                clog << "fail" << endl;
                all_ok = false;
                break;
            }
            clog << "ok" << endl;
        }
        debln(all_ok);
        if (all_ok) ans |= 1ll << cur_bit;
        debln(bitset<10>(ans));
    }
    cout << ans;

    return 0;
}

// vim: foldmethod=marker
/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/JOI16_selling_rna
 * Solution:
 * - Sort the input strings.
 * - We build the trie for the input string. With trie we can find the lower and upper bound of any string 
 *   faster than binary search.
 * - Let's define F(i, x) be the number of strings in the first i string that ends with x. The result for each query is just
 *   F(upper_bound(prefix), suffix) - F(lower_bound(prefix) - 1, suffix). From here we break the query into 2 parts that has form:
 *   (pos, suffix). After breaking all queries, we sort them increasingly by pos.
 * - Now we make another trie, we just go from i = 1 to n, add reversed(a[i]) into the new trie and then find the result
 *   (the function F) for each pair (pos, suffix) mentioned above.
 *
 * Note: the sorting input strings part in this code is done with sort() function. I also did without it, and I think it is
 * cooler. See my first AC solution with that sorting part here: https://oj.uz/submission/196747
 */
// Thu Jan 16 21:39:21 MSK 2020: START coding
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

const int maxn = 101010;
unordered_map<char, char> char_map = {
    {'A', 0},
    {'C', 1},
    {'G', 2},
    {'U', 3}
};

vector<char> transform_rna(const string s) {
    vector<char> ans(len(s));
    rep(i, len(s)) ans[i] = char_map[s[i]];
    return ans;
}

template<class container>
struct trie {
    trie* child[4];
    int cnt_all;
    trie(): child{0}, cnt_all(0) {}
    ~trie() {
        rep(i, 4) if(child[i]) delete child[i];
    }

    void insert(const container& s) {
        trie* cur = this;
        ++cnt_all;
        for (auto i: s) {
            if (!cur->child[(int)i]) cur->child[(int)i] = new trie();
            cur = cur->child[(int)i];
            ++cur->cnt_all;
        }
    }

    int get_self_cnt() {
        int ans = cnt_all;
        rep(i, 4) if (child[i]) ans -= child[i]->cnt_all;
        if (ans < 0) {
            assert(false);
        }
        return ans;
    }

    // return (lower_bound, cnt)
    pair<int, int> find_bound(const container& s) { 
        int cnt_lower = 0;
        trie* cur = this;
        for (auto i: s) {
            cnt_lower += cur->get_self_cnt();
            rep(f, i) 
                if (cur->child[f])
                    cnt_lower += cur->child[f]->cnt_all;
            if (!cur->child[(int)i]) return {cnt_lower, 0};
            cur = cur->child[(int)i];
        }
        return {cnt_lower, cur->cnt_all};
    }
};

using trie_char = trie<vector<char>>;

int n, m;
vector<char> a[maxn];

struct query {
    int id;
    int pos;
    int sign;
    vector<char> what;
    query(int i, int p, int s, const vector<char>& w): id(i), pos(p), sign(s), what(w) {}
};

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;

    trie_char root;
    rep(i, n) {
        string s; cin >> s;
        a[i] = transform_rna(s);
        root.insert(a[i]);
    }

    sort(a, a + n);

    vector<query> all_query;

    rep(q, m) {
        string pref, suff; cin >> pref >> suff;
        auto tr_pref = transform_rna(pref), tr_suff = transform_rna(suff);
        reverse(tr_suff.begin(), tr_suff.end());

        auto bound = root.find_bound(tr_pref);
        all_query.emplace_back(q, bound.first, -1, tr_suff);
        all_query.emplace_back(q, bound.first + bound.second, 1, tr_suff);
    }

    sort(all_query.begin(), all_query.end(), [](const auto& q1, const auto&q2) { return q1.pos < q2.pos; });

    int qr = 0;
    while (qr < len(all_query) and all_query[qr].pos == 0) ++qr;

    trie_char quering_root;
    vector<int> ans(m);

    rep1(i, n) {
        reverse(a[i - 1].begin(), a[i - 1].end());
        quering_root.insert(a[i - 1]);

        for (; qr < len(all_query) and all_query[qr].pos == i; ++qr) {
            ans[all_query[qr].id] += all_query[qr].sign * quering_root.find_bound(all_query[qr].what).second;
        }
    }

    rep(i, m) cout << ans[i] << '\n';

    return 0;
}

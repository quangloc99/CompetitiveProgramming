// vim: foldmethod=marker

// Author: Tran Quang Loc (darkkcyan)
// Problem: https://www.infoarena.ro/problema/sabin
// Shorten statement (you can just use google translate thought).
//   You are given n book shelves. Each of them contains k books.
//   You are also given 1 more shelf with m books on it.
//   Every books in n + 1 shelves contains title with length p.
//
//   You must answer q queries. For each queries you are given number x and a list of k books
//   you must pick from the (n + 1)th shelf. You need to count how many shelves of the first n
//   that has the value "described in the statement" that has value x.
//
// Idea:
// If k = 1, then we can just use trie to solve the problem.
// For k > 1, we can use the really similar solution. Let's define a hyper-word is a word
// that each character is made up of string, and let's also call them hyper-character.
// For each of the first n shelves we can create n hyper-words, the i-th hyper-character 
// of which is made of the i-th character of the books' title, and arrange them in the order
// they appeared in the shelf. Then we build the trie with these hyper words.
// The queries is now similer to the case k = 1, we just create a new hyper word for each
// query and do the query with it.
//
// Also I planed to also use normal trie to store the children of the hyper-words trie 
// instead of unorderd map, but that way ends up MLE and also TLE. Using unordered_map is
// actually effective.
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

const int maxm = 30101;

struct hyper_english_char_trie {
    unordered_map<string, hyper_english_char_trie*> children;

    int cnt;
    hyper_english_char_trie() : cnt(0) {}
    ~hyper_english_char_trie() {
        for (auto& it: children) delete it.second;
    }

    void insert(const vector<string>& hyper_word) {
        auto root = this;
        for (auto hyper_char: hyper_word) {
            if (!root->children.count(hyper_char))
                root->children[hyper_char] = new hyper_english_char_trie();
            ++root->cnt;
            root = root->children[hyper_char];
        }
        ++root->cnt;
    }

    int count_prefix(int length, const vector<string>& hyper_word) {
        auto root = this;
        for (auto hyper_char: hyper_word) {
            if (length-- == 0) {
                int ans = root->cnt;
                if (root->children.count(hyper_char))
                    ans -= root->children[hyper_char]->cnt;
                return ans;
            }
            if (!root->children.count(hyper_char))
                return 0;
            root = root->children[hyper_char];
        }
        return root->cnt;
    }
};

int n, k, m, p, q;
string shelf[maxm];

int main(void) {
#ifndef LOCAL
    freopen("sabin.in", "r", stdin);
    freopen("sabin.out", "w", stdout);
#endif
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> k >> m >> p >> q;

    hyper_english_char_trie trie;

    rep(i, n) {
        vector<string> hyper_word(p);
        rep(cur_word, k) {
            string word; cin >> word;
            rep(f, p) hyper_word[f] += word[f];
        }
        trie.insert(hyper_word);
    }

    rep1(i, m) cin >> shelf[i];
    rep1(query, q) {
        int num; cin >> num;
        vector<string> hyper_word(p);
        rep(cur_word, k) {
            int word_pos; cin >> word_pos;
            string& word = shelf[word_pos];
            rep(f, p) hyper_word[f] += word[f];
        }
        cout << trie.count_prefix(num, hyper_word) << '\n';
    }

    return 0;
}

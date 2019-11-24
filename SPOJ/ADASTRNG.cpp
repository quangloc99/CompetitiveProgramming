/** 
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.spoj.com/problems/ADASTRNG/
 */
#include <bits/stdc++.h>
#define clog if (0) cout
using namespace std;
using namespace std::placeholders;

inline void counting_sort_pos(vector<int>& pos, function<int(int)> get_key, size_t alphabet) {
    vector<int> cnt(alphabet), old_pos = pos;
    for (auto i: pos) cnt[get_key(i)]++;
    for (int i = 1; i < (int)alphabet; ++i) cnt[i] += cnt[i - 1];
    for (int i = (int)pos.size(); i--; ) pos[--cnt[get_key(old_pos[i])]] = old_pos[i];
}

template<class container, size_t alphabet = 256>
vector<int> construct_suffarr(const container& s) {
    vector<int> pos(s.size()), rank(s.size());
    for (int i = (int)s.size(); i--; ) {
        pos[i] = i;
        rank[i] = s[i];
    }
    for (int gap = 1; gap < (int)s.size(); gap <<= 1) {
        vector<pair<int, int>> keys(s.size());
        for (int i = 0; i < (int)s.size(); ++i)
            keys[i] = {rank[i], rank[(i + gap) % s.size()]};
        counting_sort_pos(pos, [&](int i) { return keys[i].second; }, max(alphabet, s.size()));
        counting_sort_pos(pos, [&](int i) { return keys[i].first; }, max(alphabet, s.size()));
        rank[pos[0]] = 0;
        for (int i = 1; i < (int)s.size(); ++i)
            rank[pos[i]] = rank[pos[i - 1]] + (keys[pos[i - 1]] != keys[pos[i]]);
    }
    return pos;
}

template<class container>
vector<int> construct_lcp(const container& s, const vector<int> sa) {
    int n = (int)s.size();
    vector<int> lcp(n - 1), inv(n);
    for (int i = n; i--; ) inv[sa[i]] = i;

    for (int i = 0, k = 0; i < n; ++i) {
        if (inv[i] == n - 1) {
            k = 0;
            continue;
        }
        for (int f = sa[inv[i] + 1]; i + k < n and f + k < n and s[i + k] == s[f + k]; ++k);
        lcp[inv[i]] = k;
        if (k) --k;
    }
    return lcp;
}

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    string s; cin >> s;

    s += '$';
    auto sa = construct_suffarr(s);
    auto lcp = construct_lcp(s, sa);

    llong cur_ans = len(s) - 1 - sa[1];
    char prev = 'a' - 1;
    for (int i = 2; i < len(s); ++i) {
        if (s[sa[i]] == s[sa[i - 1]]) cur_ans += len(s) - 1 - sa[i] - lcp[i - 1];
        else {
            for (char f = char(prev + 1); f < s[sa[i - 1]]; ++f) cout << "0 ";
            cout << cur_ans << ' ';
            cur_ans = len(s) - 1 - sa[i];
            prev = s[sa[i - 1]];
        }
    }
    for (char f = char(prev + 1); f < s[sa.back()]; ++f) cout << "0 ";
    cout << cur_ans << ' ';
    for (char f = char(s[sa.back()] + 1); f <= 'z'; ++f) cout << "0 ";


    return 0;
}

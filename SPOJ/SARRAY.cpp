/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.spoj.com/problems/SARRAY/
 */
#include <bits/stdc++.h>
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
    auto sa = construct_suffarr(s + "$");
    for (auto i: sa) {
        if (i == len(s)) continue;
        cout << i  << '\n';
    }

    return 0;
}

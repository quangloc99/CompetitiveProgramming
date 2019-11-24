/**
 * Author: Tran Quang Loc (darkkcyan)
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/SPOJ/SARRAY.cpp
 *        and  https://github.com/quangloc99/CompetitiveProgramming/blob/master/SPOJ/ADASTRNG.cpp
 */
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

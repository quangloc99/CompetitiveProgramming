/**
 * Author: Tran Quang Loc (darkkcyan)
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/SPOJ/NAJPF_z-function.cpp
 */
template<class container> vector<int> z_function(const container& s) {
    vector<int> z(s.size());
    for (int i = 1, l = 0, r = 0; i < (int)s.size(); ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] <= (int)s.size() and s[i + z[i]] == s[z[i]]) ++z[i];
        if (i + z[i] - 1 > r) 
            l = i, r = i + z[i] - 1;
    }
    return z;
}

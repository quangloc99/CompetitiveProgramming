/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://contest.yandex.ru/ioi/contest/562/problems/B/
 * Editorial: https://ioinformatics.org/page/ioi-2006/32
 * This solution is similar to the editorial.
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

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

#define maxn 1010
int n, m, py_w, py_h, champ_w, champ_h;
int a[maxn][maxn];
int pref_sum[maxn][maxn] = {0};
deque<int> comp_qu[maxn];

int get_sum(int end_r, int end_c, int w, int h) {
    return pref_sum[end_r][end_c] - pref_sum[end_r - h][end_c] - pref_sum[end_r][end_c - w] + pref_sum[end_r - h][end_c - w];
}

#define champ_sum(end_r, end_c) get_sum(end_r, end_c, champ_w, champ_h)
#define py_sum(end_r, end_c) get_sum(end_r, end_c, py_w, py_h)

struct result {
    int ans;
    int py_end_r, py_end_c, champ_end_r, champ_end_c;
    result() : ans(-1) {}
    result(int pr, int pc, int cr, int cc) : py_end_r(pr), py_end_c(pc), champ_end_r(cr), champ_end_c(cc) {
        ans = py_sum(pr, pc) - champ_sum(cr, cc);
    }
    friend bool operator<(const result& lhs, const result& rhs) {
        return lhs.ans < rhs.ans;
    }
};

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> m >> n >> py_w >> py_h >> champ_w >> champ_h;
    rep(i, n) rep(f, m) cin >> a[i][f];

    rep(i, n) rep(f, m)
        pref_sum[i + 1][f + 1] = pref_sum[i][f + 1] + pref_sum[i + 1][f] - pref_sum[i][f] + a[i][f];

    result ans ;

    for (int end_r = 1 + champ_h; end_r < n; ++end_r) {
        deque<pair<int, int>> all_qu;
        for (int end_c = 1 + champ_w; end_c < m; ++ end_c) {
            while (comp_qu[end_c].size() and comp_qu[end_c].front() < end_r + 1 - py_h + 1 + champ_h)
                comp_qu[end_c].pop_front();
            while (comp_qu[end_c].size() and champ_sum(comp_qu[end_c].back(), end_c) > champ_sum(end_r, end_c))
                comp_qu[end_c].pop_back();
            comp_qu[end_c].push_back(end_r);


            if (end_r + 1 < py_h) continue;
            while (all_qu.size() and all_qu.front().yy < end_c + 1 - py_w + 1 + champ_w)
                all_qu.pop_front();
            while (all_qu.size() and champ_sum(all_qu.back().xx, all_qu.back().yy) > champ_sum(comp_qu[end_c].front(), end_c))
                all_qu.pop_back();
            all_qu.emplace_back(comp_qu[end_c].front(), end_c);

            if (end_c + 1 < py_w) continue;
            result temp_ans(end_r + 1, end_c + 1, all_qu.front().xx, all_qu.front().yy);
            ans = max(ans, temp_ans);
        }
    }
    cout << ans.py_end_c - py_w + 1 << ' ' << ans.py_end_r - py_h + 1 << '\n';
    cout << ans.champ_end_c - champ_w + 1 << ' ' << ans.champ_end_r - champ_h + 1;
    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://dmoj.ca/problem/coci15c3p6
 * Editorial can be found here (contest #3):
 *      https://hsin.hr/coci/archive/2015_2016/
 * This solution is exactly same as the editorial.
 * 
 * Note for implementation:
 * - For selecting the best domino, we should use k-select algorithm
 *   instead of using sort of set (std::nth_element function), since
 *   n = 2000, not 1000.
 */
// Fri Jan  3 21:47:42 MSK 2020: START coding
// Fri Jan  3 21:57:00 MSK 2020: STOP and find a bug in my vim script
// Fri Jan  3 22:02:47 MSK 2020: reread the solution
// Fri Jan  3 22:09:26 MSK 2020: CONTINUE
// Fri Jan  3 22:24:34 MSK 2020: STOP to also reread the solution
// Fri Jan  3 22:26:00 MSK 2020: CONTINUE
// Fri Jan  3 22:39:03 MSK 2020: DONE coding
// Fri Jan  3 23:05:19 MSK 2020: FINISH debugging before submit
// Fri Jan  3 23:14:09 MSK 2020: AC

int debug = 0;
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

#define clog if (debug) cerr
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


#define maxn 2020  // yay this year :))
#define maxm 50    // make sure this number does not cause RTE
#define maxk 10
int n, k;
llong a[maxn][maxn];

enum Dir { hor, ver };
struct Domino {
    int r, c;
    Dir dir;
    Domino() {}
    Domino(int r_, int c_, Dir d) : r(r_), c(c_), dir(d) {}

    array<llong*, 2> get_cells() const {
        return {&a[r][c], dir == hor ? &a[r][c + 1] : &a[r + 1][c] };
    }

    llong get_val() const {
        return a[r][c] + *(get_cells()[1]);
    }
};

bool adj(const Domino& lhs, const Domino& rhs) {
    for (auto u: lhs.get_cells())
        for (auto v: rhs.get_cells())
        if (u == v) return true;
    return false;
}

int m;
llong gr[maxm];
llong node_val[maxm];
void iterate_clique(const vector<int>& node_list, int limit_sz, function<void(llong, llong, int, llong)> callback) {
    function<void(int, llong, llong, int, llong)> iter = [&](int i, llong black_list, llong cur_cliq, int cnt, llong cur_sum) {
        if (cnt > limit_sz) return ;
        callback(cur_cliq, black_list, cnt, cur_sum);
        if (i >= len(node_list)) return ;
        clog << "node " << node_list[i] << endl;

        iter(i + 1, black_list, cur_cliq, cnt, cur_sum);
        int u = node_list[i];
        if ((~black_list >> u) & 1) {
            iter(i + 1, black_list | ~gr[u], cur_cliq | (1ll << u), cnt + 1, cur_sum + node_val[u]);
        }
    };
    iter(0, 0, 0, 0, 0);
}

llong solve() {
    int part_size = min(21, m / 2);
    vector<int> first_half, second_half;
    rep(i, part_size) first_half.push_back(i);
    for (int i = part_size; i < m; ++i) second_half.push_back(i);

    static llong dp[1 << 21][maxk];
    memset(dp, 0, sizeof(dp));
    clog << "===First half" << endl;
    iterate_clique(first_half, k, [&](llong cliq, llong _, int cnt, llong sum) {
        llong cur = cliq & ((1 << part_size) - 1);
        clog << bitset<10>(cur) << ' ' << ' ' << cnt << ' ' << sum << endl;
        dp[cur][cnt] = max(dp[cur][cnt], sum);
    });

    clog << "=== mid processing" << endl;
    rep(i, (1 << part_size)) {
        rep(f, k + 1) {
            for (int cur = i; cur; ) {
                int first = cur & (-cur);
                dp[i][f] = max(dp[i][f], dp[i ^ first][f]);
                cur -= first;
            }
            clog << bitset<10>(i) << ' ' << f << ' ' << dp[i][f] << endl;
        }
    }

    clog << "=== second half" << endl;
    llong ans = 0;
    iterate_clique(second_half, k, [&](llong _, llong black_list, int cnt, llong sum) {
        llong other = ~black_list & ((1 << part_size) - 1);
        clog << bitset<10>(other) << ' ' << ' ' << cnt << ' ' << sum << endl;
        ans = max(ans, dp[other][k - cnt] + sum);
        clog << ans << endl;
    });
    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    // cin >> n >> k; 
    scanf("%d %d", &n, &k);

    llong sum_all = 0;
    rep(i, n) rep(f, n) {
        // cin >> a[i][f]; 
        scanf("%lld", a[i] + f);
        sum_all += a[i][f];
    }

    clog << "== sum_all " << sum_all << endl;

    vector<Domino> dom;
    rep(i, n) rep(f, n) {
        if (f != n - 1) dom.emplace_back(i, f, hor);
        if (i != n - 1) dom.emplace_back(i, f, ver);
    }

    m = min(len(dom), 7 * (k - 1) + 1);
    nth_element(dom.begin(), dom.begin() + m, dom.end(), [&](const Domino& lhs, const Domino& rhs) {
            return lhs.get_val() > rhs.get_val();
    });
    
    memset(gr, 0, sizeof(gr));
    clog << "===Best" << endl;
    rep(i, m) {
        clog << dom[i].r << ' ' << dom[i].c << ' ' << dom[i].dir << ' ' << dom[i].get_val() << endl;
    }
    rep(i, m) node_val[i] = dom[i].get_val();
    rep(i, m) rep(f, m) {
        if (!adj(dom[i], dom[f])) 
            gr[i] |= 1ll << f;
    }

    cout << sum_all - solve();
    return 0;
}

/**
 * Author: Tran Quang Loc
 * Problem: https://dmoj.ca/problem/coci08c4p4
 * Official solution can be found here: http://hsin.hr/coci/archive/2008_2009/, the 4th contest.
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
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

#define maxn 600
#define maxlgn 10
int n;
string a[maxn];

int int_log2(int x) { return x <= 1 ? 0 : 1 + int_log2(x / 2); }

int pref_sum[maxn][maxn];
void cal_pref_sum() {
    rep(i, n) pref_sum[i][0] = 0;
    rep(f, n) pref_sum[0][f] = 0;

    rep(i, n) rep(f, n) {
        pref_sum[i + 1][f + 1] = pref_sum[i][f + 1] + pref_sum[i + 1][f] - pref_sum[i][f] + (a[i][f] == '1');
    }
}

int get_sum(int r, int c, int h, int w) {  // note: height before wide
                                           // but in this problem it is not really importance, since we are calculating for square.
    return pref_sum[r + h][c + w] - pref_sum[r + h][c] - pref_sum[r][c + w] + pref_sum[r][c];
}

string ans[maxn];
void fill(int r, int c, int h, int w, bool color) {
    rep(i, h) rep(f, w)
        ans[r + i][c + f] = char('0' + color);
}

int process(int r, int c, int level) {
    if (level == 0) {
        ans[r][c] = a[r][c];
        return 0;
    }
    int mid_size = 1 << (level - 1);
    int sum_dp = 0;
    map<pair<int, int>, int> dp;
    for (int i: {r, r + mid_size})
    for (int f: {c, c + mid_size}) {
        dp[{i, f}] = process(i, f, level - 1);
        sum_dp += dp[{i, f}];
    }

    tuple<int, short, short, short, short> res = {INT_MAX, 0, 0, 0, 0};

    int quarter_area = mid_size * mid_size;
    for (int u: {r, r + mid_size})
    for (int v: {c, c + mid_size})
    for (int x: {r, r + mid_size})
    for (int y: {c, c + mid_size}) {
        if (u == x and v == y) continue;
        llong s = sum_dp - dp[{u, v}] - dp[{x, y}];
        s += get_sum(u, v, mid_size, mid_size);
        s += quarter_area - get_sum(x, y, mid_size, mid_size);
        res = min(res, {s, u, v, x, y});
    }

    // clog << r << ' ' << c << ' ' << level << ' ' << get<0>(res) << endl;
    fill(get<1>(res), get<2>(res), mid_size, mid_size, 0);
    fill(get<3>(res), get<4>(res), mid_size, mid_size,1);
    return get<0>(res);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> a[i];
    cal_pref_sum();
    int lv = int_log2(n);

    rep(i, n) ans[i] = string(n, ' ');
    cout << process(0, 0, lv) << '\n';

    rep(i, n) cout << ans[i] << '\n';

    return 0;
}

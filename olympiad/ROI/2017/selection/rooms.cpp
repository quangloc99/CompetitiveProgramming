/**
 * Author: Tran Quang Loc (darkkcyan)
 * Like to the problem + online submitting + editorial: https://csacademy.com/contest/romanian-ioi-2017-selection-1/task/rooms/statement/
 * This solution is the same as the editorial, except I use set to count the rooms at the border for shorter code. 
 * So the time complexity for this solution is O(Q * (N + M) * log(N))
 */.
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

#define maxn 2020
int n, m;
string board[maxn];

int color[maxn][maxn];

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, -1, 0, 1};

void fill(int r, int c, int col) {
    char src_char = board[r][c];
    queue<pair<int, int>> qu;
    color[r][c] = col;
    for (qu.push({r, c}); len(qu); qu.pop()) {
        r = qu.front().xx, c = qu.front().yy;
        rep(dir, 4) {
            int nr = r + dr[dir], nc = c + dc[dir];
            if (nr < 0 or nr >= n) continue;
            if (nc < 0 or nc >= m) continue;
            if (src_char != board[nr][nc]) continue;
            if (color[nr][nc] != -1) continue;
            color[nr][nc] = col;
            qu.push({nr, nc});
        }
    }
}

int is_special[maxn][maxn];
pair<int, int> special[maxn * maxn];
int sum[maxn][maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep(i, n) cin >> board[i];

    memset(color, -1, sizeof(color));
    memset(is_special, 0, sizeof(is_special));
    memset(sum, 0, sizeof(sum));
    int cur_color = 0;
    rep(i, n) rep(f, m) {
        if (color[i][f] != -1) continue;
        fill(i, f, cur_color);
        is_special[i][f] = 1;
        special[cur_color] = {i, f};
        ++cur_color;
    }

    rep(i, n) rep(f, m) {
        sum[i + 1][f + 1] = is_special[i][f] + sum[i][f + 1] + sum[i + 1][f] - sum[i][f];
    }

    int q; cin >> q;
    while (q--) {
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        --r1, --r2, --c1, --c2;
        if (r1 > r2) swap(r1, r2);
        if (c1 > c2) swap(c1, c2);

        set<int> at_border;

        auto check = [&](int r, int c) {
            int col = color[r][c];
            if (special[col].xx >= r1 and special[col].xx <= r2 and special[col].yy >= c1 and special[col].yy <= c2) return;
            at_border.insert(col);
        };
        for (int r = r1; r <= r2; ++r) for (int c: {c1, c2}) check(r, c);
        for (int c = c1; c <= c2; ++c) for (int r: {r1, r2}) check(r, c);
        int ans = sum[r2 + 1][c2 + 1] - sum[r1][c2 + 1] - sum[r2 + 1][c1] + sum[r1][c1] + len(at_border);
        cout << ans << '\n';
    }

    return 0;
}

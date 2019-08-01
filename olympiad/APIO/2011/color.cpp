/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://tioj.ck.tp.edu.tw/problems/1748
 * English statement: http://apio-olympiad.org/2011/
 * Idea:
 * 1) Let's say each cell's value is 0 or 1 as in the statement. We just
 *   need to write the rule in the statement in different way:
 *   + 4 cells in a square with values a, b, c and d must satisfy:
 *     a xor b xor c xor d = 1
 * 2) Using this new rule, we can actually find xor of values of the corners in
 *   each rectangle by xor-ing all the small squares in that rectangle together.
 * 3) Another observation is that, if we have 1 row and 1 col fully filled, 
 *   we can fill the other cells in the board.
 * 4) In my solution, I try to find a way to fill the first row and first column.
 *   We create a graph with n + m - 1 nodes, each node represents a cell in
 *   either first row or column. Using the 2) rule, we can find the relationship
 *   between the nodes in the first row or the first column. To make the relationship
 *   between nodes in the first row and column, we need to try filling the 
 *   first cell (1, 1), and then use the 2) rule again.
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

#define rem ((llong)1e9)
#define maxn 101010
struct point : pair<int, int> {
    short color;
};

int n, m, k;
point a[maxn];
int prev_row[maxn], prev_col[maxn];

int node(int r, int c) {
    if (r == 0) return c;
    return m + r - 1;
}


bool is_determined[maxn * 2];
vector<vector<pair<int, int>>> base_gr, gr;
void add_edge(int u, int v, int cost) {
#define str(node) {\
    if (node >= m) clog << "(" << node - m + 1 << ", 0)"; \
    else clog << "(0, " << node << ")";\
} while (0);
    // str(u); clog << " "; str(v); clog << " " << cost << endl;
    gr[u].emplace_back(v, cost);
    gr[v].emplace_back(u, cost);
}

int color[maxn * 2];
int bfs(int u) {
    bool has_determined = false;
    queue<int> qu;
    for (qu.push(u), color[u] = 0; len(qu); qu.pop()) {
        u = qu.front();
        if (is_determined[u]) has_determined = true;
        for (auto i: gr[u]) {
            int v = i.xx, c = i.yy;
            if (color[v] != -1) {
                if (i.yy != (color[u] != color[v])) return 0;
                continue;
            }
            color[v] = c != color[u];
            qu.push(v);
        }
    }
    return 1 + !has_determined;
}

bool rec_xor(int length, int width) {
    if (length & 1) return 0;
    if (width & 1) return 0;
    return 1;
}

#define xor2(a, b) (!(a) != !(b))
#define xor3(a, b, c) xor2(a, xor2(b, c))

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    rep(i, k) {
        cin >> a[i].xx >> a[i].yy >> a[i].color;
        --a[i].xx, --a[i].yy;
        if (a[i].xx == 0 or a[i].yy == 0) 
            is_determined[node(a[i].xx, a[i].yy)] = 1;
    }

    memset(prev_row, -1, sizeof(prev_row));
    memset(prev_col, -1, sizeof(prev_col));

    sort(a, a + k);
    gr.resize(n + m - 1);
    rep(i, k) {
        // clog << i << ' ' << a[i].xx << ' ' << a[i].yy << endl;
        if (prev_row[a[i].xx] != -1) {
            int f = prev_row[a[i].xx];
            add_edge(node(0, a[i].yy), node(0, a[f].yy), xor3(
                a[i].color,
                a[f].color,
                rec_xor(a[i].xx + 1, a[i].yy - a[f].yy + 1)
            ));
        }

        if (prev_col[a[i].yy] != -1) {
            int f = prev_col[a[i].yy];
            add_edge(node(a[i].xx, 0), node(a[f].xx, 0), xor3(
                a[i].color,
                a[f].color,
                rec_xor(a[i].xx - a[f].xx + 1, a[i].yy + 1)
            ));
        }
        prev_row[a[i].xx] = i;
        prev_col[a[i].yy] = i;
    }

    // clog << "done base" << endl;
    base_gr = move(gr);

    llong ans = 0;
    rep(first_cell_color, 2) {
        // clog << "first cell " << first_cell_color << endl;
        if (k and a[0] == make_pair(0, 0) and first_cell_color != a[0].color) continue;
        gr = base_gr;
        rep(i, k) {
            if (a[i] == make_pair(0, 0)) continue;
            add_edge(node(0, a[i].yy), node(a[i].xx, 0), xor3(
                first_cell_color,
                a[i].color,
                rec_xor(a[i].xx + 1, a[i].yy + 1)
            ));
        }

        llong cur_ans = 1;
        is_determined[0] = 1;
        memset(color, -1, sizeof(color));
        rep(i, len(gr)) if (color[i] == -1) {
            (cur_ans *= bfs(i)) %= rem;
        }


        // clog << cur_ans << endl;
        (ans += cur_ans) %= rem;
    }
    cout << ans;

    return 0;
}

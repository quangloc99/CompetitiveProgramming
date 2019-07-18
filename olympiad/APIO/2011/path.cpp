/**
 * Author: Tran Quang Loc (darkkcyan)
 * Link to online submitting: https://dunjudge.me/analysis/problems/1232/
 * Idea: this is a graph problem, and nodes are points on the coordinates.
 * To reduce the number of points, I just pick point with x and y that apperes in the input.
 * The hard part is to construct the graph.
 * Firstly, we should connect all the point on the edge of each rectangle. (actually this is my last step, but nah, it is not important).
 * The connect points from rectangle to rectangle, I compressed the cooredinates, then used sweep line.
 * This is the overall idea, for more details please see the code. Note that vertical and horizontal lines
 * are done separately but similarly.
 * 
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
#define inf ((llong)1e14)
// #define rand __rand
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

struct rect {
    int x1, y1, x2, y2;
    rect() {}
    rect(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
    }
};

#define maxn 3010
int n;
int sx, sy, dx, dy;
rect a[maxn];
vector<int> xvals, yvals;

void sort_unique(vector<int>& vals) {
    sort(vals.begin(), vals.end());
    vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
}

template<class T> 
int index_of(const vector<T>& container, T value) {
    return (int)(lower_bound(container.begin(), container.end(), value) - container.begin());
}

vector<int> sweep(vector<int>& line, int left, int right, int new_val) {
    ++right;
    vector<int> ans(line.begin() + left, line.begin() + right);
    fill(line.begin() + left, line.begin() + right, new_val);
    return move(ans);
}

struct node: public pair<int, int>  {
    node(int x, int y) : pair(x, y) {}
    operator int() const { return xx * len(yvals) + yy; }
};

llong distance(node const& u, node const& v) {
    return (llong)abs(xvals[u.xx] - xvals[v.xx]) + (llong)abs(yvals[u.yy] - yvals[v.yy]);
}

int n_gr;
vector<node> gr[maxn * maxn];
void add_edge(node const& u, node const& v) {
    // clog << "Edge " << xvals[u.xx] << ' ' << yvals[u.yy] << ' ' << xvals[v.xx] << ' ' << yvals[v.yy] << endl;
    gr[u].push_back(v);
    gr[v].push_back(u);
}

llong dis[maxn * maxn];
llong find_dis(node const& src, node const& dst) {
    rep(i, n_gr) dis[i] = inf;
    dis[src] = 0;
#define pq_t pair<llong, node>
    priority_queue<pq_t, vector<pq_t>, greater<pq_t>> pq;
    for (pq.push({0, src}); len(pq); ) {
        node u = pq.top().yy;
        llong cur_dis = pq.top().xx;
        pq.pop();
        if (cur_dis > dis[u]) continue;
        if (u == dst) return cur_dis;
        for (auto v: gr[u]) {
            llong nd = cur_dis + distance(u, v);
            if (nd >= dis[v]) continue;
            dis[v] = nd;
            pq.push({nd, v});
        }
    }
    return -1;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    rep1(testcase, ntest) {
        cin >> sx >> sy >> dx >> dy;
        cin >> n;
        rep(i, n) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            a[i] = rect(x1, y1, x2, y2);
        }

        a[n] = rect(sx, sy, sx, sy);
        a[n + 1] = rect(dx, dy, dx, dy);
        n += 2;

        xvals.clear(); yvals.clear();
        rep(i, n) {
            xvals.push_back(a[i].x1); xvals.push_back(a[i].x2);
            yvals.push_back(a[i].y1); yvals.push_back(a[i].y2);
        }
        sort_unique(xvals); sort_unique(yvals);

        rep(i, n) {
            a[i].x1 = index_of(xvals, a[i].x1);
            a[i].x2 = index_of(xvals, a[i].x2);
            a[i].y1 = index_of(yvals, a[i].y1);
            a[i].y2 = index_of(yvals, a[i].y2);
        }
        sx = index_of(xvals, sx);
        sy = index_of(yvals, sy);
        dx = index_of(xvals, dx);
        dy = index_of(yvals, dy);

        n_gr = len(xvals) * len(yvals);
        rep(i, n_gr) gr[i].clear();

        {
            vector<int> line_x(len(xvals), -1);
            sort(a, a + n, [&](const rect& u, const rect& v) { return u.y1 < v.y1; });
            rep(i, n) {
                int cur_x = a[i].x1 - 1;  // -1 just for iteration
                for (auto old_y: sweep(line_x, a[i].x1, a[i].x2, a[i].y2)) {
                    ++cur_x;
                    if (old_y < 0) continue;
                    add_edge(node(cur_x, old_y), node(cur_x, a[i].y1));
                }
            }
        }

        {
            vector<int> line_y(len(yvals), -1);
            sort(a, a + n, [&](const rect& u, const rect& v) { return u.x1 < v.x1; });
            rep(i, n) {
                int cur_y = a[i].y1 - 1;  
                for (auto old_x: sweep(line_y, a[i].y1, a[i].y2, a[i].x2)) {
                    ++cur_y;
                    if (old_x < 0) continue;
                    add_edge(node(old_x, cur_y), node(a[i].x1, cur_y));
                }
            }
        }
        rep(i, n) {
            for (int x = a[i].x1; x < a[i].x2; ++x) 
                for (int y: {a[i].y1, a[i].y2})
                    add_edge(node(x, y), node(x + 1, y));
            for (int y = a[i].y1; y < a[i].y2; ++y) 
                for (int x: {a[i].x1, a[i].x2}) 
                    add_edge(node(x, y), node(x, y + 1));
        }

        llong ans = find_dis(node(sx, sy), node(dx, dy));
        if (ans == -1) 
            cout << "No Path\n";
        else cout << ans << '\n';
    }

    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/JOI17_bulldozer
 * Idea: 
 * - Let's first see the first subtask. It is actually the classical problem called maximum sum subarray.
 *   The solution for this problem (beside bruteforcing for small n) is dp and divide and conquer, but run in O(n).
 *   Here we need the divide and conquer solution.
 * - Now imagine we need to modify the array. How to find the maximum sum subarray? We can of course use segment tree.
 *   I won't go into detail here.
 * - Here is the final observation for this problem. We now have a plane with some points on that. We project all the points
 *   into Ox line, like the first subtask. When we rotate the plane, but keeps the Ox line, the only changing is,
 *   some points SWAP places. More precisely, some subsegment of points will be reversed. For example, image 3 points lie in
 *   a line parallel with Ox. When we rotate 90 deg, 3 points line up in a line, and their place in Ox coincide, and after a few
 *   more degree rotating, their order reversed from the original.
 * - There will be O(n^2) times reversing, so the overall solution is simple: we precalculate all the event when the points
 *   swapping places. Sort those even by angle, and using segment tree to update the result for each event.
 *
 * The code below is actually ugly in my opinion. The reason is that, the oj.uz server is stupidly slow. I had to do lots of
 * optimization. This code run locally in my machine in 1.1s, and on oj.uz is 1.7s. There is actually 1 more observation for 
 * faster calculation that I did not mention above:
 * - The order of those reversing subsegment is, firstly they lie in original place, then they coincide, and then finally
 *   they are reversed. We don't actually need to handle the case when they coincide, because the "original place" and the
 *   "reversed place" cases are cover that case.
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

#define debug 0
#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
#define clog if (debug) cerr

#define maxn 2020
template<class Num>
int sign(Num num) { return num < 0 ? -1 : num > 0; }

#define data data__
struct data {
    llong max_sum_subarr, max_left_sum, max_right_sum, sum;
    data() {}
    data(llong elm): max_sum_subarr(elm), max_left_sum(elm), max_right_sum(elm), sum(elm) {}
    data(llong mss, llong mls, llong mrs, llong s):
        max_sum_subarr(mss), max_left_sum(mls), max_right_sum(mrs), sum(s) {}

    void inc(llong val) {
        sum += val;
        max_sum_subarr = max_left_sum = max_right_sum = sum;
    }
};

data operator + (const data& u, const data& v) {
    return data(
            max({u.max_sum_subarr, v.max_sum_subarr, u.max_right_sum + v.max_left_sum}),
            max(u.max_left_sum, u.sum + v.max_left_sum),
            max(v.max_right_sum, v.sum + u.max_right_sum),
            u.sum + v.sum
    );
}

data it[maxn * 4];
int state[maxn * 4] = {0};
int depth[maxn * 4] = {0};
int cur_state = 0;
vector<int> upd_queue[15];
void do_upd() {
    ++cur_state;
    for (int d = 15; d--; ) {
        for (auto u: upd_queue[d]) {
            if (state[u] != -1) it[u] = it[u << 1] + it[u << 1 | 1];
            if (u != 1 and state[u >> 1] != cur_state) {
                state[u >> 1] = cur_state;
                upd_queue[d - 1].push_back(u >> 1);
            }
        }
        upd_queue[d].clear();
    }
}

int leaf[maxn];
void build_it(int i, int l, int r, function<llong(int)> getitem) {
    if (i != 1) depth[i] = depth[i / 2] + 1;
    if (l + 1 == r) {
        leaf[l] = i;
        state[i] = -1;
        it[i] = data(getitem(l));
        upd_queue[depth[i]].push_back(i);
        return ;
    }
    int mid = (l + r) / 2;
    build_it(i << 1, l, mid, getitem);
    build_it(i << 1 | 1, mid, r, getitem);
    if (i == 1) do_upd();
}

void stage(int i) {
    i = leaf[i];
    upd_queue[depth[i]].push_back(i);
}

struct line {
    llong a, b, c;   // ax + by + c = 0
    line(llong a_ = 0, llong b_ = 0, llong c_ = 0) : a(a_), b(b_), c(c_) {}

    line& norm() {
        llong g = __gcd(a, b);
        g = __gcd(g, c);
        a /= g;
        b /= g;
        c /= g;

        // the normal will point almost the same way as Oy
        // also the line a == b == 0 does not "really" exist, no need to handle it.
        if (b < 0 || (b == 0 and a < 0)) {
            a = -a; b = -b; c = -c;
        }
        return *this;
    }

    template<class Point>
    static line through_points(Point const& u, Point const& v) {
        return line(
            u.y - v.y,
            - u.x + v.x,
            u.x * v.y - u.y * v.x
        ).norm();
    }
};

bool operator == (const line& lhs, const line& rhs) {
    return lhs.a == rhs.a and lhs.b == rhs.b and lhs.c == rhs.c;
}

struct spot {
    llong x, y, cost;
    int pos;
    spot(llong x_ = 0, llong y_ = 0, llong v = 0) : x(x_), y(y_), cost(v), pos(0) {}
};

bool cmp_line(const line& lhs, const line& rhs) {
    int cross = sign(lhs.a * rhs.b - lhs.b * rhs.a);
    if (cross == 0) return lhs.c < rhs.c;
    return cross > 0;
}

int n;
spot* a[maxn];  // for the sake of keeping things in places, I used pointer.

int main(void) {
    auto t1 = std::chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0); 
    cin >> n;
    rep(i, n) {
        a[i] = new spot();
        cin >> a[i]->x >> a[i]->y >> a[i]->cost;
    }
    sort(a, a + n, [](const spot* u, const spot* v) {
        return (u->x == v->x) ? u->y > v->y : u->x < v->x;
    });
    
    rep(i, n) a[i]->pos = i;

    vector<tuple<line, spot*, spot*>> groups;
    groups.reserve(n * n);
    rep(i, n) {
        for (int f = i + 1; f < n; ++f) {
            line ln = line::through_points(*a[i], *a[f]);
            groups.emplace_back(ln, a[i], a[f]);
        }
    }

    sort(groups.begin(), groups.end(), [](auto const& u, auto const& v) {
            return cmp_line(get<0>(u), get<0>(v));
    });

    build_it(1, 0, n, [](int u) { return a[u]->cost; });
    llong ans = 0;

    auto upd_ans = [&]() {
        do_upd();
        ans = max(ans, it[1].max_sum_subarr);
    };
    upd_ans();

    vector<tuple<llong, int, int>> ranges;
    auto process = [&]() {
        for (auto [c, min_pos, max_pos]: ranges) {
            for (int l = min_pos, r = max_pos; l < r; ++l, --r) {
                swap(a[l]->pos, a[r]->pos);
                swap(a[l], a[r]);
            }
            for (int i = min_pos; i <= max_pos; ++i) {
                it[leaf[i]] = data(a[i]->cost);
                stage(i);
            }
        }
        upd_ans();
    };

    llong lna = 1, lnb = 0;
    for (auto& [ln, p1, p2]: groups) {
        if (lna != ln.a or lnb != ln.b) {
            process();
            ranges.clear();
        }
        if (ranges.empty() or get<0>(ranges.back()) != ln.c) {
            ranges.emplace_back(ln.c, n, 0);
        }
        for (auto& i: {p1, p2}) {
            get<1>(ranges.back()) = min(get<1>(ranges.back()), i->pos);
            get<2>(ranges.back()) = max(get<2>(ranges.back()), i->pos);
        }
        lna = ln.a;
        lnb = ln.b;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    clog << duration ;

    process();
    cout << ans;
    return 0;
}

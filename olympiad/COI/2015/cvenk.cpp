// Author: Tran Quang Loc (darkkcyan)
// Problem: https://oj.uz/problem/view/COI15_cvenk
// Ideas:
// The graph is just a tree. If we built the tree, the rest is just classical dynamic programming.
// Building tree is tricky though. I'm sure there are many way, and it can be done in O(nlog^2n) since the TL is 3s.
// But this code is O(nlogn).  See the code for more details.
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

#define CONCAT_(x, y) x##y/*{{{*/
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int __db_level = 0;
bool __db_same_line = false;
#define clog cerr << string(!__db_same_line ? __db_level * 2 : 0, ' ')
struct debug_block {
    function<void()> fn;
    void print_name() { __db_same_line = true; fn(); clog << endl; __db_same_line = false;  }
    debug_block(function<void()> fn_): fn(fn_) { clog << "{ "; print_name(); ++__db_level; }
    ~debug_block() { --__db_level; clog << "} "; print_name(); }
};
#define DB(args...) debug_block CONCAT(dbbl, __LINE__)([=]{ clog << args; })
#define deb(...)  if (1) { (clog << "[" #__VA_ARGS__ "] = [" << __VA_ARGS__) << "]"; if (!__db_same_line) clog << endl; }
#else
#define clog if (0) cerr
#define DB(...)
#define deb(...)
#endif
template<class T>
ostream& operator,(ostream& out, const T& thing) { return out << ", " << thing; }
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return (out << "(" << p.first, p.second) << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return (out << "(" << get<0>(t), get<1>(t)) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return (out << "(" << get<0>(t), get<1>(t), get<2>(t)) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out, container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

const int maxn = 101010;
#ifndef LOCAL_DEBUG
const int max_bit = 31;
#else
const int max_bit = 10;
#endif
using pii = pair<int, int>;

struct node {
    int x, y, cnt;
    node *right = 0, *down = 0;
    llong dist_from_child = 0;
    node(int x_, int y_, int cnt_ = 0): x(x_), y(y_), cnt(cnt_) {
        clog << "new node" << endl;
        deb(x, y);
    }
    ~node() {
        if (right) delete right;
        if (down) delete down;
    }

    friend int dist(node* u, node* v) {
        return abs(u->x - v->x) + abs(u->y - v->y);
    }

    void dfs1() {
        for (auto i: {right, down}) {
            if (!i) continue;
            i->dfs1();
            cnt += i->cnt;
            dist_from_child += i->dist_from_child + 1ll * i->cnt * dist(this, i);
        }
    }

    llong dfs_cal_ans() {
        llong ans = dist_from_child;
        for (auto i: {right, down})
            if (i) ans = min(ans, i->dfs_cal_ans(this, cnt, dist_from_child));
        return ans;
    }

    llong dfs_cal_ans(node* parent, int sum_cnt, llong total_dist) {
        total_dist -= 1ll * cnt * dist(this, parent);
        total_dist += 1ll * (sum_cnt - cnt) * dist(this, parent);
        llong ans = total_dist;
        for (auto i: {right, down})
            if (i) ans = min(ans, i->dfs_cal_ans(this, sum_cnt, total_dist));
        return ans;
    }
};

int n;
pii pos[maxn];

tuple<node*, node*> build(node* root, pii* l, pii* r, int cur_bit = max_bit - 1) {
    DB("build "; deb(l - pos, r - pos)); 
    if (cur_bit == -1) {
        root->cnt += (int)(r - l);
        return {root, root};
    }

    int bit = 1 << cur_bit;
    pii *beg_x1 = l, *beg_y1 = r;
    for (auto i = l; i != beg_y1; ) {
        if (i->second & bit) swap(*i, *(--beg_y1));
        else {
            if (~i->first & bit) swap(*i, *(beg_x1++));
            ++i;
        }
    }
    deb(beg_x1 - pos, beg_y1 - pos);
    auto right = root, down = root;
    if (l != beg_x1) {
        tie(right, down) = build(root, l, beg_x1, cur_bit - 1);
    }
    if (beg_x1 != beg_y1) {
        right->right = new node(root->x + bit, root->y);
        auto [new_right, _] = build(right->right, beg_x1, beg_y1, cur_bit - 1);
        right = new_right;
    }

    if (beg_y1 != r) {
        down->down = new node(root->x, root->y + bit);
        auto [_, new_down] = build(down->down, beg_y1, r, cur_bit - 1);
        down = new_down;
    }
    return {right, down};
}



int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) {
        cin >> pos[i].first >> pos[i].second;
    }

    node root(0, 0);
    build(&root, pos, pos + n);
    root.dfs1();
    cout << root.dfs_cal_ans();

    return 0;
}

// vim: foldmethod=marker

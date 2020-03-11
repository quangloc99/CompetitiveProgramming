// vim: foldmethod=marker
// Author: Tran Quang Loc (darkkcyan)
// Problem "Candy machine" at https://cses.fi/107/list/
// Idea: 
// - Let's plot a graph of the candy event. For each event we plot a point at (time, position).
// - Now, rotate the graph 45 degree couter clockwise. Doing this give us a very easy way to check
//   whether we can go from point (x1, y1) to (x2, y2). If (x1 <= x2) && (y1 <= y2) then yes, we can.
// - Let's now do sweepline from bottom to top. Doing that we just need to pick point with x's coordinate
//   less than the current point.
// - If we see there is a previsous point that can reach the curernt point, then we MUST do that.
//   Because suppose that we dont and create a new cart at this point, and use the current point to
//   reach a later point (which could not be reached from the current point), then the number of cart
//   (more over, the set of carts) are the same as we move that previous cart to the current point,
//   and create new one at the later point. 
// - Also if there are many points that can reach the current point, we must pick the closest to
//   this point by x coordinate. Because there might be a point, higher than the current point but
//   lie on the left. In that case we use the further points on the left of the current point.
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

struct candy_evt {
    int where, when;
    int ans;

    llong proj_x, proj_y;
    void cal_proj() {
        proj_x = 1ll * when - where;
        proj_y = 1ll * when + where;
    }
};

const int maxn = 101010;
int n;
candy_evt a[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) {
        cin >> a[i].where >> a[i].when;
    }

    rep(i, n) {
        a[i].cal_proj();
    }

    sort(a, a + n, [](const candy_evt& u, const candy_evt& v) {
            if (u.proj_y == v.proj_y) return u.proj_x < v.proj_x;
            return u.proj_y< v.proj_y;
    });

    struct cmp_candy {
        bool operator()(candy_evt* u, candy_evt* v) const {
            if (u->proj_x == v->proj_x) return u->proj_y < v->proj_y;
            return u->proj_x < v->proj_x;
        }
    };
    set<candy_evt*, cmp_candy> prio;

    int cart_count = 0;
    rep(i, n) {
        auto prev_ans = prio.upper_bound(a + i);
        if (prev_ans == prio.begin()) {
            a[i].ans = cart_count++;
        } else {
            --prev_ans;
            a[i].ans = (*prev_ans)->ans;
            prio.erase(prev_ans);
        }
        prio.insert(a + i);
    }

    cout << cart_count << '\n';
    // rep(i, n) cout << a[i].where << ' ' << a[i].when << ' ' << a[i].ans + 1 << ' ' << a[i].proj_x << ' ' << a[i].proj_y << '\n'; 
    rep(i, n) cout << a[i].where << ' ' << a[i].when << ' ' << a[i].ans + 1 << '\n';


    return 0;
}

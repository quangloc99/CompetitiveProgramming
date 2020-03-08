// vim: foldmethod=marker
//
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://www.acmicpc.net/problem/5252
// (well when I submitted this I got the green verdict "Hit!" with google translate. I guess it is AC :)))
// Idea:
// - There are 2 numbers here, so I tend to do offline processing, and the idea some what look like
//   sweepline. So, sort the query increasing by the age, then we iterate the queries and just add the player
//   that is not added and the age is smaller than the current query's age restriction.
// - The observation here is obviously greedy, that if we are having some player that adjacent to each other,
//   then we just need to pick the player with highest skill, then 3rd highest skill, then 5th, then 7th, ...
// - So we need to somehow manage the subsegment of the players (sorted by skill). This can easily be done
//   using DSU.
// - How to find the answer though. In this solution I use fenwick tree, one for counting, and one for finding
//   sum of skill. When merging 2 subsegment with DSU, we also need to update the fenwick tree so that
//   only the first element of the segment contains the values about counting and sum of skills.
// - The edge case is that for each query, there might be the last segment where we dont need to
//   pick all the player like above. In this case I do binary search it (on the counting fenwick tree),
//   then find the sum using another prefix sum array.
//
// Sun Mar  8 10:29:51 MSK 2020: START coding
// Sun Mar  8 10:50:19 MSK 2020: PAUSE
// Sun Mar  8 10:55:08 MSK 2020: CONTINUE
// Sun Mar  8 11:24:33 MSK 2020: DONE coding
// Sun Mar  8 11:41:48 MSK 2020: OK sample
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

const int maxn = 301010;
const int max_bit = 19;

template<class T>
struct SUM_BIT {
    T data[maxn];
    SUM_BIT() {
        memset(data, 0, sizeof(data));
    }
    void upd(int i, T del = 1) {
        for (++i; i < maxn; i += i & -i) data[i] += del;
    }
    T get(int i) {
        i = min(i, maxn - 2);
        T ans = 0;
        for (++i; i > 0; i -= i & -i) ans += data[i];
        return ans;
    }

    pair<int, T> find_last(T k) {
        int pos = 0;
        T cur_sum = 0;
        for (int i = max_bit; i--; ) {
            int nxt = pos + (1 << i);
            if (nxt >= maxn) continue;
            if (cur_sum + data[nxt] >= k) continue;
            cur_sum += data[nxt];
            pos = nxt;
        }
        return {pos - 1, cur_sum};
    }
};
SUM_BIT<int> count_bit;
SUM_BIT<llong> skill_bit;

int dsu[maxn], seg_len[maxn];
llong val[maxn][2];
int findp(int u) {
    int p = u;
    while (p != dsu[p]) p = dsu[p];
    while (u != p) { int t = dsu[u]; dsu[u] = p; u = t;}
    return p;
}

void init_node(int u, llong skill) {
    dsu[u] = u;
    seg_len[u] = 1;
    val[u][0] = skill;
    val[u][1] = 0;
    skill_bit.upd(u, skill);
    count_bit.upd(u, 1);
}

void join(int u, int v) {
    DB("JOIN");
    u = findp(u); v = findp(v);
    if (u == v) return ;
    if (u < v) swap(u, v);
    deb(u, v);
    dsu[u] = v;

    skill_bit.upd(u, -val[u][0]);
    llong x = val[u][0], y = val[u][1];
    if (seg_len[v] & 1) swap(x, y);
    val[v][0] += x;
    val[v][1] += y;
    skill_bit.upd(v, x);

    count_bit.upd(u, -(seg_len[u] + 1) / 2);
    count_bit.upd(v, -(seg_len[v] + 1) / 2);
    seg_len[v] += seg_len[u];
    count_bit.upd(v, (seg_len[v] + 1) / 2);
}

struct player {
    int age;
    llong skill;
};

struct query {
    llong *ans;
    int age_limit;
    int count_limit;
};

int n, q;
player pl[maxn];
int ptr_pl[maxn];
llong ans[maxn];
query qr[maxn];

bool mark[maxn] = {0};
llong sum[2][maxn];

llong do_query(int count) {
    DB(""; deb(count));
    auto [pos, count_before] = count_bit.find_last(count);
    deb(pos, count_before);
    ++pos;
    llong res = skill_bit.get(pos - 1);
    count -= count_before;
    res += sum[pos & 1][pos / 2 + count] - sum[pos & 1][pos / 2];
    return res ;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> pl[i].age >> pl[i].skill;
    cin >> q;
    rep(i, q) {
        cin >> qr[i].age_limit >> qr[i].count_limit;
        qr[i].ans = ans + i;
    }

    sort(pl, pl + n, [](const player& u, const player& v) { return u.skill > v.skill; });
    { DB("sorted players by skill");
        rep(i, n) deb(pl[i].age, pl[i].skill);
    }
    rep(i, n) ptr_pl[i] = i;
    sort(ptr_pl, ptr_pl + n, [](int u, int v) { return pl[u].age < pl[v].age; });
    { DB("sorted players by age");
        rep(i, n) deb(pl[ptr_pl[i]].age, pl[ptr_pl[i]].skill);
    }
    sort(qr, qr + q, [](const query& u, const query& v) { return u.age_limit < v.age_limit; });

    sum[0][0] = sum[1][0] = 0;
    rep(i, n) sum[i & 1][i / 2 + 1] = sum[i & 1][i / 2] + pl[i].skill;

    int u = 0;
    rep(v, q) {
        DB(""; deb(v, qr[v].age_limit, qr[v].count_limit));
        for (;u < n and pl[ptr_pl[u]].age <= qr[v].age_limit; ++u) {
            int i = ptr_pl[u];
            deb("add player", i);
            init_node(i, pl[i].skill);
            mark[i] = 1;
            if (i > 0 and mark[i - 1]) join(i, i - 1);
            if (i < n - 1 and mark[i + 1]) join(i, i + 1);
            deb(count_bit.get(0));
        }
        *(qr[v].ans) = do_query(qr[v].count_limit);
        deb(*qr[v].ans);
    }
    rep(i, q) cout << ans[i] << '\n';
    return 0;
}

// vim: foldmethod=marker

/**
Author: Tran Quang Loc (darkkcyan)
Problem melody at: https://cses.fi/99/list/
Idea:
We can consider the graph between notes, 2 nodes are adjacencent with each other if we can play them consecutively.
Building this graph as well as finding all shortest paths (with tracing) is not hard.
To solve the song, we can use dp as following: dp[i] = min cost so that we can play the song till this note, as well as
we stop at this note. Calculate dp[i] can be done in O(n * log(song length)) (n = the number of notes). For each of
the notes v, we find the closest position in the song to the currect position that containts notes v, but not too close,
precisly must be bigger than the distance between v and the note at position i. Let that position be p, then from p we can 
increase the result by i - p - 1 to the position i by replace all the notes between them with the path between them on the graph.
If there is more space to fill then just print 1 notes more than 1 times.
*/

// Wed Feb 26 22:45:01 MSK 2020: START coding
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

const int maxn = 111;
const int maxl = 101010;
const int inf = (int)1e7;
int n, s, g;
string notes[maxn];
vector<int> gr[maxn];
vector<pair<int, int>> min_dist_from[maxn];

vector<pair<int, int>> bfs(int source) {
    vector<pair<int, int>> min_dist(n + 2, {inf, 0});
    queue<int> qu;
    for (qu.push(source), min_dist[source] = {0, source};  len(qu); qu.pop()) {
        int u = qu.front();
        for (auto v: gr[u]) {
            if (min_dist[v].first != inf) continue;
            min_dist[v].first = min_dist[u].first + 1;
            min_dist[v].second = u;
            qu.push(v);
        }
    }
    return min_dist;
}

int count_diff(const string& u, const string& v) {
    int ans = 0; 
    rep(i, min(len(u), len(v)))
        ans += u[i] != v[i];
    return ans;
}

int song[maxl];
vector<int> pos[maxn];

pair<int, int> dp[maxl];

void print_bfs_trace(int source, int u, int dist) {
    if (dist == 0) return ;
    print_bfs_trace(source, min_dist_from[source][u].second, dist - 1);
    if (u != 0 and u != n + 1)
        cout << u << ' ';
}

void print_dp_trace(int i) {
    if (i == 0) return ;
    int f = dp[i].second;
    print_dp_trace(f);
    if (f == 0) rep(x, i - f) cout << song[i] << ' ';
    else print_bfs_trace(song[f], song[i], i - f);
}


int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> s >> g;
    rep1(i, n) cin >> notes[i];
    int song_length; cin >> song_length;
    rep1(i, song_length) cin >> song[i];
    song[0] = 0;
    song[song_length + 1] = n + 1;

    rep1(i, n) rep1(f, n)
        if (count_diff(notes[i], notes[f]) <= g) {
            gr[i].push_back(f);
            gr[f].push_back(i);
        }
    rep1(i, n) {
        gr[i].push_back(n + 1);
    }

    rep1(i, n + 1) {
        min_dist_from[i] = bfs(i);
    }

    rep1(i, n) pos[i].push_back(0);
    dp[0] = {0, -1};

    rep1(i, song_length + 1) {
        dp[i] = {inf, -1};
        int u = song[i];
        rep1(v, n) {
            if (min_dist_from[v][u].first == inf) continue;
            auto t = upper_bound(pos[v].begin(), pos[v].end(), i - min_dist_from[v][u].first);
            if (t == pos[v].begin()) continue;
            --t;
            dp[i] = min(dp[i], {dp[*t].first + i - *t - 1, *t});
        }
        pos[u].push_back(i);
    }

    cout << dp[song_length + 1].first << '\n';
    print_dp_trace(song_length + 1);

    return 0;
}

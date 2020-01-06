// vim: foldmethod=marker
/**
 * BEFORE READING THE CODE: the lines with DB() and debln are just for debugging. Ignore though lines if
 * you just need to read the idea.
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://wcipeg.com/problem/coci096p6
 * Ideas:
 * - this problem seems to be like finding longest path in a given times. Because the number of nodes is
 *   limited and there will always be a node in the current times, so our path will have the following form:
 *   + start from node x.
 *   + reach node y and start going in cycles to return to node y
 *   + finally from y we go to node z and stop there.
 *   + Or we can write like this: x -> (y) -> z
 * - We can solve this problem with this approach in O(n^6) with the help of DP. If we call 
 *   min_time[u][v][cl] is the minimum time we start from node u to node v with the chain length is `cl`,
 *   then we can bruthforces x, y, r and the chain length from x->y, y->z and the repeat chain length
 *   of the cycle containing y.
 * - We can do some further optimization: we don't need to know x and z, so instead for every node y, we
 *   just need to calculate for each `cl`, which is the minimum time can we reach y if we start from 
 *   one of the `n` nodes with the chain length equals to `cl`. So we completely remove x. Do the same thing
 *   with z. This optimization brings the time complexity down to O(n^4).
 * - And finally, we also don't need to know the number of chain length we start and end with, so we can
 *   combine them in to something called `non repeated chain length`. Use the information we calculated above,
 *   we can compute the minimum time for each `non repeated chain length` we need to spend. So the 
 *   solution now is O(n^3), since we need to bruteforce only `non repeated chain length`, `repeated chain length`,
 *   and `y`.
 * - Well, this code is O(n^3 * log(n^2)), since it used Dijikastra's algorithm. I think there will be 
 *   another optimization for this part.
 */


// Mon Jan  6 18:34:16 MSK 2020: START coding
// Mon Jan  6 18:45:48 MSK 2020: STOP for to think about the finding min_time part
// Mon Jan  6 18:53:26 MSK 2020: CONTINUE coding
// Mon Jan  6 19:02:58 MSK 2020: FINISH coding
// Mon Jan  6 19:13:56 MSK 2020: OK samples
// Mon Jan  6 19:42:02 MSK 2020: AC
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

/*{{{*/
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int debug = 1;
#define DB(...) stringstream CONCAT(ss, __LINE__); CONCAT(ss, __LINE__) << __VA_ARGS__; debug_block CONCAT(dbbl, __LINE__)(CONCAT(ss, __LINE__).str())
#else
int debug = 0;
#define DB(...)
#endif
int __db_level = 0;
#define clog if (debug) cerr << string(__db_level * 2, ' ')
struct debug_block {
    string name;
    debug_block(const string& name_): name(name_) { clog << "{ " << name << endl; ++__db_level; }
    ~debug_block() { --__db_level; clog << "} " << name << endl; }
};
#define deb(...) "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]"
#define debln(...)  clog << "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]" << endl
#define _ << ", " <<
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return out << "(" << p.first _ p.second << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return out << "(" << get<0>(t) _ get<1>(t) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return out << "(" << get<0>(t) _ get<1>(t) _ get<2>(t) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out _ container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }
/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

#define maxn (111)
const int inf = (int) 5e8;
int n, max_chain;
llong t;
int y[maxn];
vector<pair<int, int>> gr[maxn];

int min_time[maxn][maxn][maxn];  // [u][v][cl]: shortest time from u to v with chain length `cl`

int main(void) {
    debln(sizeof(min_time) / 1024 / 1024);
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> t;
    rep(i, n) {
        int k; cin >> k >> y[i];
        rep(f, k) {
            int u; cin >> u;
            gr[i].emplace_back(u - 1, 0);
        }
        rep(f, k) {
            cin >> gr[i][f].second;
            gr[i][f].second += y[i];
        }
    }

    max_chain = n + 10;
    rep(u, n) rep(v, n) rep(cl, max_chain)
        min_time[u][v][cl] = inf;

    rep(start, n) {
        DB(deb(start));
        auto cur_min_time = min_time[start];
        typedef tuple<int, int, int> vertex;
        priority_queue<vertex, vector<vertex>, greater<vertex>> prio;
        cur_min_time[start][0] = 0;
        for (prio.emplace(0, start, 0); len(prio); ) {
            int cur_dis, des, cl;
            tie(cur_dis, des, cl) = prio.top();  // dam it, C++11
            prio.pop();
            if (cur_dis > cur_min_time[des][cl]) continue;
            debln(des _ cl _ cur_dis);

            int new_cl = cl + 1;
            for (auto i: gr[des]) {
                int v = i.first, new_dis = cur_dis + i.second;
                if (new_dis >= cur_min_time[v][new_cl]) continue;
                cur_min_time[v][new_cl] = new_dis;
                prio.emplace(new_dis, v, new_cl);
            }
        }
    }

    llong ans = 0;
    rep(repeat_point, n) {
        DB(deb(repeat_point));
        static int min_start_time[maxn];
        static int min_end_time[maxn];
        
        rep(cl, max_chain) {
            min_start_time[cl] = inf;
            min_end_time[cl] = inf;
            rep(u, n) {
                min_start_time[cl] = min(min_start_time[cl], min_time[u][repeat_point][cl]);
                min_end_time[cl] = min(min_end_time[cl], min_time[repeat_point][u][cl]);
            }
            debln(cl _ min_start_time[cl] _ min_end_time[cl]);
        }

        rep(non_repeat_cl, max_chain) {
            DB(deb(non_repeat_cl));

            llong min_non_repeat_time = inf;
            rep(start_cl, max_chain) {
                int end_cl = non_repeat_cl - start_cl;
                if (end_cl < 0) break;
                min_non_repeat_time = min(min_non_repeat_time, 1ll * min_start_time[start_cl] + min_end_time[end_cl]);
            }
            debln(min_non_repeat_time);

            if (min_non_repeat_time > t) continue;
            llong time_left = t - min_non_repeat_time;
            llong cur_ans = 0;

            rep1(repeat_cl, max_chain - 1) {
                cur_ans = max(cur_ans,
                        time_left / min_time[repeat_point][repeat_point][repeat_cl] * repeat_cl +
                        non_repeat_cl
                );
            }
            debln(cur_ans);
            ans = max(ans, cur_ans);
        }
    }
    cout << ans;

    return 0;
}

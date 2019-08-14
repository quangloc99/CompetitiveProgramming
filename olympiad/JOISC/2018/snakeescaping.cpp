/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/JOI18_snake_escaping
 * Idea: 
 * - It seems very easy to solve just one query in one time. But we are processing multiple queries :(.
 * - The problem seems to be really nice since we can just do offline calculation. When doing so, we
 *   notice that there are some queries having the same prefix with each other. 
 * - Using the above observation, I could process them in the same the same time and divide then when
 *   they are different. That's right, I was doing divide and conquer.
 * - Suppose we are processing the first digit. We divide the queries into 3 group: starting with 0, 1 and 
 *   '?' respectively. For the '0' group, we simply delete the first character of the queries and recursively
 *   processing those queries with the first half of the snake-array. Do the same for the '1' group with
 *   the second half. We can also do like that with the '?' group: simply we merge 2 halfs into 1 by adding
 *   adding elements from 2nd half into the 1st one. Note that after adding and done processing, we must
 *   also subtracting elements, in other word, we need to restore the original array.
 *
 * - The advantage of my solution is that we just need memory for the input and the queries, no additional
 *   memory when processing (actually, it is still O(logn) memory since we are doing recursion).
 * - The sad part that I still did not figure it out the time complixity for this algorithm. But I think 
 *   it performs well enough. The reason is, you can see the recursion tree has a lot of leaves (3^20), but
 *   we need only 10^6 queries. The ratio is really small, so I think there will be a lot of leaves
 *   won't be visited.
 * Implementation notes:
 * - The memory limit is actually small, so I must compress the queries string to fit that limit.
 * - I tried to code like C++ stl, so I tried using template with iterator. Here I used some stl functions
 *   with iterator: distance(u, v) will return the distance between 2 iterators, and advance(u, x) will
 *   do ++u for x times (or u += x if u is random access iterator).
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

struct query_string {
    llong data;
    query_string() {}
    query_string& operator= (const string& s) {
        data = 0;
        for (auto i = s.rbegin(); i != s.rend(); ++i)
            data = data << 2 | (*i == '?' ? 3 : (*i - '0'));
        return *this;
    }

    int operator[](size_t pos) {
        return (data >> pos >> pos) & 3;
    }
};

#define maxl 20
#define maxn (1 << maxl)
#define maxq 1000101
int l, n, q;
int toxic[maxn];
pair<query_string, int*> queries[maxq];
int ans[maxq];

template<class ToxicIter, class QueryIter>
void solve(int level, ToxicIter beg_tox, ToxicIter end_tox, QueryIter beg_que, QueryIter end_que) {
    if (beg_que == end_que) return ;
    if (level == l) {
        assert(distance(beg_tox, end_tox) == 1);
        for (auto i = beg_que; i != end_que; ++i)
            *(i->yy) = *beg_tox;
        return ;
    }

    auto oneIter = beg_que, questionIter = end_que;
    for (auto i = beg_que; i != questionIter; ) {
        if (i->xx[level] == 3) swap(*i, *--questionIter);
        else if (i->xx[level] == 0) swap(*(i++), *(oneIter++));
        else ++i;
    }

    ToxicIter mid_tox = beg_tox;
    advance(mid_tox, distance(beg_tox, end_tox) / 2);

    solve(level + 1, beg_tox, mid_tox, beg_que, oneIter);
    solve(level + 1, mid_tox, end_tox, oneIter, questionIter);

    for (auto i = beg_tox, f = mid_tox; f != end_tox; ++i, ++f)
        *i += *f;

    solve(level + 1, beg_tox, mid_tox, questionIter, end_que);

    for (auto i = beg_tox, f = mid_tox; f != end_tox; ++i, ++f)
        *i -= *f;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> l >> q;
    n = 1 << l;
    {
        string s; cin >> s;
        rep(i, n) toxic[i] = s[i] - '0';
    }

    rep(i, q) {
        string que; cin >> que;
        queries[i].xx = que;
        queries[i].yy = ans + i;
    }

    solve(0, toxic, toxic + n, queries, queries + q);

    rep(i, q) cout << ans[i] << '\n';

    return 0;
}

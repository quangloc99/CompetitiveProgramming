/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: BOI 2018 candies - https://cses.fi/106/list/
 * Ideas: it is the same as the official edtorial, but I implemented the first part
 * differently. I divided the array into 10 parts. For each part, first I
 * remove that part, calculate the "order" from the others, then now
 * I was able to calculate the "order" of each element in that part
 * by adding the other element of that part into the calculated order.
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
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

#define maxn 111
#define maxval 7070
#define maxsum (maxn * maxval)
// #define maxsum 30
int n;
int a[maxn];

bool comp(tuple<int, int, bitset<maxsum>> const& u, tuple<int, int, bitset<maxsum>> const& v) {
    if (get<0>(u) == get<0>(v)) return a[get<1>(u)] > a[get<1>(v)];
    return get<0>(u) < get<0>(v);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> a[i];
    int bs_size = (int)(sqrt(n) + 1);

    bitset<maxsum> base_bs;
    base_bs[0] = 1;

    tuple<int, int, bitset<maxsum>> ans;
    for (int i = 0; i < n; i += bs_size) {
        auto buc_bs = base_bs;
        for (int f = i + bs_size; f < n; ++f) {
            buc_bs |= buc_bs << a[f];
        }

        for (int u = i; u < min(i + bs_size, n); ++u) {
            auto cur_bs = buc_bs;
            for (int v = i; v < min(i + bs_size, n); ++v) {
                if (v == u) continue;
                cur_bs |= cur_bs << a[v];
            }
            // clog << u << ' ' << a[u] << ' ' << cur_bs << endl;
            ans = max(ans, {cur_bs.count(), u, cur_bs}, comp);
        }
        
        for (int f = i; f < min(i + bs_size, n); ++f) {
            base_bs |= base_bs << a[f];
        }
    }

    int u = get<1>(ans);
    bitset<maxsum * 2> bs;
    bs[maxsum] = 1;
    rep(i, n)
        if (i != u)
            bs |= (bs >> a[i]) | (bs << a[i]);
    rep1(i, maxsum) {
        if (!bs[i + maxsum]) {
            cout << a[u] << ' ' << i;
            return 0;
        }
    }
    assert(0);


    return 0;
}

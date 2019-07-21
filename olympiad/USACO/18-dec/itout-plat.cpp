/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: http://usaco.org/index.php?page=viewproblem2&cpid=865
 * The official solution can be found there too.
 * Observation summary:
 * - The set we must choose must satisfy that the unchosen elements must form an increasing array.
 *   That means the minimum number of elements that needed to be picked is N - (longest increasing subsequence)
 * - To find the K-th smallest set, we just need to find K-th bigest LIS, then remove them to archive the result.
 * Note:
 * - After reading the input, I assigned each of a[i] to be n - a[i] + 1 for some convenients.
 * - There is a wellknown way to find both LIS and the number of LIS using BIT. But I totally forgot about it.
 *   So here I have done the "stupid" way. The only thing I can say for myself is, if it works, then it not stupid :).
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
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

template<class T>
struct Bit {
    vector<T> data;
    function<T(T, T)> fn;

    Bit(int n, int init_val, function<T(T, T)> const& fn_)
        : data(n + 3, init_val), fn(fn_) {}

    void upd(int i, T val) {
        for (++i; i < len(data); i += i & -i)
            data[i] = fn(data[i], val);
    }

    T get(int i, T ans = INT_MIN) {
        for (++i; i > 0; i -= i & -i) ans = fn(ans, data[i]);
        return ans;
    }
};

#define maxn 101010
const unsigned llong inf = (llong)2e18 + 10;
int n;
unsigned llong k;
int a[maxn];
int pos[maxn];
int lis[maxn];
vector<int> lis_group[maxn];
unsigned llong n_way[maxn];

unsigned llong safe_sum(unsigned llong u, unsigned llong v) {
    return min(u + v, inf);
}

template<class container, class T>
int lower_index(container const& x, T val) {
    return (int)(lower_bound(x.begin(), x.end(), val) - x.begin());
}

int main(void) {
    // ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    freopen("itout.in", "r", stdin);
    freopen("itout.out", "w", stdout);
    cin >> n >> k;
    // clog << n << ' ' << k << endl;
    rep(i, n) {
        cin >> a[i];
        a[i] = n - a[i] + 1;
        pos[a[i]] = i;
    }

    Bit<int> max_bit(n, 0, (const int&(*)(const int&, const int&))max<int>);
    int max_lis = 0;
    for (int i = n; i--; ) {
        lis[a[i]] = max_bit.get(a[i]) + 1;
        max_bit.upd(a[i], lis[a[i]]);
        lis_group[lis[a[i]]].push_back(a[i]);
        max_lis = max(max_lis, lis[a[i]]);
    }
    assert(max_lis != n);  // because k >= 1 and there is a least 1 way.

    // find the number of way, the stupid way :))
    rep1(i, max_lis) sort(lis_group[i].begin(), lis_group[i].end());  // the sorting is also needed for the construction part.
    vector<Bit<unsigned llong>> sum_bits;
    rep(i, max_lis + 1) {
        sum_bits.emplace_back(len(lis_group[i]), 0, safe_sum);
    }
    
    for (int i = n; i--; ) {
        int le = lis[a[i]];
        unsigned llong s = 1;
        int cur_id = lower_index(lis_group[le], a[i]);
        if (le > 1) {
            int upper_id = lower_index(lis_group[le - 1], a[i]) - 1;    // it should be more correct to use upper_bound,
                                                                        // but since there is not a[i] in the array, it is fine.
            s = sum_bits[le - 1].get(upper_id, 0);
        }
        sum_bits[le].upd(cur_id, s);
        n_way[a[i]] = s;
    }

    vector<int> unpicked;
    unpicked.push_back(n + 1);
    pos[n + 1] = -1;
    for (int length = max_lis; length > 0; --length) {
        vector<int> group;
        for (auto i: lis_group[length])
            if (pos[i] > pos[unpicked.back()]) group.push_back(i);

        int i = 0;
        for (; i < len(group) and k > n_way[group[i]]; ++i) {
            k -= n_way[group[i]];
        }
        assert(group[i] < unpicked.back());
        assert(i < len(group) and i >= 0);
        unpicked.push_back(group[i]);
    }

    cout << n - max_lis << '\n';
    reverse(unpicked.begin(), unpicked.end());
    for (int i = n; i > 0; --i) {
        while (len(unpicked) and unpicked.back() > i)
            unpicked.pop_back();
        if (unpicked.back() != i) cout << n - i + 1 << '\n';
    }

    return 0;
}

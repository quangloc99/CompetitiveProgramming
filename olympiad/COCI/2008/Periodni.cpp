/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://dmoj.ca/problem/coci08c4p6
 * Idea: 
 * - Can you see a tree here? :).
 * - First we recursively devide the histogram into rectangles. First rectangle will be the widest, then 
 *   highest rectangle with the bottom side coincide with the bottom side of the histogram. After removing this
 *   rectangle, we recursively do the same with the left histograms.
 * - Now a tree. Each rectangle represent a node. There will be an edge if 2 rectangle shared a side, and
 *   the lower rectangle will be the parent.
 * - Now for the dp part. dp[u][i] = the number of way to put i kinds of gases into the subtree rooted at u.
 *   First we just combine the dp state of the children into the parent, this part is simple. Then we
 *   tried to fill the rest. Let's width[u] and height[u] be the side of the represented rectangle by node u.
 *   If we already have x gases from the children, and we wanted y more gases filled at the u's rectangle,
 *   then there will be (comb(width[u] - x, y) * comb(height[u], y) * y!) ways to do it, where comb(n, k)
 *   is combination function.
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

#define maxn 555
#define rem ((llong) 1e9+7)
#define maxa 1010101

llong mpow(llong base, llong exp) {
    llong ans = 1;
    for (; exp > 0; exp >>= 1) {
        if (exp & 1) (ans *= base) %= rem;
        (base *= base) %= rem;
    }
    return ans;
}

llong fac[maxa], rev_fac[maxa];
void precal_fac() {
    fac[0] = rev_fac[0] = 1;
    rep1(i, maxa - 1) {
        fac[i] = 1ll * i * fac[i - 1] % rem;
        rev_fac[i] = mpow(fac[i], rem - 2);
    }
}

llong comb(int n, int k) {
    if (k > n) return 0;
    return fac[n] * rev_fac[k] % rem * rev_fac[n - k] % rem;
}

int n, k;
int a[maxn];

int n_nodes = 0;
int width[maxn], height[maxn];
vector<int> gr[maxn];
int build_tree(int left, int right, int lower_bound) {
    if (left > right) return -1;
    int cur_node = n_nodes++;
    int upper_bound = *min_element(a + left, a + right) + 1;
    width[cur_node] = right - left;
    height[cur_node] = upper_bound - lower_bound;

    // clog << "tree "<< "(" << left << ", " << right << ") " << cur_node << ' ' << width[cur_node] << ' ' << height[cur_node] << endl;

    for (int i = left; i <= right; ++i) {
        if (i != right and a[i] >= upper_bound) continue;
        int f = i - 1;
        while (f >= left and a[f] >= upper_bound) --f;
        if (++f == i) continue;
        gr[cur_node].push_back(build_tree(f, i, upper_bound));
        // clog << "add child " << cur_node << ' ' << gr[cur_node].back() << endl;
    }

    return cur_node;
}

vector<llong> dp[maxn];
void dfs(int u) {
    dp[u].resize(width[u] + 1);
    dp[u][0] = 1;

    for (auto v: gr[u]) {
        dfs(v);
        vector<llong> new_dp(width[u] + 1);
        rep(i, len(dp[u]))
            rep(f, len(dp[v])) {
                if (i + f >= len(new_dp)) continue;
                (new_dp[i + f] += dp[u][i] * dp[v][f]) %= rem;
            }
        
        dp[u] = move(new_dp);
    }

    vector<llong> final_dp(width[u] + 1);
    rep(i, len(dp[u])) {
        rep(f, len(dp[u])) {
            if (i + f >= len(dp[u])) continue;
            llong x = dp[u][i];
            llong y = comb(width[u] - i, f) * comb(height[u], f) % rem * fac[f] % rem;
            (final_dp[i + f] += x * y) %= rem;
        }
    }

    dp[u] = move(final_dp);
    // clog << "dp " << u << ": ";
    // for (auto i: dp[u]) clog << i << ' ';
    // clog <<endl;
}

int main(void) {
    precal_fac();
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> k;
    rep(i, n) cin >> a[i];

    int root = build_tree(0, n, 1);
    dfs(root);
    cout << dp[root][k];

    return 0;
}

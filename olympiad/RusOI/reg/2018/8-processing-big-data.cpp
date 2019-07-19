/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem link: https://codeforces.com/gym/101714. It is the 8th problem - Обработка больших данных.
 * Idea:
 * - If we build a segment tree over all the cells of the memory, then every correct segment 
 *   is managed by exactly one node on that tree.
 * - We should "STORE" the parent before the child, because if we "STORE" the parent after "STORING"
 *   the child, we just cancel the child's work.
 * - We can decide the value to be "STORED" at the parent from what we should "STORE" at the child:
 *   suppose at the immediate left child of the i-th node, we "STORE" value x, and at the right child
 *   we "STORE" value y, then at the i-th node, we can just "STORE" the value x and remove the "STORE"
 *   operation of the left child, or we "STORE" the value y and remove the operation of the right child.
 *   especially, if x == y, then we just "combine" 2 operation into 1.
 * 
 * So for small subtask, we can calculate dp[i][x] - the minimum number of "STORE" operations we need to put the state
 * of the node i to the state described in the problem statement, and the "STORE" operation at the node i-th is the
 * value x.
 *
 * For large K (the number in the input), we can save the segment tree discretely, so a node is a leaf when it contains
 * the same value for every cell in it. We will end up having O(n * k) leaves, since every segment can be breaked down
 * into O(log(segment tree length)) = O(log(2^k)) = O(k). For each leaf, we bring its value to the root, so each
 * leaf will be processed in O(k) times. So overall complexity is O(n*k^2) 
 *
 * NOTE: I was attemped to use map, but in order to reduce the time complexity, I switched to vector but keep the old code,
 * so there are a lot of lines where I use iterator instead of just a simple counting variable.
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
#define inf ((llong)INT_MAX)
// #define rand __rand
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

vector<pair<int, llong>> min_complement(vector<pair<int, llong>> const& vals) {
    vector<llong> st;
    st.push_back(inf);
    for (auto i = vals.rbegin(); i != vals.rend(); ++i) 
        st.push_back(min(st.back(), i->yy));

    vector<pair<int, llong>> ans;
    llong pref_min = inf;
    for (auto& i: vals) {
        st.pop_back();
        ans.emplace_back(i.xx, min(pref_min, st.back()));
        pref_min = min(pref_min, i.yy);
    }
    return ans;
}

template<class U, class V>
bool cmp_second(const pair<U, V>& x, const pair<U, V>& y) { return x.yy == y.yy ? x.xx < y.xx : x.yy < y.yy; }

// [l; r)
struct itnode {
    int l, r;
    int val;
    unique_ptr<itnode> lchild, rchild;

    itnode(int l_, int r_, int val_ = -1) : l(l_), r(r_), val(val_) {}
    void assign(int al, int ar, int aval) {
        if (l >= ar or al >= r) return ;
        if (al <= l and r <= ar) {
            val = aval;
            return ;
        }
        brach();
        lchild->assign(al, ar, aval);
        rchild->assign(al, ar, aval);
    }

    void brach() {
        assert(l < r) ;
        int mid = (l + r) / 2;
        if (!lchild) lchild.reset(new itnode(l, mid));
        if (!rchild) rchild.reset(new itnode(mid, r));
    }

    vector<pair<int, llong>> process() {
        if (!lchild) {
            assert(val != -1);
            return {{val, 1}};
        }
        vector<pair<int, llong>> dp;
        auto ldp = move(lchild->process());
        auto rdp = move(rchild->process());
        llong l_min = min_element(ldp.begin(), ldp.end(), cmp_second<int, llong>)->yy;
        llong r_min = min_element(rdp.begin(), rdp.end(), cmp_second<int, llong>)->yy;
        auto l_min_comp = min_complement(ldp);
        auto r_min_comp = min_complement(rdp);

        int f = 0;
        for (auto i: ldp) {
            for (;f < len(rdp) and rdp[f].xx < i.xx; ++f) {
                dp.emplace_back(rdp[f].xx, inf);
            }
            if (f < len(rdp) and rdp[f].xx == i.xx) ++f;
            dp.emplace_back(i.xx, inf);
        }
        for (; f < len(rdp); ++f) dp.emplace_back(rdp[f].xx, inf);

        auto ldp_it = ldp.begin(), rdp_it = rdp.begin();
        auto l_min_comp_it = l_min_comp.begin(), r_min_comp_it = r_min_comp.begin();
        for (auto& i: dp) {
            while (ldp_it != ldp.end() and ldp_it->xx < i.xx) ++ldp_it, ++l_min_comp_it;
            while (rdp_it != rdp.end() and rdp_it->xx < i.xx) ++rdp_it, ++r_min_comp_it;
            bool has_l = ldp_it != ldp.end() and ldp_it->xx == i.xx;
            bool has_r = rdp_it != rdp.end() and rdp_it->xx == i.xx;
            assert(has_l or has_r);
            if (has_l and has_r) {
                i.yy = min(i.yy, ldp_it->yy + rdp_it->yy - 1);
                i.yy = min(i.yy, ldp_it->yy + r_min_comp_it->yy);
                i.yy = min(i.yy, l_min_comp_it->yy + rdp_it->yy);
            }
            if (!has_r) i.yy = min(i.yy, ldp_it->yy + r_min);
            if (!has_l) i.yy = min(i.yy, l_min + rdp_it->yy);
        }

        // clog << l << ' ' << r << ": ";
        // for (auto i: dp) clog << i.xx << ":" << i.yy << ' ';
        // clog << endl;
        return move(dp);
    }
};


int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int k, n, m;
    cin >> k >> n >> m;
    auto root = make_unique<itnode>(0, 1 << k);
    int cur = 0;
    rep(i, n) {
        int c, v; cin >> c >> v;
        root->assign(cur, cur + c, v);
        cur += c;
    }
    auto res = root->process();
    llong ans = min_element(res.begin(), res.end(), cmp_second<int, llong>)->yy;
    cout << ans;

    return 0;
}

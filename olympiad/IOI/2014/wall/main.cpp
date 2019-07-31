/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://wcipeg.com/problem/ioi1412
 * Idea; 
 * - It is clear that we should you segment tree (with lazy propagation).
 * - Each node of the segment tree, we should store a range: low and high - the minimum and maximum
 *   values of all elements in this node.
 * - The lazy information of each node should also be a range with 2 values above.
 *   When we apply the lazy information to the node's data, we change the node's data
 *   to the its intersection with the lazy range. If they are not intersected, then
 *   we assign this node's values to the end point of the lazy range, which is 
 *   closer to the current data range.
 * - When we got a query of the 1st type, we update the range with `low` = `height`, and `high = infinity`.
 *   For the second type, `low` = 0, and `high = height`.
 */
#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

/**
 * Author: Tran Quang Loc
 * For usage, please visit: https://github.com/quangloc99/CompetitiveProgramming/tree/master/prepared-code/segment-tree
 */

template<typename TVal>
struct segtree_node {
    TVal val;
    int left, right;
    std::unique_ptr<segtree_node<TVal>> lchild, rchild;
    
    segtree_node(int left_, int right_)
        : left(left_), right(right_) {}

    void combine_children() {
        val.combine(*lchild.get(), *rchild.get());
    }

    inline bool expand() {
        if (right - left <= 1) return false;
        if (lchild) return true;
        int mid = left + (right - left) / 2;
        lchild.reset(new segtree_node(left, mid));
        rchild.reset(new segtree_node(mid, right));
        return true;
    }

    void propagate() { 
        if (expand()) {
            lchild->val.add_lazy(*this, *lchild);
            rchild->val.add_lazy(*this, *rchild);
        }
        val.apply_lazy(*this);
    }

    void set_values(std::function<TVal(int)> const& get_val) {
        if (right - left == 1) val = get_val(left);
        if (!expand()) return ;
        lchild->set_values(get_val);
        rchild->set_values(get_val);
        combine_children();
    }

    bool inside(int l, int r) { return l <= left and right <= r; }
    bool outside(int l, int r) { return l >= right or left >= r; }

    operator TVal&() { return val; }
    operator bool() { return !lchild; }
};


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

struct range {
    int low, high;
    range(int low_ = 0, int high_ = INT_MAX) : low(low_), high(high_) {}
};

range combine(const range& a, const range& b) {
    if (b.low > a.high) return range(b.low, b.low);
    if (b.high < a.low) return range(b.high, b.high);
    return range(max(a.low, b.low), min(a.high, b.high));
}

struct itval {
    range data, lazy;
    itval(): data(0, 0), lazy(0, 0) {}
    void apply_lazy(bool) {
        data = ::combine(data, lazy);
        lazy = range();
    }

    void add_lazy(const itval& par_val, bool) {
        lazy = ::combine(lazy, par_val.lazy);
    }

    void combine(const itval& l_val, const itval& r_val) {
        data = ::combine(l_val.data, r_val.data);
    }
};

void upd(segtree_node<itval>* root, int l, int r, range const& val) {
    root->propagate();
    if (root->outside(l, r)) return ;
    if (root->inside(l, r)) {
        root->val.lazy = combine(root->val.lazy, val);
        return ;
    }
    upd(root->lchild.get(), l, r, val);
    upd(root->rchild.get(), l, r, val);
    root->combine_children();
}

void iterate(segtree_node<itval>* root, function<void(range const&)> const& callback) {
    root->propagate();
    if (root->right - root->left == 1) {
        callback(root->val.data);
        return ;
    }
    iterate(root->lchild.get(), callback);
    iterate(root->rchild.get(), callback);
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, k;
    cin >> n >> k;
    auto root = new segtree_node<itval>(0, n);

    root->set_values([](int i) { return itval(); });

    rep(i, k) {
        int type, left, right, height; cin >> type >> left >> right >> height;
        upd(root, left, right + 1, (type == 1 ? range(height, INT_MAX) : range(0, height)));
    }

    iterate(root, [](range const& val) {
            cout << val.low << '\n';
    });

    return 0;
}

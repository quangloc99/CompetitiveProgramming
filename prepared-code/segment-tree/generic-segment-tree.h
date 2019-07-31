/**
 * Author: Tran Quang Loc
 * For usage, please visit:
 */

#include<bits/stdc++.h>

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


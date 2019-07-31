# Generic segment tree implementation.
A prepared code for segment tree to use in Competitive programming.

## Why
This is a very often used data structure in Competitive programming, but in order
to use it, about 50 or 70 lines of codes must be written during the contest. That
is a lot of times. Most of competitive programmer now prefers to use prepared
code. I think I should make my version too.

There are already many implementations for generic segment tree on the internet,
but they somehow lack of usage and not really generic. My goal here is to make 
a data structure that can be customized in as much details as it can, but still required
simple code.

## Usage
### Declare your own data type.
There is only 1 class in the file `generic-segment-tree.h`: `segtree_node`. To use
it, you must define your own data type class for each node of the segment tree. Let's
say your data class is `Val`. `Val` must have the following template:
```c++
struct Val {
    // define your other data here, including your lazy data.
    // and also a constructor if you want.
    
    // a method to combine 2 children node value. The `this` object here is 
    // the data of the parent node.
    void combine(const Val& l_val, const Val& r_val) { }
    // if you want the full information of the node, use the version below instead.
    // void combine(const segtree_node<Val>& l_node, const segtree_node<Val>& r_node) {}

    // a method to apply the lazy propagation on the current node.
    void apply_lazy(const segtre_node<Val>& current_node) {}
    // if you *don't* want the full information of the current node, use the version below instead
    // void apply_lazy(bool) {}

    // a method to add the lazy information from the parent node to the child node.
    // `this` object is the data of the child node.
    void add_lazy(const segtree_node<Val>& parent, const segtree_node<Val>& child) { }
    // as the above, if you don't need the full information of each node, you
    // can replace each argument with `const Val&` or if you don't need it, replace with
    // `bool`
};
```

### Declare your tree
Then declare you tree like this:
```c++
segtree_node<Val> root = new segtree_node<Val>(0, n);
```
To see which methods the `segtree_node` has, please see the file.
*Note*: `left` and `right` properties of `segtree_node` is the range `[left, right)`
(left inclusive, right exclusive). I do this since it was the C++ way
for **iterator**, and it is my preferred way.

### Declare the query.
I did not write `get` or `update` query, because if I did so, I also need them 
to be universal enough to control every details. But by doing that, the code will
eventually become complex and lengthy. There are also the case where we need
more than these 2 operations, and we also want to control the breaking
and continuing conditions when we do our query.

So instead, I added some methods that helps write the query functions easily.

The function should have the following template:
```c++
`your-data-type` query(segtree_node<Val>* root, int l, int r, ...) {
    root->propagate();  // propagate the lazy information
    if (root->outside(l, r)) {
        // you probably will return the function from here, since
        // the root is outside of the range we do the query
    }
    if (root->inside(l, r)) {
        // add your new value to the lazy information.
        return `some-value`;
    }
    auto l_val = query(root->lchild.get(), l, r, ...);
    auto r_val = query(root->rchild.get(), l, r, ...);
    // you can also call root->combine_children() if you want to update 
    // root->val from root's children's vals.
    return 'combine the l_val and r_val`;
}
```

### Notes
- This segment tree handles the tree discretely. So you can even create a tree
to manage large range (like 10^9).
- I use smart pointer to manage the children pointers. If you want to do 
some modification for persistent segment tree, you should change the smart pointer
from `unique_ptr` to `shared_ptr`.

## Example
### Increase a range's values by x; range sum query.
#### The `Val struct`
```c++
struct Val {
    int sum, lazy;
    Val() : sum(0), lazy(0) {}

    void combine(const Val& l_val, const Val& r_val) {
        sum = l_val.sum + r_val.sum;
    }

    void apply_lazy(const segtree_node<Val>& node) {
        sum += lazy * (node.right - node.left);
        // since `this` object is the data of the current object, you can also write
        // node.val.sum += node.val.lazy * (node.right - node.left);

        lazy = 0;
    }

    void add_lazy(const Val& par_val) {
        lazy += par_val.lazy;
    }
};
```

#### Modify query
```c++
void modify(segtree_node<Val>* root, int l, int r, int x) {
    root->propagate();
    if (root->outside(l, r)) return ;
    if (root->inside(l, r)) {
        root->val.lazy += x;
        return ;
    }
    modify(root->lchild.get(), l, r, x);
    modify(root->rchild.get(), l, r, x);
    root->combine_children();
}
```

#### Sum query
```c++
int get_sum(segtree_node<Val>* root, int l, int r) {
    root->propagate();
    if (root->outside(l, r)) return 0;
    if (root->inside(l, r)) return root->val.sum;
    return get_sum(root->lchild.get(), l, r) + get_sum(root->rchild.get(), l, r);
}
```

### Code example
The code example for problem IOI 2014 can be found [here](https://github.com/quangloc99/CompetitiveProgramming/blob/master/olympiad/IOI/2014/wall/main.cpp)


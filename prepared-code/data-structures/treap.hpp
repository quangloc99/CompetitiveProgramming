/**
 * Author: Tran Quang Loc (darkkcyan)
 * Require rand() function.
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/SPOJ/ORDERSET.cpp
 */
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    return (T)(rng() % range);
}
struct treap_node {
    treap_node *lchild, *rchild;
    long long val, prio;
    treap_node(long long v = 0, long long p = rand())
        : lchild(0), rchild(0), val(v), prio(p), size(1) {}
    // prevent NULL value;
    friend int get_size(treap_node* u) { return u ? u->size : 0; }
    friend void upd_size(treap_node* u) {
        if (!u) return ;
        u->size = 1 + get_size(u->lchild) + get_size(u->rchild);
    }
    private: int size; // made this private also for prevent accidentally using NULL.
};
treap_node* unite(treap_node* u, treap_node* v) {
    if (!u or !v) return u ? u : v;
    if (u->prio > v->prio) {
        u->rchild = unite(u->rchild, v);
        upd_size(u);
        return u;
    } else {
        v->lchild = unite(u, v->lchild);
        upd_size(v);
        return v;
    }
}
// left side: [-inf, val); right side: [val, +inf)
tuple<treap_node*, treap_node*> split(treap_node* root, long long val) {
    if (!root) return {NULL, NULL};
    treap_node* temp;
    if (root->val >= val) {
        tie(temp, root->lchild) = split(root->lchild, val);
        upd_size(root);
        return {temp, root};
    } else {
        tie(root->rchild, temp) = split(root->rchild, val);
        upd_size(root);
        return {root, temp};
    }
}
//--------------------------------------------------
// basic operations.
bool contain(treap_node* root, long long val) {
    if (!root) return false;
    if (root->val == val) return true;
    return contain(val >= root->val ? root->rchild : root->lchild, val);
}
void insert(treap_node*& root, treap_node* new_val) {
    if (!root) return void(root = new_val);
    if (root->prio < new_val->prio) {
        tie(new_val->lchild, new_val->rchild) = split(root, new_val->val);
        root = new_val;
    } else {
        insert(new_val->val >= root->val ? root->rchild : root->lchild, new_val);
    }
    upd_size(root);
}
void erase(treap_node*& root, long long value) {
    if (!root) return ;
    if (root->val == value) {
        auto temp = root;
        root = unite(root->lchild, root->rchild);
        delete temp;
    } else erase(root->val < value ? root->rchild : root->lchild, value);
    upd_size(root);
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://www.spoj.com/problems/ORDERSET/
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
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    return (T)(rng() % range);
}

struct treap_node {
    treap_node *lchild, *rchild;
    llong val, prio;
    treap_node(llong v = 0, llong p = rand())
        : lchild(0), rchild(0), val(v), prio(p), size(1) {}

    // prevent NULL value
    friend int get_size(treap_node* u) { return u ? u->size : 0; }
    friend void upd_size(treap_node* u) {
        if (!u) return ;
        u->size = 1 + get_size(u->lchild) + get_size(u->rchild);
    }

    private: int size; // made this private also for prevent accidentally using NULL
                       // always use get_size function
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
tuple<treap_node*, treap_node*> split(treap_node* root, llong val) {
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

bool contain(treap_node* root, llong val) {
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

void erase(treap_node*& root, llong value) {
    if (!root) return ;
    if (root->val == value) {
        auto temp = root;
        root = unite(root->lchild, root->rchild);
        delete temp;
    } else erase(root->val < value ? root->rchild : root->lchild, value);
    upd_size(root);
}

llong get_kth(treap_node* root, int k) {
    assert(root);
    int left_sz = get_size(root->lchild);
    if (k == left_sz + 1) return root->val;
    if (k <= left_sz) return get_kth(root->lchild, k);
    return get_kth(root->rchild, k - left_sz - 1);
}

int count_smaller(treap_node* root, llong x) {
    if (!root) return 0;
    if (root->val >= x) return count_smaller(root->lchild, x);
    return count_smaller(root->rchild, x) + 1 + get_size(root->lchild);
}

void print_tree(treap_node* root) {
    if (!root) return ;
    clog << "(";
    if (root->lchild) print_tree(root->lchild);
    clog << root->val << ":" << get_size(root)<< ' ';
    if (root->rchild) print_tree(root->rchild);
    clog << ") ";
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    treap_node* root = NULL; 
    int q; cin >> q;
    // int q = 10000;
    while (q--) {
        char query_type; llong num;
        cin >> query_type >> num;
        // query_type = ("IDKC")[rand(4)];
        // num = rand();
        if (query_type == 'I') {
            if (!contain(root, num)) insert(root, new treap_node(num));
        }
        if (query_type == 'D') erase(root, num);
        if (query_type == 'K') {
            if (num > get_size(root)) cout << "invalid\n";
            else cout << get_kth(root, (int)num) << '\n';
        }
        if (query_type == 'C') cout << count_smaller(root, num) << '\n';
        // print_tree(root);
        // clog << endl;
    }

    return 0;
}

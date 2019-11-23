/**
 * Author: Tran Quang Loc
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF932-D12-F.cpp
 * This version is for min lichao. For max lichao, the val of the node need to be initialized with line(0, -inf), 
 * and the function max should be used instead of min.
 */
const long long inf = 1e15;
struct line {
    long long a, b; 
    line(long long a_ = 0, long long b_ = 0) : a(a_), b(b_) {}
    long long operator()(llong x) const { return x * a + b; }
};

int timer = 0;  // increase this value for refreshing the tree. 
                // no need if the tree does not need to be refresh
// [l, r)
struct lichao_node {
    int l, r;
    lichao_node *lchild, *rchild;
    line val;
    int cur_time;  // no need if the tree does not need to be refresh
    
    lichao_node(int l_, int r_)
      : l(l_), r(r_), lchild(0), rchild(0), val(0, inf), cur_time(-1) {
        if (r - l <= 1) return ;
        int mid = (l + r) / 2;
        lchild = new lichao_node(l, mid);
        rchild = new lichao_node(mid, r);
    }

    // no need if the tree does not need to be refresh.
    void refresh() {
        if (cur_time != timer) {
            val = line(0, inf);
            cur_time = timer;
        }
    }

    void upd(line new_line) {
        refresh();
        int m = (l + r) / 2;  // it could be rchild->l, but we should avoid NULL
        bool better_new = new_line(m) < val(m);
        bool intersect_left = better_new != (new_line(l) < val(l));
        if (better_new) swap(new_line, val);
        if (r - l == 1) return ;
        (intersect_left ? lchild : rchild)->upd(new_line);
    }

    llong get(long long x) {
        refresh();
        if (l > x or x >= r) return inf;
        if (r - l == 1) return val(x);
        return min({val(x), lchild->get(x), rchild->get(x)});
    }
};

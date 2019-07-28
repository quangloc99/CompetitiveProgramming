/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/CEOI16_icc
 * Official solution: http://www.ceoi2016.ro/competition/tasks/ (see the solution column)
 * 
 * The official solution I think is similar to my solution, but I see it in different way.
 * The funny thing is, they see everything in bits manipulation, and I didn't.
 * I hope my explaination will be more intuitive than the bits stuffs.
 * 
 * Ideas:
 * - If we know 2 disjoin subsets that have only 1 edge connecting 1 node 
 *   from 1 set to 1 node of the other, we can binary search to find the exact
 *   connected 2 nodes.
 * - To find this 2 disjoint subsets, we can do like the following:
 *   0) Suppose we have 2 sets A and B, that we know for sure, there
 *      is no edge connecting 2 nodes from A and B before.
 *   1) Now, we do query on these 2 sets. If the answer is 1, then concrats :).
 *   2) Otherwise, we now know that the edge connects 2 nodes that either in A or B.
 *     but instead of making 2 separated queries, we can do like this:\
 *     we can break A and B into 4 smaller subsets, let's call them
 *     A1, A2, B1 and B2. Then we can merge A1 and B1, A2 and B2 together
 *     and do query on this 2 new sets.
 *   3) If we still get 0, then we continue divide these 4 sets into 8:
 *     A11, A12, A21, A22, B11, B12, B21, B22. Then, we merge A11, A21, B12, B21
 *     together, merge A12, A22, B12, B22 together and do the query.
 *   4) ...  
 * - If these steps are still confused, then here is a little explaination.
 *   At the 2) step, what we want to know does the new edge connect 
 *   a node from A1 and A2, or connect a node from B1 or B2.
 *   So instead of 2 queries, we do 1 instead.
 * 
 * If we stop our observations here, we can build an algorithm that works with
 * about n * (3 * log(n)) queries, since doing binary search cost 2 * log(n) queries.
 * To reduce the number of queries, here is my observation: the finding step above
 * can be seen as a bianry tree. Let's say the root have depth 0,
 * and depth of the child is the depth of parent plus 1. For each step `d` above, we
 * were actually merging left nodes at depth `d`, as well as the right nodes
 * at the same depth. Let's say we found our 2 disjont sets at the `D` depth.
 * So D queries were made. To reduce the size of 2 sets, I can actually find
 * 2 exact nodes at the depth `D` like the following:
 * - From root, we do query like above with nodes at depth `D`, but now only
 *   with the left child. If the answer is yes, then we go to the left child
 *   and repeat this step until we reach depth `D` . Otherwise we go to the right
 *   and do the same.
 * 
 * If our tree is random, then reducing the size of 2 sets to the size of the nodes
 * helps us nothing. But if we use, say, huffman tree, then the height of the tree
 * as well as the size of each nodes is actually balanced! Overall we just
 * did n * (2 * log(n)) queries. This can be proven, but I think it is not
 * neccessay.
 *
 * And about the performance, for each edge, my solution cost only 12 or 13 queries,
 * and when submitted, the overall queries is only about 1200 or 1300!
 */
#include <bits/stdc++.h>
#include "icc.h"
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

///////////////////////////////////////////////////////////////////////////////////////////
#define maxn 111

// It is more convenient to have the query() function this way.
// int query_cnt = 0;
template<class container>
int query(const container& a, const container& b) {
    // ++query_cnt;
    // clog << "query: " << query_cnt << ": " << endl;
    // clog << "{"; for (auto i: a) clog << i << ", "; clog << "} " << endl;
    // clog << "{"; for (auto i: b) clog << i << ", "; clog << "} " << endl;
    int cpy_a[len(a)], cpy_b[len(b)];
    copy(a.begin(), a.end(), cpy_a);
    copy(b.begin(), b.end(), cpy_b);
    return query(len(a), len(b), cpy_a, cpy_b);
}

// forward declaration for 100 points and 61 ponits.
// By doing this we won't have any variable's name confliction.
// See the implemenations below.
namespace solution_100_points { void run(int n); }
namespace solution_61_points { void run(int n); };

// The run() function needed to be implement. Change 100 to 61 for the other solution.
void run(int n) { solution_100_points::run(n); }

////////////////////////////////////////////////////////////////////////////////
// Below is some code used in both solution.
struct Dsu {
    int set_head[maxn];
    list<int> data[maxn];

    Dsu() {}
    Dsu(int n) {
        rep(i, n) {
            set_head[i] = i;
            data[i].assign(1, i);
        }
    }
    
    int find_set(int u) { return u == set_head[u] ? u : set_head[u] = find_set(set_head[u]); }
    void join(int u, int v) {
        u = find_set(u);
        v = find_set(v);
        if (u == v) return ;
        if (rand(2)) swap(u, v);
        set_head[u] = v;
        data[v].splice(data[v].end(), data[u]);  // this is O(1)
    }
     
    template<class container, class result_container = container>
    result_container get_all_members(const container& a) {
        result_container ans;
        for (auto i: a) {
            copy(data[i].begin(), data[i].end(), back_inserter(ans));
        }
        return ans;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////

namespace solution_100_points {
    struct huffman_node {
        list<int> data;
        int depth;
        huffman_node *lchild, *rchild;

        huffman_node(const list<int>& data_) :data(data_), lchild(nullptr), rchild(nullptr) {}
        huffman_node(huffman_node* lchild_, huffman_node* rchild_)
            : lchild(lchild_), rchild(rchild_)
        {
            copy(lchild->data.begin(), lchild->data.end(), back_inserter(data));
            copy(rchild->data.begin(), rchild->data.end(), back_inserter(data));
        }

        ~huffman_node() {
            if (lchild) delete lchild;
            if (rchild) delete rchild;
        }

        void calculate_depth(int cur_depth = 0) {
            depth = cur_depth;
            if (lchild) lchild->calculate_depth(cur_depth + 1);
            if (rchild) rchild->calculate_depth(cur_depth + 1);
        }

        tuple<list<int>, list<int>> get_data(int level) {
            if (level < depth) { return {{}, {}}; }
            if (level == depth) {
                return { lchild ? lchild->data : list<int>(), rchild ? rchild->data : list<int>()};
            }

            list<int> left, right, child_left, child_right;
            if (lchild) {
                tie(left, right) = lchild->get_data(level);
            }
            if (rchild) {
                tie(child_left, child_right) = rchild->get_data(level);
                left.splice(left.end(), child_left);
                right.splice(right.end(), child_right);
            }
            return {left, right};
        }
    };

    tuple<list<int>, list<int>> split_in_half(const list<int>& u) {
        list<int> res[2];
        bool cur = 0;
        for (int i: u) {
            res[cur].push_back(i);
            cur = !cur;
        }
        return {res[0], res[1]};
    }

    huffman_node* build_huffman_tree(vector<list<int>> const& datas) {
        struct cmp {
            bool operator()(huffman_node* u, huffman_node* v) {
                if (len(u->data) == len(v->data)) return u < v;
                return len(u->data) < len(v->data);
            }
        };

        set<huffman_node*, cmp> pq;
        for (auto& i: datas) pq.insert(new huffman_node(i));

        while (len(pq) > 1) {
            auto u = *pq.begin(); pq.erase(pq.begin());
            auto v = *pq.begin(); pq.erase(pq.begin());
            pq.insert(new huffman_node(u, v));
        }
        return *pq.begin();
    }

    void run(int n) {
        Dsu dsu(n + 1);
        rep(edge, n - 1) {
            // query_cnt = 0;
            vector<list<int>> subsets;
            rep1(i, n)
                if (dsu.find_set(i) == i)
                    subsets.push_back(dsu.data[i]);
            auto huff_root = build_huffman_tree(subsets);
            huff_root->calculate_depth();
            list<int> left, right;

            int joined_level = 0;
            for (; ;++joined_level) {
                tie(left, right) = huff_root->get_data(joined_level);
                if (query(left, right) == 1) break;
            }


            huffman_node* joined_node = huff_root;
            while (joined_node->depth < joined_level) {
                assert(joined_node->lchild or joined_node->rchild);
                if (!joined_node->lchild) joined_node = joined_node->rchild;
                else if (!joined_node->rchild) joined_node = joined_node->lchild;
                else {
                    tie(left, right) = joined_node->lchild->get_data(joined_level);
                    if (query(left, right)) joined_node = joined_node->lchild;
                    else joined_node = joined_node->rchild;
                }
            }
            list<int> u, v;
            tie(u, v) = joined_node->get_data(joined_level);
            while (len(u) > 1) {
                tie(left, right) = split_in_half(u);
                if (query(left, v)) u = left;
                else u = right;
            }
            while (len(v) > 1) {
                tie(left, right) = split_in_half(v);
                if (query(left, u)) v = left;
                else v = right;
            }
            int a = *u.begin(), b = *v.begin();
            setRoad(a, b);
            dsu.join(a, b);
            // clog << query_cnt << endl;
            delete huff_root;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

namespace solution_61_points {
    tuple<vector<int>, vector<int>> random_split(vector<int> a) {
        random_shuffle(a.begin(), a.end(), rand<int>);
        auto mid = a.begin() + len(a) / 2;
        return {vector<int>(a.begin(), mid), vector<int>(mid, a.end())};
    }
     
    void run(int n) {
        Dsu dsu(n + 1);
     
        int n_query = 0;
        rep(edge_num, n - 1) {
            vector<int> head;
            rep1(i, n) 
                if (dsu.find_set(i) == i) head.push_back(i);
     
            vector<int> u, v;
            do {
                tie(u, v) = random_split(head);
                u = dsu.get_all_members(u);
                v = dsu.get_all_members(v);
                ++n_query;
            } while (query(u, v) == 0);
     
            vector<int> left, right;
     
            while (len(u) > 1) {
                tie(left, right) = random_split(u);
                ++n_query;
                if (query(left, v) == 1) u = move(left);
                else u = move(right);
            }
     
            while (len(v) > 1) {
                tie(left, right) = random_split(v);
                ++n_query;
                if (query(u, left) == 1) v = move(left);
                else v = move(right);
            }
     
            int a = u[0], b = v[0];
            setRoad(a, b);
            dsu.join(a, b);
            // clog << edge_num << ' ' << a << ' ' << b <<  ' ' << n_query << endl;
        }
    }
};

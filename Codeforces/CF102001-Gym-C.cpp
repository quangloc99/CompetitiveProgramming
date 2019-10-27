/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://codeforces.com/gym/102001/problem/C
 * Idea: this is actually a problem about De bruijn sequence construction. You can find it on Wikipedia.
 * The hard part is, the number length is big!!!
 * But the problem did not ask to find the full sequence. Actually, the length of
 * the sequence we must find is n + k - 1. So in this solution, I set the limit of the sequence length to
 * k. When the length exceed this number, is auto matically stop. In worse case, out found sequence will have
 * length 2 * k, which is fine :). After that I add n - 1 more number 0 in the sequence, and finally we just 
 * need to print the first n + k - 1 numbers.
 *
 * But what about the vertices. It is actually not possible to represent a number with 10^5 digits here. So instead
 * I use the rolling hash code of the number. Firstly, we can calculate the hash code of the next number easily, and
 * secondly, even the number have the same hash code, the algorithm will work fine:
 * - In this case the number of vertices is probably big, so we don't need the whole sequence.
 * - If node u and v have the same hash code, and we visit u first, so in the case of v, we can just get the next
 *   edge of u. In other word, u and v share the same edge list. There will (probably) not be much such pair of vertices in the
 *   algorithm.
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

#define rem ((llong)1e9 + 7)
#define maxlg 19
#define maxk 10
int n, m, k;
int alphabet[maxk];
llong pow_m;
int limit_length;

struct node;
map<llong, node*> gr;
struct node {
    node* ancestor[maxlg];
    int child_cnt;
    llong hash_code;
    int n_branch;

    node(llong hc, int branch, node* parent = NULL): child_cnt(0), hash_code(hc), n_branch(branch) {
        if (!parent) parent = this;
        ancestor[0] = parent;
        rep(i, maxlg - 1) ancestor[i + 1] = ancestor[i]->ancestor[i];
    }

    node* next_child() {
        int cur = child_cnt ++;
        auto child_hc = cal_child_hashcode(cur);
        if (!gr.count(child_hc)) gr[child_hc] = new node(child_hc, cur, this);
        return gr[child_hc];
    }

    node* get_ancestor(int level) {
        node* ans = this;
        for (int i = 0; level > 0; level >>= 1, ++i) {
            if (level & 1) ans = ans->ancestor[i];
        }
        return ans;
    }

    llong cal_child_hashcode(int branch) {
        llong ans = (hash_code - get_ancestor(n - 2)->n_branch * pow_m) * m + branch;
        ans %= rem;
        if (ans < 0) ans += rem;
        return ans;
    }
};

list<node*> get_euler_tour(node* u) {
    static int scope = 0;
    ++scope;
    // clog << "---" << scope << endl;
    list<node*> ans;
    while (u->child_cnt != m) {
        ans.emplace_back(u->next_child());
        u = ans.back();
        // clog << u->hash_code << ' ' << u->n_branch << endl;
        if (len(ans) > limit_length) return ans;
    }

    for (auto it = ans.begin(); it != ans.end(); ++it) {
        ans.splice(next(it), get_euler_tour(*it));
        if (len(ans) > limit_length) return ans;
    }
    // clog << "---" << scope << endl;
    --scope;
    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    rep(i, m) cin >> alphabet[i];
    list<node*> ans;
    if (n == 1) {
        rep(i, m) ans.push_back(new node(0, i));
    } else {
        {
            pow_m = 1;
            llong base = m;
            for (llong temp = n - 2; temp > 0; temp >>= 1) {
                if (temp & 1) (pow_m *= base) %= rem;
                (base *= base) %= rem;
            }
        }

        auto root = gr[0] = new node(0, 0);
        limit_length = k;
        ans = get_euler_tour(root);
        rep(i, n - 1) ans.push_front(root);
    }


    assert(len(ans) >= n + k - 1);
    ans.resize(n + k - 1);
    for (auto it: ans) cout << alphabet[it->n_branch];

    return 0;
}

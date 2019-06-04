// NOTE: I decreased `f` by 1 after reading the input.
/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: this code contains the solution for subtask 2, 3 and 4. See the code for the subtask 3.
 * It is ez too see that if we just take 1 node from level 0 and remove all node that could not be reach from this node, 
 * e got a binary tree! And the reverse is also true: we get 1 node at the last level and remove all node that could not reach this node.
 * From this observation we can easily calculate the number of node that reachs this node, the number of node that
 * can be reached from this node, and finally check if some node A can be reached from node B.
 * 
 * But if there are 2 nodes A and B that A can be reached from B, and both are blacks, then the 2 informations above of these nodes
 * may be overlap. And dp solution is used here to solved this problem.
 *
 * That is the subtask 3, the subtask 4 is really similar with the help of data structure. There is easier implementation using map, but
 * here I used trie inside a trie.
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
// mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

#define maxn 30
int f, t;
vector<int> black[maxn];
void read_inp() {
    ifstream cin("fft2d.in");
    cin >> f >> t;
    --f;
    while (t--) {
        int u, v; cin >> u >> v;
        black[u].push_back(v);
    }
}

llong brute_force_solve();

struct trie1 {
    llong s;
    unique_ptr<trie1> child[2];
    trie1(): s(0) {}
    void insert(llong val, int num, int lv) {
        if (lv == 0) {
            s += val;
            return ;
        }
        auto& c = child[num & 1];
        if (!c) c.reset(new trie1());
        c->insert(val, num >> 1, lv - 1);
    }
    
    llong query(int num, int lv) {
        auto& c = child[num & 1];
        return s + (lv and c ? c->query(num >> 1, lv - 1) : 0);
    }
};

struct trie2 {
    trie1 data;
    unique_ptr<trie2> child[2];
    trie2() {}

    list<trie1*> insert(int num, int lv) {
        if (lv == 0) return {&data};
        auto& c = child[(num << 1 >> lv) & 1];
        if (!c) c.reset(new trie2());
        auto ans = c->insert(num, lv - 1);
        ans.push_front(&data);
        return move(ans);
    }

    trie1* query(int num, int lv) {
        if (lv == 0) return &data;
        auto& c = child[(num << 1 >> lv) & 1];
        return c ? c->query(num, lv - 1) : nullptr;
    }
};

llong solve_sub4() {
    llong ans = (1 << f);
    ans *= ans;

    trie2 ds;
    for (int i_lv = f + 1; i_lv--; )
    for (auto i_val: black[i_lv]) {
        llong contrib = 1 << (f - i_lv);
        int mask = i_val >> (f - i_lv);
        auto t1 = ds.query(mask, i_lv);
        if (t1) contrib -= t1->query(i_val, f);
        ans -= contrib * (1 << i_lv);

        for (auto i: ds.insert(mask, i_lv)) {
            i->insert(contrib, i_val, f - i_lv);
        }
        // clog << i_lv << ' ' << i_val << ' ' << contrib << endl;
    }

    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    read_inp();
    ofstream("fft2d.out") << solve_sub4();

    return 0;
}

//////////////////////////////////////////////////////////////////////

int first_bits(int num, int cnt, int n_bit = f) {
    return num >> (n_bit - cnt);
}

int last_bits(int num, int cnt) {
    return num & ((1 << cnt) - 1);
}

llong solve_sub3() {
    llong ans = (1 << f);
    ans *= ans;

    vector<llong> dp[maxn];

    for (int i_lv = f + 1; i_lv--; )
    for (auto i_val: black[i_lv]) {
        llong contrib = 1 << (f - i_lv);
        for (int g_lv = i_lv + 1; g_lv <= f; ++g_lv) 
        for (int g = 0; g < len(black[g_lv]); ++g) {
            int g_val = black[g_lv][g];
            if (first_bits(i_val, i_lv) != first_bits(g_val, i_lv)) continue;
            if (last_bits(i_val, f - g_lv) != last_bits(g_val, f - g_lv)) continue;
            contrib -= dp[g_lv][g];
        }
        dp[i_lv].push_back(contrib);
        ans -= contrib * (1 << i_lv);
        // clog << i_lv << ' ' << i_val << ' ' << contrib << endl;
    }

    return ans;
}

llong solve_sub2() {  // sub 1 included
    if (f > 16) return -1;
    vector<bitset<1 << 15>> source(1 << f);
    rep(i, len(source)) {
        source[i][i] = 1;
    }
    for (auto i: black[0]) source[i][i] = 0;

    for (int lv_sz = 1 << (f - 1), lv = 1; lv_sz >= 1; lv_sz >>= 1, ++lv) {
        // clog << "--------------" << endl;
        // clog << lv_sz << ' ' << lv << endl;
        rep(i, len(source)) {
            int nxt = i ^ lv_sz;
            if (i > nxt) continue;
            auto temp = source[i];
            source[i] |= source[nxt];
            source[nxt] |= temp;
            // clog << i << ' ' << (i ^ (lv_sz)) << ' ' << bitset<8>(source[i].to_ullong())<< endl;
        }
        for (auto i: black[lv]) source[i].reset();
    }

    llong ans = 0;
    for (auto& i: source) {
        ans += i.count();
        // clog << bitset<8>(i.to_ullong()) << endl;
    }
    return ans;
}

llong brute_force_solve() {
    if (f <= 16) return solve_sub2();
    else return solve_sub3();
}

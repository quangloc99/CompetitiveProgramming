/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/COCI17_rima
 * Idea:
 * - It is easy to see that 2 words rhythm with each other if their lengths differ no more than 1 character.
 * - I group all the string that rhythm with each other and has the same lengths into 1 node.
 * - 2 nodes will be connected if there are 1 word in 1 node rhtyhm with the other word in the other node.
 * - It is also easy to see that the graph will form a tree. Our sequence is an "almost" simple path
 *   between 2 nodes. "Almost" here means that, if we are trying to go from nodes u and v, and
 *   there are more than 1 word in lca(u, v), we can also try going up 1 times, from 1 word in lca(u, v), 
 *   to 1 word in the parent of lca(u, v), then go back to lca(u, v). This "almost" part cost me lost of
 *   time debugging, since I thought that it must be a simple path.
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

#define rem ((llong) 1000027241)
#define base 31
#define maxn 501010

int dsu[maxn];
int set_cnt[maxn];
int find_set(int u) { return u == dsu[u] ? u : dsu[u] = find_set(dsu[u]); }
void join(int u, int v) {
    if (rand() & 1) swap(u, v);
    u = find_set(u); v = find_set(v);
    if (u == v) return ;
    set_cnt[u] += set_cnt[v];
    dsu[v] = u;
}

struct word {
    llong hash_code;
    int length;
    word(llong hc = 0, int l = 0): hash_code(hc), length(l) {}
    word(const string& s, int l = -1): hash_code(0), length(l) { 
        if (length == -1) length = len(s);
        rep(i, length) 
            ((hash_code *= base) += s[i] - 'a' + 1) %= rem;
    }

    word add(char ch) {
        return word((hash_code * base + ch - 'a' + 1) % rem, length + 1);
    }
};

bool operator< (const word& u, const word& v) {
    if (u.length == v.length) return u.hash_code < v.hash_code;
    return u.length < v.length;
}

int n;
word words[maxn], pref_words[maxn];
bool has_parent[maxn];
vector<int> gr[maxn];

pair<int, int> dfs(int u, int p = -1) {
    bool can_go_to_parent = set_cnt[u] > 1 and p != -1;
    pair<int, int> ans(set_cnt[u] + can_go_to_parent, set_cnt[u]);
    for (auto v: gr[u]) {
        if (v == p) continue;
        auto temp = dfs(v, u);
        ans.xx = max(ans.xx, temp.xx);
        ans.xx = max(ans.xx, temp.yy + ans.yy + can_go_to_parent);
        ans.yy = max(ans.yy, set_cnt[u] + temp.yy);
    }
    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) {
        string s; cin >> s;
        reverse(s.begin(), s.end());
        pref_words[i] = word(s, len(s) - 1);
        words[i] = pref_words[i].add(s.back());
    }

    rep(i, n) {
        dsu[i] = i;
        set_cnt[i] = 1;
    }

    map<word, int> pref_pos;
    rep(i, n) {
        if (!pref_pos.count(pref_words[i])) 
            pref_pos[pref_words[i]] = i;
        else 
            join(i, pref_pos[pref_words[i]]);
    }

    rep(i, n) {
        if (!pref_pos.count(words[i])) continue;
        int u = find_set(i);
        int v = find_set(pref_pos[words[i]]);
        gr[u].emplace_back(v);
        has_parent[v] = 1;
    }

    int ans = 0;
    rep(i, n) {
        if (i != find_set(i)) continue;
        if (has_parent[i]) continue;
        ans = max(ans, dfs(i).xx);
    }
    
    cout << ans;

    return 0;
}

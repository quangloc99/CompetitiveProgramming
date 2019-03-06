// graph, dp, bitmask
/**
 * @Author Tran Quang Loc (darkkcyan)
 * Solution idea:
 *   It is easy to see that we need to find/construct a tree that:
 *   - Has minimum cost/weight.
 *   - Contains all the judges' vertices and the destination.
 *
 *   To find such tree, I used dp with bitmasking. The main thing is done in the function process and doTrace.
 *   Here is some explainations for the variables that I used:
 *   - dp[mask] = the minimum possible weight of the tree that contains all the vertices represent by mask.
 *   - backEdge[mask] is the pair of vertices (u, v) such that: dp[mask] = dp[mask ^ (1 << u)] + gr[u][v]
 *     In other word, dp[mask & (1<<u)] is the optimal dp state that lead to dp[mask].
 *   - dir[u] = the parent of node u in the constructed tree. Node that destination is the root.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long
#define len(x) ((int)x.size())

#define maxn 21
int n, m, judgeCnt;
int dest;
llong gr[maxn][maxn];
int judges[maxn];

llong dp[1 << maxn];
pair<int, int> backEdge[1 << maxn];
int dir[maxn];

void process(int mask) {
    if (dp[mask] != -1) return;
    if (__builtin_popcount(mask) == 1) {
        dp[mask] = (mask >> dest) & 1 ? 0 : LLONG_MAX;
        return;
    }
    dp[mask] = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        if ((~mask >> i) & 1) continue;
        int subMask = mask ^ (1 << i);
        process(subMask);
        if (dp[subMask] == LLONG_MAX) continue;
        llong curAns = LLONG_MAX;
        int back = -1;
        for (int f = 0; f < n; ++f) {
            if ((~subMask >> f) & 1) continue;
            if (gr[f][i] == -1) continue;
            if (curAns < dp[subMask] + gr[f][i]) continue;
            curAns = dp[subMask] + gr[f][i];
            back = f;
        }
        if (dp[mask] < curAns) continue;
        dp[mask] = curAns;
        backEdge[mask] = {i, back};
    }
}

void doTrace(int mask) {
    if (dp[mask] == 0) return;
    dir[backEdge[mask].first] = backEdge[mask].second;
    doTrace(mask ^ (1 << backEdge[mask].first));
}

int main(void) {
    if (ifstream("../main.inp").good()) {
        freopen("../main.inp", "r", stdin);
        freopen("../main.out", "w", stdout);
    }
    ios_base::sync_with_stdio(0); cin.tie(0);
    int testCase = 1;
    while (cin >> n) {
        if (n == -1) break;
        cin >> dest;
        --dest;
        memset(gr, -1, sizeof(gr));
        cin >> m;
        for (int i = 0; i < m; ++i) {
            int u, v; llong cost;
            cin >> u >> v >> cost;
            --u; --v;
            gr[u][v] = gr[v][u] = cost;
        }
        int src = 1 << dest;
        cin >> judgeCnt;
        for (int i = 0; i < judgeCnt; ++i) {
            cin >> judges[i];
            --judges[i];
            src ^= 1ll << judges[i];
        }
        memset(dp, -1, sizeof(dp[0]) * (1 << n));
        int minMask = (1 << n) -1;
        process(minMask);
        tuple<llong, int, int> ans = {dp[minMask], __builtin_popcount(minMask), minMask};
        for (int i = 0; i < (1 << n); ++i) {
            if ((i & src) != src) continue;
            tuple<llong, int, int> curAns = {dp[i], __builtin_popcount(i), i};
            ans = min(ans, curAns);
        }

        doTrace(get<2>(ans));
        printf("Case %d: distance = %lld\n", testCase++, get<0>(ans));
        for (int i = 0; i < judgeCnt; ++i) {
            printf("   ");
            for (int f = judges[i]; f != dest; f = dir[f]) {
                printf("%d-", f + 1);
            }
            printf("%d\n", dest + 1);
        }
        printf("\n");
    }

    return 0;
}

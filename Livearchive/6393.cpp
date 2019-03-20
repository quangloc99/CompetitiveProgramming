/**
 * Author: Tran Quang Loc (darkkcyan)
 * Submit it on Kattis: https://icpc.kattis.com/problems/assembly (in order to prevent the EOF and EOL problem with Livearchive OJ)
 * Idea: the most important part (for me) is that we can reflect a cell beside rotate. So, it is easy to see that we can
 * just add cell above or to the right of the existed cell, because if we, for example, try to place below a cell, we can just reflect
 * that 2 cells and add the new cell above the existed cell. And also because we just need to add cell above or to the right, then
 * there is only one resticted for a cell (rather than 4).
 * So with that information, if we can reach a cell with a certain state (rotated or reflected) that we reached it before, we can 
 * conclude that this is unbounded. So this is the part of cycle dectection.
 * The other part is we should add more node inorder to reduce the number of edges. In my code, I add nodes represents the "Connector label",
 * so for each cell (that rotated and reflected), I just need to add an edges between the nodes representing the cell and the connectors.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()

struct cell {
  int edges[4];  // this is actually connecter label, but I convert it into number.
                 // from -26 to 26. 0 represents "00".
  cell() {}
  cell(const cell& other) {
    memcpy(edges, other.edges, 4 * sizeof(edges[0]));
  }

  cell(const string& s) {
    for (int i = 0; i < 8; i += 2) {
      edges[i / 2] = 0;
      if (s[i] == '0') continue;
      edges[i / 2] += s[i] - 'A' + 1;
      if (s[i + 1] == '-') edges[i / 2] *= -1;
    }
  }

  void rotate() {
    int t = edges[0];
    rep(i, 3) edges[i] = edges[i + 1];
    edges[3] = t;
  }

  void reflect() {
    swap(edges[0], edges[2]);
  }

  llong toNum() const {
    llong ans = 0;
    rep(i, 4) ans = ans * 53 + edges[i] + 26;
    return ans;
  }
};

#define N_CASES ((llong)(53 * 53 * 53 * 53 + 10))
vector<llong> all;
bitset<N_CASES> has;
int n;
vector<vector<int>> gr;

vector<int> state;
bool hasCycle(int u) {
  state[u] = 1;
  for (auto v: gr[u]) {
    if (state[v] == 2) continue;
    if (state[v] == 1) return true;
    if (hasCycle(v)) return true;
  }
  state[u] = 2;
  return false;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    while (cin >> n) {
      gr.assign(53, vector<int>());
      rep(i, n) {
        string s; cin >> s;
        cell t(s);
        rep(ref, 2) {
          rep(rot, 4) {
            llong num = t.toNum();
            if (!has[num]) {
              has[num] = 1;
              all.push_back(num);
              int u = len(gr);
              gr.push_back(vector<int>());
              if (t.edges[0]) gr[u].push_back(t.edges[0] + 26);
              if (t.edges[3]) gr[u].push_back(t.edges[3] + 26);
              if (t.edges[1]) gr[-t.edges[1] + 26].push_back(u);
              if (t.edges[2]) gr[-t.edges[2] + 26].push_back(u);
            }
            t.rotate();
          }
          t.reflect();
        }
      }

      state.assign(len(gr), 0);
      bool ans = 0;
      for (int i = 53; i < len(gr); ++i) {
        assert(state[i] != 1);
        if (state[i] == 2) continue;
        if (hasCycle(i)) {
          ans = 1;
          break;
        }
      }

      cout << (ans ? "un" : "") << "bounded\n";
      for (auto i: all) has[i] = 0;
      all.clear();
    }


    return 0;
}

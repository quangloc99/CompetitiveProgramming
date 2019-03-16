/**
 * @Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/3462
 *
 * Idea: let's call S the sum of the survived mushroom after all the tree has fallen. We need to compute EV(S)
 * And because S = X1 + X2 + ... Xm (Xi is 0 if mushroom i-th is destroyed otherwise it equals to the i-th mushroom's power).
 * so, EV(S) = EV(X1) + EV(X2) + ... + EV(Xm)
 * Now we just need to compute each EX(Xi) and then sum them up. Let's compute EV(X1).
 * Let cal(i) be the EV of X1 if there are only tree numbered from 1 to i in the forest. We can easily got the formula:
 *
 * cal(i) = cal(i - 1) * (
 *     (first mushroom lies between trees[i].x - trees[i].h and trees[i].x) ? (1 - trees[i].probL) :
 *     (fisrt mushroom lies between trees[i].x and trees[i].x + trees[i].h) ? (1 - trees[i].probr) :
 *     1
 * )
 * 
 * But because we have m mushroom, and the formula is similar for all the mushroom, we can use some data structure like segment tree to handle them
 * simultaneously.
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

#define uptr unique_ptr
#define mku make_unique

// [l; r)
struct itnode {
  uptr<itnode> lchild, rchild;
  int l, r;
  long double sum, lz;

  itnode(int _l, int _r, function<long double(int)> getArr) : l(_l), r(_r), sum(getArr(_l)), lz(1) {
    if (r - l <= 1) return ;
    int mid = (l + r) / 2;
    lchild = mku<itnode>(l, mid, getArr);
    rchild = mku<itnode>(mid, r, getArr);
    sum = lchild->sum + rchild->sum;
  }

  void dolz() {
    sum *= lz;
    if (lchild) lchild->lz *= lz;
    if (rchild) rchild->lz *= lz;
    lz = 1;
  }

  void upd(int rangel, int ranger, long double val) {
    dolz();
    if (rangel >= r or l >= ranger) return;
    if (rangel <= l and r <= ranger) {
      lz *= val;
      dolz();
      return;
    }

    lchild->upd(rangel, ranger, val);
    rchild->upd(rangel, ranger, val);
    sum = lchild->sum + rchild->sum;
  }

  // no need for get
};

struct Tree {
  int x, h, probl, probr;
};

#define maxn 101010
int n, m;
Tree trees[maxn];
pair<int, int> mush[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    rep(i, n) cin >> trees[i].x >> trees[i].h >> trees[i].probl >> trees[i].probr;
    rep(i, m) cin >> mush[i].xx >> mush[i].yy;
    sort(mush, mush + m);

    auto root = mku<itnode>(0, m, [](int i) { return (long double)mush[i].yy; });
    rep(i, n) {
      int rangel, ranger;

      rangel = (int)(lower_bound(mush, mush + m, make_pair(trees[i].x - trees[i].h, INT_MIN)) - mush);
      ranger = (int)(lower_bound(mush, mush + m, make_pair(trees[i].x, INT_MIN)) - mush);
      root->upd(rangel, ranger, 1.0 - trees[i].probl / 100.0);

      rangel = (int)(upper_bound(mush, mush + m, make_pair(trees[i].x, INT_MAX)) - mush);
      ranger = (int)(upper_bound(mush, mush + m, make_pair(trees[i].x + trees[i].h, INT_MAX)) - mush);
      root->upd(rangel, ranger, 1.0 - trees[i].probr / 100.0);
    }

    cout << fixed << setprecision(10) << root->sum;

    return 0;
}

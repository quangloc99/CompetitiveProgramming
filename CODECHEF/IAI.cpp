/**
 * Author: Tran Quang Loc (darkkcyan)
 * Idea: it is easily to see that we only need one or 2 intervals inorder to get all the intersections.
 * This code include both brutefore (for small sub) and the correct solution that cover both sub.
 * See the bruteforce sub first. After that you shoud notice the function `get_sum` here, and it has the form of a linear function.
 * So indead, I use convex hull trick with dynamic data structure.
 * For the rest, please see the full code :).
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
mt19937 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

#define maxn 101010
int n, m;
llong a[maxn];
pair<int, int> segs[maxn];

llong sum_ai[maxn];
llong sum_a[maxn];

// sample code: https://github.com/quangloc99/CompetitiveProgramming/blob/master/Codeforces/CF931-D12-F.cpp

struct line;
typedef multiset<line>::iterator mslitor;
struct line {
  llong k, b;
  mutable mslitor curNode;

  line() {}
  line(llong ak, llong ab): k(ak), b(ab) {}

  llong cal(llong x) const { return x * k + b; }
};

bool check(const line& l1, const line& l2, const line& t) {
  return (t.k - l2.k) * (l1.b - l2.b) >= (t.b - l2.b) * (l1.k - l2.k);
}

multiset<line>* containerSet = 0;
bool operator<(const line& u, const line& v) {
  if (!containerSet) return u.k > v.k;
  auto nxt = next(u.curNode);
  if (nxt == containerSet->end()) return false;
  return u.cal(v.b) >= nxt->cal(v.b);
}

struct LowerLineContainer : multiset<line> {  // maintains lower hull
  bool isAboveHull(iterator itor) {
    if (size() == 1) return false;
    if (itor == begin()) return (itor->k == next(itor)->k) and (itor->b > next(itor)->b);
    if (next(itor) == end()) return itor->b > prev(itor)->b and itor->k == prev(itor)->k;
    return check(*prev(itor), *itor, *next(itor));
  }

  void insert(line x) {
    mslitor nn = multiset<line>::insert(x);
    nn->curNode = nn;
    if (isAboveHull(nn)) {
      erase(nn);
      return ;
    }
    while (next(nn) != end() and isAboveHull(next(nn))) erase(next(nn));
    while (nn != begin() and isAboveHull(prev(nn))) erase(prev(nn));
  }

  llong query(llong x) {
    if (!size()) return LLONG_MAX - 1000;
    containerSet = this;
    llong ans = lower_bound(line(0, x))->cal(x);
    containerSet = 0;
    return ans;
  }
};

struct UpperLineContainer : LowerLineContainer {
    inline void insert(line x) {
        x.k *= -1; x.b *= -1;
        LowerLineContainer::insert(x);
    }
    inline llong query(llong x) { 
        return - LowerLineContainer::query(x); 
    }
};

struct itnode {
    unique_ptr<itnode> lchild, rchild;
    int l, r;
    UpperLineContainer data;
    itnode() {}
    itnode(int l_, int r_): l(l_), r(r_) {
        if (r - l <= 1) return ;
        int mid = (l + r) / 2;
        lchild.reset(new itnode(l, mid));
        rchild.reset(new itnode(mid, r));
    }
};

void insert(itnode* root, int id, line const& x) {
    if (id >= root->r or id < root->l) return; 
    root->data.insert(x);
    if (root->lchild) insert(root->lchild.get(), id, x);
    if (root->rchild) insert(root->rchild.get(), id, x);
}

llong query(itnode* root, int l, int r, llong x) {
    if (l >= root->r or root->l >= r) return LLONG_MIN;
    if (l <= root->l and root->r <= r) {
        // for (auto i: root->data) clog << "(" << -i.k << ' ' << -i.b << ") ";
        // clog << endl;
        return root->data.query(x);
    }
    return max(
            query(root->lchild.get(), l, r, x),
            query(root->rchild.get(), l, r, x)
    );
}

llong get_sum(int l, int r) {
    llong sai = sum_ai[r + 1] - sum_ai[l];
    llong sa = sum_a[r + 1] - sum_a[l];
    return sai - sa * l;
}

void read_inp() {
    cin >> n >> m;
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> segs[i].xx >> segs[i].yy;
}

void gen_test() {
    ofstream out("main.inp");
    n = (int)(rand() % 5) + 1;
    m = (int)(rand() % 100) + 1;
    out << "1\n";
    out << n << ' ' << m << '\n';
    rep(i, n) {
        a[i] = (int)rand() % 10000 - (int)rand() % 10000;
        out << a[i] << ' ';
    }
    out << '\n';

    rep(i, m) {
        segs[i] = {rand() % n + 1, rand() % n + 1};
        if (segs[i].xx > segs[i].yy) swap(segs[i].xx, segs[i].yy);
        out << segs[i].xx << ' ' << segs[i].yy << '\n';
    }
}

llong brute() {
    llong ans = LLONG_MIN;
    rep(i, m) for (int f = i; f < m; ++f) {
        int l = max(segs[i].xx, segs[f].xx);
        int r = min(segs[i].yy, segs[f].yy);
        if (l <= r) ans = max(ans, get_sum(l - 1, r - 1));
        // clog << i << ' ' << f << ' ' << ans << endl;
    }
    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int ntest; cin >> ntest;
    // int ntest = 1000;
    while (ntest--) {
        read_inp();
        // gen_test();

        sum_ai[0] = sum_a[0] = 0;
        rep(i, n) {
            sum_a[i + 1] = sum_a[i] + a[i];
            sum_ai[i + 1] = sum_ai[i] + a[i] * (i + 1) ;
        }
        vector<vector<int>> seg_ends(n);
        rep(i, m) {
            seg_ends[segs[i].xx - 1].push_back(segs[i].yy);
        }

        itnode root(0, n);

        llong ans = LLONG_MIN;
        rep(i, n) {
            sort(seg_ends[i].begin(), seg_ends[i].end());
            for (auto f: seg_ends[i]) {
                insert(&root, f - 1, line(-sum_a[f], sum_ai[f]));
                llong temp = query(&root, i, f, i);
                ans = max(ans, temp + sum_a[i] * i - sum_ai[i]);
                // clog << i << ' ' << temp << ' ' << ans << ' ' << sum_a[i] << ' ' << sum_ai[i] << endl;
            }
        }
        cout << ans << '\n';

        // llong t = brute();
        // cout << t << ' '<< t - ans  << endl;
        // assert(ans == t);
    }

    return 0;
}

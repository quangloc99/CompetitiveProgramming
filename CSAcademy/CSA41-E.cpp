/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://csacademy.com/contest/archive/task/candles/solution/
 */
#include <bits/stdc++.h>

using namespace std;

#define llong long long 
#define ii pair<int, int>
#define xx first
#define yy second
#define len(v) ((int)v.size())
#define rep(i,n) for (int i = -1; ++i < n; )
#define rep1(i,n) for (int i = 0; i++ < n; )

template<typename x> ostream& operator<< (ostream& cout, const vector<x>& a) {
    cout << "[";
    if (len(a)) cout << a[0];
    rep1(i, len(a) - 1) cout << ", " << a[i];
    return cout << "]";
}

template<typename u, typename v> ostream& operator<<(ostream& cout, const pair<u, v>& a) {
    return cout << "(" << a.xx << ", " << a.yy << ")";
}

#define maxn 101010
int n, m;
int h[maxn];
int c[maxn];

llong it[maxn * 4], leftmost[maxn * 4], lz[maxn * 4];
void build(int i = 1, int l = 0, int r = n - 1) {
  // clog << i << ' ' << l << ' ' << r << endl;
  lz[i] = 0;
  leftmost[i] = h[l];
  if (l == r) it[i] = h[l];
  if (l >= r) return;
  int mid = (l + r) / 2;
  build(i << 1, l, mid);
  build(i << 1 | 1, mid + 1, r);
  it[i] = it[i << 1] + it[i << 1| 1];
}

void lazy(int i, int l, int r) {
  if (l > r) return ;
  it[i] += (r - l + 1) * lz[i];
  leftmost[i] += lz[i];
  if (l < r) {
    lz[i << 1] += lz[i];
    lz[i << 1 | 1] += lz[i];
  }
  lz[i] = 0;
}

void update(int ql, int qr, int val, int i = 1, int l = 0, int r = n - 1) {
  lazy(i, l, r);
  if (ql > qr or l > r or r < ql or qr < l) return ;
  if (ql <= l and r <= qr) {
    lz[i] += val;
    lazy(i, l, r);
    return ;
  }
  int mid = (l + r) / 2;
  update(ql, qr, val, i << 1, l, mid);
  update(ql, qr, val, i << 1 | 1, mid + 1, r);
  it[i] = it[i << 1] + it[i << 1 | 1];
  leftmost[i] = leftmost[i << 1];
}

llong get(int ql, int qr, int i = 1, int l = 0, int r = n - 1) {
  lazy(i, l, r);
  if (ql > qr or l > r or r < ql or qr < l) return 0;
  if (ql <= l and r <= qr) {
    return it[i];
  }
  int mid = (l + r) / 2;
  llong ans = get(ql, qr, i << 1, l, mid) + get(ql, qr, i << 1 | 1, mid + 1, r);
  return ans;
}

int findlastge(int val, int i = 1, int l = 0, int r = n - 1) {
  lazy(i, l, r);
  if (l == r) return l;
  int mid = (l + r) / 2;
  // // clog << i << ' ' << l << ' ' << r << ' ' << val << ' ' << leftmost[i << 1 | 1] << endl;
  lazy(i << 1 | 1, mid + 1, r);
  if (leftmost[i << 1 | 1] >= val) return findlastge(val, i << 1 | 1, mid + 1, r);
  return findlastge(val, i << 1, l, mid);
}

void printcontent() {
  clog << "IT content: " << endl;
  rep(i, n) clog << get(i, i) << ' ';
  clog << endl;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n >> m;
  // n = 99998;
  // m = 100000;
  rep(i, n) {
    cin >> h[i];
    // h[i] = 1;
  }
  // h[0] = 100000;
  rep(i, m) {
    cin >> c[i];
    // c[i] = 2;
  }
  sort(h, h + n, greater<int>());
  //clog << vector<int>(h, h + n) << endl;
  
  build();
  // printcontent();
  rep(ans, m) {
    int i = c[ans];
    if (i > n) {
      cout << ans;
      return 0;
    }
    int t = (int)get(i - 1, i - 1);
    int pos = findlastge(t + 1);
    int left;
    if (get(pos, pos) == t) {
      left = i;
    } else {
      update(0, pos, -1);
      //clog << "up " << 0 << ' ' << pos << endl;
      left = i - pos - 1;
    }
    int lasteqt = findlastge(t);
    update(lasteqt - left + 1, lasteqt, -1);
    //clog << "up " << lasteqt - left + 1 << ' ' << lasteqt << endl;
    //clog << "query " << i << ' ' << t << ' ' << pos << ' ' << left << ' ' << lasteqt << endl;
    // printcontent();
    if (get(n - 1, n- 1) < 0) {
      cout << ans;
      return 0;
    }
  }
  cout << m;

 
  return 0;
}

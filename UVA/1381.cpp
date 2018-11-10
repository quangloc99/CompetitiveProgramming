/**
 * Authors: Tran Quang Loc (darkkcyan)
 * Editorial: https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/UVA/UVA_1381.txt
 */
#include <bits/stdc++.h>
using namespace std;

#define n 16
#define llong long long 
#define rep(i,n) for (int i = -1; ++ i < (n); )
#define len(x) ((int)x.size())

template<typename x> ostream& operator<<(ostream& cout, const vector<x>& a) {
  cout << "[";
  if (len(a)) cout << a[0];
  rep(i, len(a) - 1) cout << ", " << a[i + 1];
  return cout << "]";
}

vector<int> sm4b16, sm8b16;
vector<int> onbit[1 << n];
vector<int> edges[1 << n];
void preprocess() {
  rep(i, (1 << 16))  {
    if (__builtin_popcount(i) == 4) sm4b16.push_back(i);
    if (__builtin_popcount(i) == 8) sm8b16.push_back(i);
  }
  for (auto i: sm4b16) 
    rep(f, n) 
      if ((i >> f) & 1) onbit[i].push_back(f);
  for (auto i: sm4b16)
    for (auto f: sm4b16)
      if (!(i & f)) {
        edges[i].push_back(f);
      }
}

vector<int> getallsum(vector<int> vals) {
  sort(vals.begin(), vals.end());
  vector<int> ans;
  do {
    ans.push_back(0);
    rep(i, len(vals)) {
      ans.back() += (i + 1) * vals[i];
      clog << i + 1 << " * " << vals[i] << ' ';
    }
    clog << "= " << ans.back() << endl;
  } while (next_permutation(vals.begin(), vals.end()));
  return ans;
}

int countpair(const vector<int>& x, const vector<int>& y) {
  static int cnt[1024 * 14] = {0};
  for (auto i: x) cnt[i] ++;
  int ans = 0;
  for (auto i: y) ans += cnt[i];
  for (auto i: x) cnt[i] --;
  return ans;
}


int a[n + 10];
vector<int> getvals(int mask) {
  vector<int> ans;
  for (auto i: onbit[mask]) ans.push_back(a[i]);
  return ans;
}

vector<int> sum4b[1 << n];
llong ans8b[1 << n];

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  preprocess();

  int testnum = 1;
  while (1) {
    cin >> a[0];
    if (a[0] == 0) break;
    //if (testnum > 1) cout << '\n';
    cout << "Case " << testnum ++ << ": ";
    rep(i, n - 1) cin >> a[i + 1];
    for (auto i: sm4b16) {
      sum4b[i] = getallsum(getvals(i));
      clog << bitset<16>(i) << ": " << sum4b[i] << ' ' << getvals(i) << endl;
    }
    for (auto i: sm8b16) ans8b[i] = 0;
    for (auto i: sm4b16)
      for (auto f: edges[i]) {
        ans8b[i | f] += countpair(sum4b[i], sum4b[f]);
      }
    //clog << "OK" << endl;
    llong ans = 0;
    for (auto i: sm8b16) {
      //clog << bitset<16>(i)  << ' ' << ans8b[i] << ' ' << ans8b[(1 << n) - i - 1] << endl;
      ans += ans8b[i] * ans8b[((1 << n) - 1) ^ i];
    }
    ans /= 8;
    cout << ans << '\n';
  }

  return 0;
}

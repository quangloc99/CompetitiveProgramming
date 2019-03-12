/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://codeforces.com/blog/entry/21755
 * At the first look I think the editorial is the same as my solution, but I will try to explain it quicker here.
 * Idea:
 * The result = expected value of the overall rank = (exptected value of the number of participants that has less overall rank) + 1
 *            = (total number of participants that has less overall rank for every case) / (the total number of cases) + 1
 * The total number of cases is clearly ((m - 1)!)^n (for each competition, we got (m - 1)! cases).
 * For the last part, we can do it as follow: Let's call it X because it shorter :). If we found some rank set of 1 participant that
 * has less overall rank, we can see that this set can be found in ((m - 2)!)^n cases (with the similar explaination like the above),
 * but that is only for 1 person, so we must multiply this number by (m - 1) for (m - 1) person. And also, all rank set are indipendent
 * to each other, so if we let S be the number of rank set that has less overall rank, then X = S * ((m - 2)!)^n * (m - 1)
 * The final formula is:
 * S * ((m - 2)!)^n * (m - 1) / ((m - 1)!)^n = S / (m - 1)^(n - 1)
 *
 * In order to compute S, we can use dp with the state (i, sum) is the number of way to choose the rank for 1 person in the first i
 * competitions such that the overrall rank of that person is sum. The time complexity is O(n * m * maxSumValue) = O(n*m * n*m)
 * because we have n competitions, for each competitions, we have (m - 1) = O(m) way to choose the rank. With the help of prefix sum,
 * we can reduce the complexity downto O(n^2 * m) and compute each dp value for O(1).
 *
 * In my code, I don't compute S, but S/(m - 1)^n and then multiply the result by m - 1, because if we just compute S and (m - 1)^(n-1)
 * alone, then the result will be much bigger hence the precision error.
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

#define maxn 111
#define maxval 100100
int n, m;
int a[maxn];
long double cnt[maxval];
long double presum[maxval];

void compPreSum() {
    presum[0] = 0;
    rep(i, n * m)
      presum[i + 1] = presum[i] + cnt[i];
}

// [l; r)
long double getSum(int l, int r) {
  l = max(l, 0);
  r = max(r, 0);
  return presum[r] - presum[l];
}


int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    if (m == 1) {
      cout << 1;
      return 0;
    }
    int s = 0;
    rep(i, n) {
      cin >> a[i];
      s += a[i];
    }

    cnt[0] = 1;
    rep(comp, n) {
      compPreSum();
      for (int i = n * m + 1; i--;) {
        cnt[i] = getSum(i - m, i);
        if (i >= a[comp]) cnt[i] -= cnt[i - a[comp]];
        cnt[i] /= m - 1;
      }
    }
    compPreSum();
    cout << fixed << setprecision(12) << getSum(0, s) * (m - 1) + 1;

    return 0;
}

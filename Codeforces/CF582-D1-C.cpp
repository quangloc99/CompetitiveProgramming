/**
 * Author: Tran Quang Loc (darkkcyan)
 * My idea is slightly difference from the editorial, because I saw that they used 2 pointer techinque, but I didn't.
 * So here is the idea.
 *
 * The main part is same as the editorial: we should process each sequence's length first. That is, we fixed s, and count the l.
 * Let's construct a binary string s. S[i] is 1 if a[i] is the biggest number among all a[(i + k * s) % n] for all k.
 * So our valid pair (l, s) will represent consecutive sequences that all of the number are 1. And this part is ez. 
 * If we found there is a group of 1 that has length x, then there we can take (x - s + 1) position to be l.
 *
 * The other part is smartly take and process the s. We can see that for any k, k * s % n will equal to i * gcd(s, n) for some i.
 * So we can group the numbers s by their gcd with n, and process the gcd only. To find the final result, please see my code :).
 * Finally, like in the editorial, the time complexity is O(d(n) * n) approximately = O(n ^ (4/3))
 *
 * Implementation notes: to calculate the sum of every (x - s + 1) faster, instead of using binary search, I used lookup table.
 * Another note that to split the string s into group of 1, I used strtok, because it has less code (and for me, it is less bug) but
 * the time complexity stay the same. It just looks like a replacement for split in Java and Python but for C.
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

#define maxn 201010
int n;
int a[maxn];
vector<int> group[maxn];

llong process(int x) {
  vector<int> lookup(n + 1, 0);
  vector<llong> prefsum(1, 0);
  rep(i, len(group[x])) {
    lookup[group[x][i]] = i + 1;
    prefsum.push_back(prefsum[i] + group[x][i]);
  }

  rep1(i, n) if (lookup[i] == 0)
    lookup[i] = lookup[i - 1];

  assert(n % x == 0);
  vector<int> maxInRing(x, -1);
  rep(i, n) {
    int pos = i % len(maxInRing);
    maxInRing[pos] = max(maxInRing[pos], a[i]);
  }

  // cheated a little bit using string :)
  string s(n, '0');
  rep(i, n) {
    if (a[i] == maxInRing[i % len(maxInRing)])
      s[i] = '1';
  }
  // clog << s << endl;
  vector<int> groupOf1;
  for (char* t = strtok(&s[0], "0"); t; t = strtok(0, "0"))
    groupOf1.push_back((int)strlen(t));

  assert(len(groupOf1));
  if (len(groupOf1) == 1 and groupOf1[0] == n)
    return 1ll * n * len(group[x]);

  if (s[0] == '1' and s.back() == '1') {
    groupOf1[0] += groupOf1.back();
    groupOf1.pop_back();
  }
  llong res = 0;
  for (auto i: groupOf1) {
    int l = lookup[i];
    res += 1ll * (i + 1) * l - prefsum[l];
  }

  return res;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n;
    rep(i, n) cin >> a[i];
    rep1(i, n - 1) {
      group[__gcd(i, n)].push_back(i);
    }

    llong ans = 0;
    rep1(i, n) if (len(group[i]))
      ans += process(i);

    cout << ans;

    return 0;
}

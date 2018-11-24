/**
 * Author: Tran Quang Loc (darkkcyan)
 * Update: found indetailed editorial: http://staff.bath.ac.uk/masjhd/NWERC/news/nwerc2017slides.pdf, but a little different than mine
 * since I used a data structure to manage my data.
 *
 * Solution: (similar to editorial)
 * First observation: the root must be coprime to all number in the array. If there are more then 1 such number, pick any (I pick the 1st one).
 * To proof that work for any such number, we can change from an existed solution into a solution with desired root, with the help of
 * operation-like AVL-tree's rotate.
 * When we got the root, we split the original array into 2 parts, solve them to get the left and right subtree (Devide and conquer). The thing
 * here is, 2 parts may has different sizes, if each part is procced in O(n) then the final time is O(n^2). So the trick here,
 * we process (not solve, just process) the part that has more element with the time complexity equals to the smaller part. By doing so,
 * we can still archive O(nlogn).
 * To do so, I need a data structure that has add and remove operations. At the beginning we add all element to the data structure
 * then when processing 2 parts, first we remove all the smaller part's elements out of the data structure, then process the bigger part, then after
 * we are back to the current recursive state, we again add all the smaller part's elements and process the smaller one. Since after process all the
 * bigger part, all the elements are removed, so we don't need to reset the data structure.
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long /*{{{*/
#define ii pair<int, int>
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )

template<class x> vector<typename x::value_type> tovec(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> tovec(u a, u b) { return vector<ptrtype(u)>(a, b); }

template<typename x> ostream& operator<<(ostream& cout, const vector<x>& a) {
  cout << "[";
  if (len(a)) cout << a[0];
  rep1(i, len(a) - 1) cout << ", " << a[i];
  return cout << "]";
}

template<typename u, typename v> ostream& operator<<(ostream& cout, const pair<u, v>& a) {
  return cout << "(" << a.xx << ", " << a.yy << ")";
}/*}}}*/

#define maxn 1000100
#define maxa ((llong)1e7 + 10)
llong divp[maxa] = {0};
void gendiv() {
  divp[1] = 1;
  for (llong i = 2; i < maxa; ++i) {
    if (divp[i]) continue;
    divp[i] = i;
    for (llong f = (llong)i * i; f < maxa; f += i)
      divp[f] = i;
  }
}

int getdiv(int& x) {
  int ans = (int)divp[x];
  while (x != 1 and x % ans == 0) x /= ans;
  return ans;
}


int n;
int a[maxn];
vector<int> divs[maxn];   // divs[i] contains all prime divisor of a[i]

// data structures
int xpos[maxa] = {0}, cnt[maxa] = {0};  // cnt[i] = the number of prime divisor equal to i of all the number in the ds.
                                        // xpos[i] = the xor-sum of the indexes of all the number in the ds that has i as prime divisor.
                                        // The purpose of xpos is for quickly adding and removing elements. And when cnt[i] == 1, xpos[i] 
                                        // will be the only number in the ds that contains i as prime number, which is the one I care.
                                        // These 2 can be replaced with set<int> but it will cost more time.

int cntcomp[maxn] = {0};                // In short, if cntcomp[i] == divs[a[i]].size(), then a[i] can be the root.
int setsize = 0;                        // The number of element in the ds.

set<int> candidate;                     // Contains all number that can be root.

void add(int p) {
  ++setsize;
  if (a[p] == 1) candidate.insert(p);
  else for (auto i: divs[p]) {
    if (cnt[i] == 1) {
      if (cntcomp[xpos[i]] -- == len(divs[xpos[i]])) candidate.erase(xpos[i]);
    }
    ++cnt[i];
    xpos[i] ^= p;
    if (cnt[i] != 1) continue;
    if (++cntcomp[p] == len(divs[p])) candidate.insert(p);
  }
}

void rem(int p) {
  --setsize;
  if (a[p] == 1) candidate.erase(p);
  else for (auto i: divs[p]) {
    if (cnt[i] == 1) {
      if (cntcomp[p]-- == len(divs[p])) candidate.erase(p);
    }
    --cnt[i];
    xpos[i] ^= p;
    if (cnt[i] != 1) continue;
    if (++cntcomp[xpos[i]] == len(divs[xpos[i]])) candidate.insert(xpos[i]);
  }
}


int ans[maxn];

// Iterative version with the helps of stack. I do these because I have stack-overflow when all the number of the input is 1 and n is big.
// See the other version below.
void process() {
  vector<int> ls, rs, roots;
#define pushstate(l, r, root) do {\
  ls.push_back(l); \
  rs.push_back(r); \
  roots.push_back(root); \
} while (0)

  pushstate(1, n, 0);
  while (len(roots)) {
    int l = ls.back(); ls.pop_back();
    int r = rs.back(); rs.pop_back();
    int root = roots.back(); roots.pop_back();
    if (l > r) continue;
    if (setsize and !len(candidate)) {
      throw "no ans";
      return ;
    }
    if (setsize == 0) for (int i = l; i <= r; ++i) add(i);

    int x = *candidate.begin();
    rem(x);
    ans[x] = root;
    if (l == r) continue;
    if (x - l < r - x) {
      pushstate(l, x - 1, x);
      pushstate(x + 1, r, x);
      for (int i = l; i < x; ++i) rem(i);
    } else {
      pushstate(x + 1, r, x);
      pushstate(l, x - 1, x);
      for (int i = r; i > x; --i) rem(i);
    }
  }
#undef pushstate
}

// The recursive version, I keep it here because it described the idea very well.
void process(int l, int r, int root) {
  if (l > r) return;
  //clog << l << ' ' << r << ' ' << root << endl;
  //clog << tovec(candidate) << endl;
  if (!len(candidate)) {
    throw "no ans";
    return;
  }
  int x = *candidate.begin();
  rem(x);
  ans[x] = root;

  //clog << "found root " << x << ' ' << root<< endl;
  if (l == r) return ;
  if (x - l < r - x) {
    for (int i = l; i < x; ++i) rem(i);
    process(x + 1, r, x);
    //clog << "Add process ";
    for (int i = l; i < x; ++i) add(i);
    //clog << tovec(candidate) << endl;
    process(l, x - 1, x);
  } else {
    for (int i = r; i > x; --i) rem(i);
    process(l, x - 1, x);
    //clog << "Add process ";
    for (int i = r; i > x; --i) add(i);
    //clog << tovec(candidate) << endl;
    process(x + 1, r, x);
  }
}

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  gendiv();

  while (cin >> n) {
    rep1(i, n) cin >> a[i];
    rep1(i, n) {
      divs[i].clear();
      for (int x = a[i], d; (d = getdiv(x)) != 1; ) 
        divs[i].push_back(d);
      add(i);
    }
    try {
      //process(1, n, 0);
      process();
      rep1(i, n) {
        cout << ans[i];
        if (i < n) cout << ' ';
      }
    } catch (const char* x) {
              
      cout << "impossible";
    }
    rep1(i, n) {
      for (auto f: divs[i]) {
        xpos[f] = cnt[f] = 0;
      }
      cntcomp[i] = 0;
    }
    cout << '\n';
    candidate.clear();
    setsize = 0;
  }

  return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://discuss.codechef.com/questions/78425/cyclrace-editorial, but my solution look like the one in the comment.
 * Just a quick note about the part that computes the new line. Let's imagine the graph. Our lines represent the position of
 * the racer depent on time. If we got new update query (time, id, newSpeed), then the racer id-th is at the position
 * lines[id](time) (see the code), and the coordinates on the graph is (time, lines[id](time)).
 * So the new line cross this point, and from here, transform a little, get the new equation.
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

struct line {
  llong k, b;
  line(): k(0), b(0) {}
  line(llong argk, llong argb): k(argk), b(argb) {}
  llong operator()(llong x) { return k* x + b; }
};

bool check(const line& l1, const line& l2, const line& nl) {
  return (long double) (nl.b - l2.b) * (l2.k - l1.k) >= (long double)(nl.k - l2.k) * (l2.b - l1.b);
}

ostream& operator<<(ostream& cout, const line& u) {
  return cout << make_pair(u.k, u.b);
}

#define maxn 101010
line lines[maxn];
int n, q;

int main(void) {
  ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
  cin >> n >> q;
  deque<line> hull;
  hull.push_back(line(0, 0));
  while (q--) {
    int t; cin >> t;
    if (t == 1) {
      int id;
      llong newSpeed, updTime;
      cin >> updTime >> id >> newSpeed;
      //clog << id << " " << newSpeed << " " << updTime << ' ' << lines[id] << ' ' << lines[id](updTime) << '\n';
      lines[id] = line(newSpeed, lines[id](updTime) - newSpeed * updTime);
      while (len(hull) > 1 and check(hull[len(hull) - 2], hull.back(), lines[id])) 
        hull.pop_back();
      hull.push_back(lines[id]);
    } else {
      llong qrTime; cin >> qrTime;
      while (len(hull) > 1 and hull[0](qrTime) <= hull[1](qrTime))
        hull.pop_front();
      cout << hull[0](qrTime) << '\n';
    }
    //clog << tovec(lines + 1, lines + 1 + n) << endl;
    //clog << tovec(hull) << '\n';
  }


  return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * The idea is slightly different from the editorial.
 * Idea:
 * First, I do the query "? 0 0" to determine which one is bigger, and if a < b, then I swap there role (see the do_swap method and
 * everywhere that has swapped variable). Then I solved it from the most to less significant bit.
 * Let's say the current bit is i-th bit and we know for sure bits 29, 28, 27, .. i + 1 (0-indexed). Let's call those known
 * bits a_pref and b_pref. If now I have a > b and I ask for (((a_pref << 1) | 1) << i) and (((b_pref << 1) | 1) << i), that is,
 * we flip the i-th bit, turn off all bits in a_pref and b_pref and keep the other.
 * - If we got 1 then i-th of a and b are the same. To find that bit, we do like above but we just flip the i-th bit of b,
 *   and don't flip a's bit. If answer == 1, then that bit is 1, and if answer == -1, then that bit is 0. From here we
 *   we know a still biger than b, so we continue finding bit i-1.
 * - If we got -1, then this is the bit that "determine" that a > b. So a's i-th bit is 1 and b's bit is 0.
 *   and from here we must also ask another question to find out now which one has the bigger rest, and if a's rest part is smaller
 *   we again do the swapping.
 * - In case we got 0 (that is, a == b), we do like the case when we got 1 to detemine the big.
 *
 * Another note: to use the tester, simple remove the comment in the main function and remove the code below the commnet.
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

struct solver {
  int a_pref, b_pref;
  int cur_lv;
  bool swapped;
  solver(int lv = 30): a_pref(0), b_pref(0), cur_lv(30), swapped(false) {}
  virtual ~solver() {}

  virtual int ask_(int x, int y) {
    cout << "? " << x << ' ' << y << endl;
    int ans; cin >> ans;
    return ans;
  }

  virtual void answer_(int x, int y) {
    cout << "! " << x << ' ' << y << endl;
    exit(0);
  }

private:
  int ask(int x, int y) {
    if (swapped) swap(x, y);
    return ask_(x, y) * (swapped ? -1 : 1);
  }

  void answer(int x, int y) {
    if (swapped) swap(x, y);
    answer_(x, y);
  }

  void do_swap() {
    swap(a_pref, b_pref);
    swapped = !swapped;
  }

  void solve_bigger() {
    --cur_lv;
    if (cur_lv < 0) answer(a_pref, b_pref);
    a_pref <<= 1;
    b_pref <<= 1;
    switch (ask((a_pref | 1) << cur_lv, (b_pref | 1) << cur_lv)) {
      case -1:
        a_pref |= 1;
        if (ask(a_pref << cur_lv, b_pref << cur_lv) == -1)
          do_swap();
        break;
      case 0:
      case 1:
        if (ask(a_pref << cur_lv, (b_pref | 1) << cur_lv) == 1) {
          a_pref |= 1;
          b_pref |= 1;
        }
        break;
    }
    if (swapped) swap(a_pref, b_pref);
    clog << bitset<30>(a_pref) << ' ' << bitset<30>(b_pref) << endl;
    if (swapped) swap(a_pref, b_pref);
    solve_bigger();
  }

public:
  virtual void solve() {
    if (ask(0, 0) == -1) do_swap();
    solve_bigger();
  }
};

struct tester : solver {
  int a, b;
  int step;
  tester(int a_, int b_) : solver(), a(a_), b(b_), step(1) {}
  virtual int ask_(int x, int y) override {
    cout << step << ". Asked: " << bitset<30>(x) << ' ' << bitset<30>(y) << endl;
    ++step;
    x ^= a; y ^= b;
    int r = (x < y) ? -1 : (x > y);
    cout << "Result: " << r << endl;
    return r;
  }

  virtual void answer_(int x, int y) override {
    cout << "Answered: " << bitset<30>(x) << ' ' << bitset<30>(y) << endl;
    if (step > 62) {
      throw string("SLE");  // step limit excceded
    }
    if (a == x and b == y) {
      throw string("AC");
    } else {
      throw string("WA");
    }
  }

  virtual void solve() override {
    cout << "Solving " << bitset<30>(a) << ' ' << bitset<30>(b) << endl;
    solver::solve();
  }
};

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    // srand(time(0));
    // for (int i = 0; i < 1000; ++i) {
      // cout << endl;
      // auto* s = new tester(rand() % (1<<30), rand() % (1<<30));
      // try {
        // s->solve();
      // } catch (string e) {
        // cout << e << endl;
        // if (e == "WA" || e == "SLE") break;
      // }
      // delete s;
    // }
    
    auto s = new solver();
    s->solve();
    delete s;
    return 0;
}

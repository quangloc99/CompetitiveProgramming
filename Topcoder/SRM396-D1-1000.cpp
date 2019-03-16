/**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://www.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm396
 * In short, after making some assumption in the editorial, the part that check if vertor are linear independent is to use
 * Guassian elimination.
 */
#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <fstream>

#define llong long long
#define rep(i,n) for (int i = -1; ++ i < n; )
#define len(a) ((int)a.size())
using namespace std;

int rankOf(vector<llong> mat) {
  int curR = 0;
  for (int i = 0; i < 55 and curR < len(mat); ++i) {
    for (int f = curR; f < len(mat); ++f) {
      if ((~mat[f] >> i) & 1) continue;
      swap(mat[curR], mat[f]);
      break;
    }
    if ((~mat[curR] >> i) & 1) continue;
    for (int f = curR + 1; f < len(mat); ++f) {
      if ((~mat[f] >> i) & 1) continue;
      mat[f] ^= mat[curR];
    }
    ++curR;
  }
  return curR;
}

class MoreNim {
    public:
    long long bestSet(vector<string> heaps) {
        vector<llong> a;
        for (auto i: heaps) {
          stringstream ss; ss << i;
          llong t; ss >> t;
          a.push_back(t);
        }
        sort(a.begin(), a.end());
        vector<llong> optimal; 
        llong ans = 0;
        for (int i = len(a); i--; ) {
          optimal.push_back(a[i]);
          if (rankOf(optimal) != len(optimal)) {
            optimal.pop_back();
            ans += a[i];
            // clog << "take " << a[i] << endl;
          }
        }
        return ans;
    }
};

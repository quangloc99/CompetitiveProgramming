/**
 * Author: Tran Quang Loc (darkkcyan)
 * Using greedy, it is easily to get the final state. So when we have the initial and the final state, we need to find
 * the way to swap them, and graph theory is applied here. This graph is made by the permutations of the final state.
 * The way I do is for each person, swap their present with the one with lowest index in their component, then the result made a new graph.
 * Continue doing with the new one until the all the processes are done.
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

using namespace std;

#define len(x) ((int)x.size())

class YankeeSwap {
    public:
    string sequenceOfSwaps(vector<string> preferences) {
      int n = len(preferences);
      string desired(n, ' ');
      bool has['Z' + 1] = {0};
      for (int i = n; i--; ) {
        for (auto ch: preferences[i]) {
          if (has[(int)ch]) continue;
          desired[i] = ch;
          has[(int)ch] = true;
          break;
        }
      }
      //clog << desired << endl;

      int pos['Z' + 1];
      string cur = "";
      for (char i = 'A'; i <= 'Z'; ++i) {
        pos[(int)i] = i - 'A';
        cur += i;
      }
      cur.resize(n);
      string ans = "";
      for (int i = 0; i < n; ++i) {
        int minpos = i;
        int f = i;
        //clog << i << ": ";
        do  {
          f = pos[(int)desired[f]];
          //clog << f << ' ';
          minpos = min(minpos, f);
        } while (f != i);
        //clog << "=> " << minpos << endl;
        swap(pos[(int)cur[i]], pos[(int)cur[minpos]]);
        swap(cur[i], cur[minpos]);
        if (i == minpos) ans += '-';
        else ans += char(minpos + 'a');
        //clog << cur << endl;
      }
      return ans;
    }
};

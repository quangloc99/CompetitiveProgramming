/**
 * Author: Tran Quang Loc (darkkcyan)
 * This solution is somewhat the same as in the editorial https://www.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm326
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

class BerryPacker {
    public:
    double bestPacking(vector<int> first, vector<int> period, int berries) {
      int min_boxes = (berries + 8) / 9;
      vector<int> cnt_mask(1 << first.size());
      vector<int> cnt(first.size());
      long double ans = 0;

      for (int i = 1; i <= berries; ++i) {
        int cur_mask = 0;
        for (int f = 0; f < (int)first.size(); ++f) {
          if (i > first[f] and (i - first[f] - 1) % period[f] == 0)  {
            cur_mask |= 1 << f;
            cnt[f] ++;
          }
        }
        cnt_mask[cur_mask] ++;
        if (i < min_boxes) continue;

        vector<pair<long double, int>> coef;
        for (int f = (int)cnt_mask.size(); f--; ) {
          if (cnt_mask[f] == 0) continue;
          long double c = 0;
          for (int g = 0; (1 << g) <= f; ++g)
            if ((f >> g) & 1) c += ((long double)i / cnt[g]);
          coef.emplace_back(c, cnt_mask[f]);
        }
        sort(coef.begin(), coef.end());

        int b = berries, n = i;
        vector<int> distribution(10);
        for (int f = 10; f-- > 2; ) {
          distribution[f] = (b - n) / (f - 1);
          n -= distribution[f];
          b -= distribution[f] * f;
        }
        distribution[1] = b;

        auto temp_coef = coef;
        long double cur_ans = 0;
        int u = (int)coef.size() - 1, v = 9;
        while (u >= 0 and v >= 0) {
          int x = min(coef[u].second, distribution[v]);
          cur_ans += x * coef[u].first * v;
          coef[u].second -= x;
          distribution[v] -= x;
          if (coef[u].second == 0) --u;
          if (distribution[v] == 0) --v;
        }

        ans = max(ans, cur_ans);
      }
      return (double)(ans / (long double)first.size());
    }
};

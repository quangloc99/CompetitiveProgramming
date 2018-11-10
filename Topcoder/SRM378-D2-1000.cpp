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

#define len(x) ((int)x.size())

using namespace std;

class HanoiState {
  public:
  string partwayState(string target, int moves) {
    string ans (len(target), 'A');
    for (int i = len(target); i--; ) {
      if (ans[i] == target[i]) continue;
      char otherGoTo = 'A' ^ 'B' ^ 'C' ^ target[i] ^ ans[i];
      int requiredStep = (1 << i);
      if (requiredStep <= moves) {
        moves -= requiredStep;
        ans[i] = target[i];
        for (int f = i; f--; ) ans[f] = otherGoTo;
        continue;
      }
      for (int f = i; f--; ) target[f] = otherGoTo;
    }
    return ans;
  }
};


/**
 * Author: Tran Quang Loc (darkkcyan)
 * I was not able to find the editorial. So here is my solution with meet in the middle.
 * Because the paths have length of 21 characters, so let's call the 11-th character the center (or mid point, whatever).
 * So instead of counting the number of 21-character long path, let's count 11-character long path from a point, and then 
 * combine the results. When counting the path, we store the mask the contains 11 characters along that path. So for each 
 * found mask, we add to the result: cnt[((1 << 21) - 1) ^ mask ^ (1 << c)], where
 * - cnt[m] is the number of way that has character mask equals to m
 * - c is the character at the center.
 * The time complexity is O(n * m * 3 ^ 10 * 4), because at each point beside the center, we only has 3 options, since we cannot go back. 
 * Note that if you find first and combine the result, it would be correct, but if you do like me: combine while counting, 
 * then the result should be multiply by 2 because each path counted 2 times.
 * Also another note: the given alphabet is "discontinous", so I have to preprocess and store it into "alphabet_map".
 */
#include <bits/stdc++.h>

using namespace std;

#define llong long long
int dir_r[] = {-1, 0, 1, 0};
int dir_c[] = {0, -1, 0, 1};
const int all_on = (1 << 21) - 1;
const string alphabet = "ABCDEFZHIKLMNOPQRSTVX";
unordered_map<char, int> alphabet_map;
class AlphabetPaths {
  vector<string> maze;

  vector<int> cnt;
  vector<int> changes;
  int root_char;

  void add(int num) {
    if (cnt[num] == 0) changes.push_back(num);
    ++cnt[num];
  }
  void reset() {
    cnt.resize(1 << 21);
    for (auto i: changes) cnt[i] = 0;
    changes.clear();
  }

  llong dive(int r, int c, int mask) {  // it is not really dfs, so I called it `dive` :p
    if (__builtin_popcount(mask) == 11) {
      add(mask);
      return cnt[all_on ^ mask ^ root_char];
    }
    llong ans = 0;
    for (int i = 4; i--; ) {
      int nr = r + dir_r[i];
      int nc = c + dir_c[i];
      if (nr < 0 || nr >= (int)maze.size()) continue;
      if (nc < 0 || nc >= (int)maze[0].size()) continue;
      if (maze[nr][nc] == '.') continue;
      int cur_char = alphabet_map[maze[nr][nc]];
      if (mask & cur_char) continue;
      ans += dive(nr, nc, mask | cur_char);
    }
    return ans;
  }

  public:
  long long count(vector<string> letterMaze) {
    if (alphabet_map.empty()) {
      for (int i = (int)alphabet.size(); i--; ) {
        alphabet_map[alphabet[i]] = 1 << i;
      }
    }
    maze = move(letterMaze);

    llong ans = 0;
    for (int i = (int)maze.size(); i--; )
      for (int f = (int)maze[0].size(); f--; ) {
        if (maze[i][f] == '.') continue;
        reset();
        root_char = alphabet_map[maze[i][f]];
        ans += dive(i, f, root_char);
      }
    return ans * 2;
  }
};

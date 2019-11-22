/**
 * Author: Tran Quang Loc (darkkcyan)
 * Tested with https://github.com/quangloc99/CompetitiveProgramming/blob/master/SPOJ/NAJPF.cpp
 */
 
template<class container>
vector<int> cal_lsp(const container& s) {
  vector<int> ans(1, 0);
  for (int cur_len = 0; len(ans) < len(s); ) {
    if (s[len(ans)] == s[cur_len]) ans.push_back(++cur_len);
    else if (cur_len) cur_len = ans[cur_len - 1];
    else ans.push_back(0);
  }
  return ans;
}

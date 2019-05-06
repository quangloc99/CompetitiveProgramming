/**
 * Author: Tran Quang Loc (darkkcyan)
 *
 * This code is designed to work with not only string but also vector.
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

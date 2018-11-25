#ifndef llong/*{{{*/
#define llong long long 
#endif
#define rem ((llong)1e9+7)
#define ull pair<unsigned llong, llong>
ull operator + (ull a, const ull& b) {
  a.xx += b.xx;
  (a.yy += b.yy) %= rem;
  return a;
}
ull operator + (ull a, const llong x) {
  a.xx += x;
  (a.yy += x) %= rem;
  return a;
}
ull operator - (ull a, const ull& b) {
  a.xx -= b.xx;
  (a.yy -= b.yy) %= rem;
  if (a.yy < 0) a.yy += rem;
  return a;
}
ull operator* (ull a, const ull& b) {
  a.xx *= b.xx;
  (a.yy *= b.yy) %= rem;
  return a;
}
ull operator * (ull a, const llong x) {
  a.xx *= x;
  (a.yy *= x) %= rem;
  return a;
}
vector<ull> p29d(1, {1, 1});
ull p29(int i) {
  while ((int)p29d.size() <= i)
    p29d.push_back(p29d.back() * 29);
  return p29d[i];
}
struct hs {
  vector<ull> dat;
  hs(const string& s) : dat(1, {0, 0}) {
    for (auto ch: s) {
      dat.push_back(dat.back() * 29 + (llong)(ch - 'a' + 1));
    }
  }
  ull get(int l, int r) {
    return dat[r + 1] - dat[l] * p29(r - l + 1);
  }
};/*}}}*/

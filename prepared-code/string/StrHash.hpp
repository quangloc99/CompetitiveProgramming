// require ModInt.hpp
template<typename U = ModInt<(int)1e9 + 8181>, typename V = unsigned long long>
struct Hash {
    using self_t = Hash<U, V>;
    U x; V y;
    Hash(): x(0), y(0) {}
    Hash(U u, V v): x(u), y(v) {}
    Hash(int u): x(u), y(u) {}
#define component_op(op, uop) def_op(op, uop, { x uop o.x; y uop o.y; })
    component_op(+, +=)
    component_op(-, -=)
    component_op(*, *=)
    def_cmp_op(==, u.x == v.x && u.y == v.y)
    def_cmp_op(!=, u.x != v.x || u.y != v.y)
    def_cmp_op(<, u.x < v.x || (u.x == v.x && u.y < v.y))
    friend ostream& operator<<(ostream& out, const self_t& u) { return out << "(" << u.x << ", " << u.y << ")"; }
};
template<int base = 29, int first_alpha = 'a', typename Hs = Hash<>>
struct StrHash {
    static Hs pow_base(int exp) {
        static vector<Hs> data(1, 1);
        while ((int)data.size() <= exp) data.push_back(data.back() * base);
        return data[exp];
    }
    vector<Hs> hs;
    StrHash() = default;
    StrHash(const string& s): hs(s.size() + 1, {0, 0}) {
        for (size_t i = 0; i < s.size(); ++i)
            hs[i + 1] = hs[i] * base + (s[i] - first_alpha + 1);
    }
    Hs get(int l, int r) const {
        return hs[r] - hs[l] * pow_base(r - l);
    }
    int size() const { return (int)hs.size() - 1; }
    Hs get_pref(int length) const { return get(0, length); }
    Hs get_suff(int length) const { return get(size() - length, size()); }
    int count_occ(const Hs& xhs, int length) const {
        int ans = 0;
        for (int i = 0; i + length <= size(); ++i) {
            ans += xhs == get(i, i + length);
        }
        return ans;
    }
};

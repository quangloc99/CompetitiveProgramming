// vim: foldmethod=marker
  
// Author: Tran Quang Loc (darkkcyan)
// Problem: https://oj.uz/problem/view/IZhO18_segments
// Can also be found and submit here: https://cses.fi/236/list/ and https://codeforces.com/group/Uo1lq8ZyWf/contest/219058
// Ideas:
// We can count the complement instead: for each segment i, either:
// - length of i is less than k
// - i has left endpoint bigger than (the query's right endpoint) - k + 1
// - i has right enpoint less thatn (the query's left endpoint) + k - 1
//
// If we sort the segments by the length, then we can easily use sqrt decomposition to find the answer in
// O(sqrt(n) * log(sqrt(n))).
//
// We do sqrt decomposition on queries and break those queries into sqrt(n) buckets. 
// Suppose we are currently on i-th bucket. For each queries of type 3 we can find the answer 
// for the current bucket in O(sqrt(n)). For the previous buckets, we decompose them like I said above.
///////////////////////////////////////////////////
// This solution barely passed the time limit. Serveral optimization has been done:
// - Only call rebuild() when we are having the number of changes bigger than the bucket's size.
// - We also only call rebuild() when answering the query of type 3.
// - The first and the second query are done in O(1) with the help of STL list.
///////////////////////////////////////////////////
// This code also included a simple test generator with on the fly checker.
// See test_interactor class above the main function.
  
// Sun Feb  9 11:53:54 MSK 2020: START coding
// Sun Feb  9 12:04:53 MSK 2020: PAUSE
// Sun Feb  9 12:10:50 MSK 2020: CONTINUE
// Sun Feb  9 13:05:32 MSK 2020: DONE coding
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

/*{{{*/
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int debug = 1;
#define DB(...) stringstream CONCAT(ss, __LINE__); CONCAT(ss, __LINE__) << __VA_ARGS__; debug_block CONCAT(dbbl, __LINE__)(CONCAT(ss, __LINE__).str())
#else
int debug = 0;
#define DB(...)
#endif
int __db_level = 0;
#define clog if (debug) cerr << string(__db_level * 2, ' ')
struct debug_block {
    string name;
    debug_block(const string& name_): name(name_) { clog << "{ " << name << endl; ++__db_level; }
    ~debug_block() { --__db_level; clog << "} " << name << endl; }
};
#define deb(...) "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]"
#define debln(...)  clog << "[" << #__VA_ARGS__ << "] = [" << __VA_ARGS__ << "]" << endl
#define _ << ", " <<
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return out << "(" << p.first _ p.second << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return out << "(" << get<0>(t) _ get<1>(t) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return out << "(" << get<0>(t) _ get<1>(t) _ get<2>(t) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out _ container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }
/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

#ifdef LOCAL
const int bucket_size = 3;  // for testing
#else
const int bucket_size = 447;
#endif
const int maxn = 200'100;
struct segment {
    llong l, r;

    segment() = default;
    segment(llong l_, llong r_) : l(l_), r(r_) {}
    inline llong length() const { return r - l + 1; }

    friend llong cnt_intersect(const segment& lhs, const segment& rhs) {
        if (lhs.l > rhs.r or rhs.l > lhs.r) return 0;
        return min(lhs.r, rhs.r) - max(lhs.l, rhs.l) + 1;
    }

    bool is_removed() { return l > r; }

    static bool cmp_by_length(const segment& u, const segment& v) { return u.length() < v.length(); }
    static bool cmp_by_left(const segment& u, const segment& v) { return u.l < v.l; }
    static bool cmp_by_right(const segment& u, const segment& v) { return u.r < v.r; }
};

ostream& operator<<(ostream& out, const segment& seg) {
    return out << "segment(" <<
        "l = " << seg.l _
        "r = " << seg.r << 
        ")";
}

template<class T>
function<bool(T*, T*)> cmp_ptr(const function<bool(const T& u, const T& v)>& cmp_ref) {
    return [&](T* u, T* v) -> bool { return cmp_ref(*u, *v); };
}


struct bucket {
    llong min_length, max_length;
    vector<segment> left, right;
    bucket() {}

    template<class iter>
    void assign(iter beg, iter end) {
        left.clear();
        right.clear();
        int dst = (int)distance(beg, end);
        left.reserve(dst);
        right.reserve(dst);
        min_length = min_element(beg, end, segment::cmp_by_length)->length();
        max_length = max_element(beg, end, segment::cmp_by_length)->length();
        for (; beg < end; ++beg) {
            left.push_back(*beg);
            right.push_back(*beg);
        }
    
        sort(left.begin(), left.end(), segment::cmp_by_left);
        sort(right.begin(), right.end(), segment::cmp_by_right);
    }

    int query(segment const& qr, llong k) {
        if (k == 0) return len(left);
        if (max_length < k) return 0;
        if (min_length <= k and k <= max_length and max_length != min_length) {
            int ans = len(left);
            for (auto& seg: left) {
                if (cnt_intersect(seg, qr) < k) -- ans;
            }
            return ans;
        }

        segment upper_left(qr.r - k + 1, qr.r), lower_right(qr.l, qr.l + k - 1);

        return len(left) - (int)(
            (left.end() -  upper_bound(left.begin(), left.end(), upper_left, segment::cmp_by_left)) + 
            (lower_bound(right.begin(), right.end(), lower_right, segment::cmp_by_right) - right.begin())
        );
    }
};

int max_unassigned = 1;
list<segment> all_segments;
list<segment>::iterator seg_id[maxn];

int n_bucket = 0;
bucket buckets[maxn / bucket_size + 10];

vector<segment> current_removed, current_added;

void rebuild() {
    current_removed.clear();
    current_added.clear();

    vector<segment> avaliable_seg(all_segments.begin(), all_segments.end());
    sort(avaliable_seg.begin(), avaliable_seg.end(), segment::cmp_by_length);

    n_bucket = 0;
    for (int i = 0; i < len(avaliable_seg); i += bucket_size)
        buckets[n_bucket++].assign(avaliable_seg.begin() + i, avaliable_seg.begin() + min(i + bucket_size, len(avaliable_seg)));
}

void do_add(const segment& new_seg) { 
    all_segments.push_back(new_seg);
    seg_id[max_unassigned++] = --all_segments.end();
    current_added.push_back(new_seg);
}

void do_remove(int id) {
    current_removed.push_back(*seg_id[id]);
    all_segments.erase(seg_id[id]);
}

int do_query(segment querying_seg, llong k) {
    if (querying_seg.length() < k) return 0;
    int ans = 0;
    rep(i, n_bucket) ans += buckets[i].query(querying_seg, k);

    for (auto& seg: current_added) 
        if (cnt_intersect(seg, querying_seg) >= k) ++ans;
    for (auto& seg: current_removed) 
        if (cnt_intersect(seg, querying_seg) >= k) --ans;
    return ans;
}

struct stdio_interactor {
    int n, t;
    int last_ans = 0;
    stdio_interactor() { scanf("%d %d", &n, &t); }

    inline tuple<int, segment, llong> get_query() {
        int qr_type;
        segment new_seg; 
        llong num;
        scanf("%d", &qr_type);
        if (qr_type == 1 or qr_type == 3) {
            llong l, r; scanf("%lld %lld", &l, &r);
            l ^= t * last_ans;
            r ^= t * last_ans;
            if (l > r) swap(l, r);
            new_seg = segment(l, r);
        }
        if (qr_type == 2 or qr_type == 3) scanf("%lld", &num);
        return {qr_type, new_seg, num};
    }

    inline void answer(int cur_ans) {
        printf("%d\n", cur_ans);
        last_ans = cur_ans;
    }
};

struct test_interactor {
    int n = 3;
    ofstream inp;
    map<int, segment> having_segments;
    set<int> removed;
    int current_ans = -1;

    test_interactor(): inp("test.inp") {
        inp << n << ' ' << 0 << endl;
    }

    ~test_interactor() {
        if (current_ans != -1) {
            clog << "not answering the last query" << endl;
        }
    }

    segment rand_seg() {
        auto l = rand(2'000'000'000L), r = rand(2'000'000'000L);
        if (l > r) swap(l, r);
        return segment(l, r);
    }

    tuple<int, segment, llong> get_query() {
        if (current_ans != -1) {
            clog << "Not answering the previous query" << endl;
            exit(0);
        }

        int qr_type;
        do {
            qr_type = rand(3) + 1;
        } while (qr_type == 2 and !having_segments.size());

        inp << qr_type << ' ';
        segment seg; llong num;
        if (qr_type == 1) {
            seg = rand_seg();
            having_segments[len(having_segments) + 1] = seg;
            inp << seg.l << ' ' << seg.r << endl;
        } else if (qr_type == 2) {
            do {
                num = rand(having_segments.size()) + 2;
            } while (removed.count((int)num));
            removed.insert((int)num);
            inp << num << endl;
        } else {
            seg = rand_seg();
            num = rand(2'000'000'000L);
            current_ans = 0;
            for (auto [id, hseg]: having_segments) {
                if (removed.count(id)) continue;
                current_ans += cnt_intersect(hseg, seg) >= num;
            }
            inp << seg.l << ' ' << seg.r << ' ' << num << endl;
        }
        return {qr_type, seg, num};
    }

    inline void answer(int ans) {
        if (current_ans == -1) {
            clog << "The query does not need to be asnwer" << endl;
            exit(0);
        }
        if (current_ans != ans) {
            clog << "Different answer: " << endl;
            debln(current_ans);
            debln(ans);
            exit(0);
        }
        current_ans = -1;
    }
};

int main(void) {
    // ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0); 
    stdio_interactor interactor;          
    // test_interactor interactor;          

    rep1(nquery, interactor.n) {
        auto [qr_type, new_seg, num] = interactor.get_query();
        if (qr_type == 1) do_add(new_seg);
        else if (qr_type == 2) do_remove((int)num);
        else if (qr_type == 3) {
            if (len(current_added) + len(current_removed) >= bucket_size)
                rebuild();
            interactor.answer(do_query(new_seg, num));
        }
        else assert(false);

    }

    return 0;
}

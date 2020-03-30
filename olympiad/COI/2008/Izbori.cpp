// Author: Tran Quang Loc (darkkcyan)
// Problem: https://wcipeg.com/problem/coi08p1
//
// The indended solution is binary search + dp.
// My solution replace dp part with greedy, which is kinda suprise and also a bit hard to implement imo.
// Idea:
// - The maximum part is ez. We now focus on the minimum part.
// - Let's also forget that we have the threshold 5%.
// - For the pary i, We can do binary search to find the minimum number: 
//   find the "maximum" number of seats can be archive by the party i in the "worse case".
//   (remember, I do greedy instead of dp, so it is a bit different).
// - To check if the number x is ok, first we just simulate (without removing any parties
//   or adding votes. In other words, we do with the initial number of votes), until we 
//   reach the state where party i has x - 1 seats.
// - Here is the observation: adding it now (at the current state), or any other state, is the same
//   as adding them in the beginning. Little proof is at the end of the program. Really recomemend
//   to read it.
// - Now we need to some how distribute the remaining votes to the other party so that 
//   the i-th party can have exactly one more seat. We can find it in greedy maner:
//   for each party beside i, we calculate the minimum number of votes need to to pass
//   the i-th party. Let's say, among the other parties, party j has minimum number of needed votes.
//   Then we add that number of votes to party j as well as 1 seat to j. 
// - The above process continue when we are out of seats or votes. If we are out of votes, then then answer
//   is yes, because we can add at least 1 seat to party i. Otherwise, if we are out of seats, then 
//   we could not add 1 more seat to i-th party.
// - After this my solution also re-simulate the process but with the new number of votes.
// - The final part, the threshold. Notice that we can sort the parties by the initial number of votes.
//   If there are parties that need to be removed, they must be the parties with minimum number of
//   initial votes. So we can sort, then do removal while looping.
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) { return (T)(rng() % range); }

#define CONCAT_(x, y) x##y/*{{{*/
#define CONCAT(x, y) CONCAT_(x, y)
#ifdef LOCAL_DEBUG   
int __db_level = 0;
bool __db_same_line = false;
#define clog cerr << string(!__db_same_line ? __db_level * 2 : 0, ' ')
struct debug_block {
    function<void()> fn;
    void print_name() { __db_same_line = true; fn(); clog << endl; __db_same_line = false;  }
    debug_block(function<void()> fn_): fn(fn_) { clog << "{ "; print_name(); ++__db_level; }
    ~debug_block() { --__db_level; clog << "} "; print_name(); }
};
#define DB(args...) debug_block CONCAT(dbbl, __LINE__)([=]{ clog << args; })
#define deb(...)  if (1) { (clog << "[" #__VA_ARGS__ "] = [" << __VA_ARGS__) << "]"; if (!__db_same_line) clog << endl; }
#else
#define clog if (0) cerr
#define DB(...)
#define deb(...)
#endif
template<class T>
ostream& operator,(ostream& out, const T& thing) { return out << ", " << thing; }
template<class U, class V>
ostream& operator<<(ostream& out, const pair<U, V>& p) { return (out << "(" << p.first, p.second) << ")"; }
template<class A, class B>
ostream& operator<<(ostream& out, const tuple<A, B>& t) { return (out << "(" << get<0>(t), get<1>(t)) << ")"; }
template<class A, class B, class C>
ostream& operator<<(ostream& out, const tuple<A, B, C>& t) { return (out << "(" << get<0>(t), get<1>(t), get<2>(t)) << ")"; }
template<class T> ostream& operator<<(ostream& out, const vector<T>& container) { 
    out << "{";
    if (len(container)) out << container[0];
    rep1(i, len(container) - 1) out, container[i];
    return out << "}";
}
template<class x> vector<typename x::value_type> $v(const x& a) { return vector<typename x::value_type>(a.begin(), a.end()); }
#define ptrtype(x) typename iterator_traits<x>::value_type
template<class u> vector<ptrtype(u)> $v(u a, u b) { return vector<ptrtype(u)>(a, b); }/*}}}*/
// ACTUAL SOLUTION BELOW ////////////////////////////////////////////////////////////

template<class T>  using rw = reference_wrapper<T>;
struct party {
    int id = 0;
    llong voted = 0;
    int allocated_seat = 0;

    party() {}
    party(int i, int v, int as = 0): id(i), voted(v), allocated_seat(as) {}

    friend bool operator<(const party& u, const party& v) {
        llong diff = 1ll * u.voted * (1 + v.allocated_seat) - 1ll * v.voted * (1 + u.allocated_seat);
        if (diff == 0) return u.id > v.id;
        return diff < 0;
    }

    llong need_to_catch(const party& other) const {
        llong ans = ((1 + allocated_seat) * other.voted + other.allocated_seat) / (1 + other.allocated_seat);
        if (ans * (1 + other.allocated_seat) == other.voted * (1 + allocated_seat) and id > other.id) 
            ++ans;
        return max(0ll, ans - voted); 
    }

    double get_coef() const { // just for debugging
        return (double) voted / (1 + allocated_seat);
    }
};

ostream& operator<<(ostream& out, const party& u) {
    return (out << "(" << u.id, u.voted, u.allocated_seat, u.get_coef()) << ")";
}

struct cmp_by_need {
    rw<party> to;
    cmp_by_need(rw<party> to_) : to(to_) {}
    bool operator()(const party& u, const party& v) const {
        llong diff = u.need_to_catch(to) - v.need_to_catch(to);
        if (diff == 0) return u.id < v.id;
        return diff > 0;
    }
};

llong v, remain, required;
int n, m;
vector<party> parties;

llong solve(party cur_party, llong cur_remain, const vector<party>& filtered_parties) {
    deb(filtered_parties);
    auto ok = [&](llong estimated) -> bool {
        auto all_p = filtered_parties;
        auto cur_p = cur_party;

        priority_queue<rw<party>> prio(all_p.begin(), all_p.end());
        prio.push(cur_p);
        int nseat = m;
        for (; len(prio) and nseat > 1 and cur_p.allocated_seat < estimated - 1; nseat--) {
            auto& t = prio.top().get();
            prio.pop();
            t.allocated_seat ++;
            prio.push(t);
        }

        if (cur_p.allocated_seat + 1 != estimated) return false;

        priority_queue<
            rw<party>, 
            vector<rw<party>>,
            cmp_by_need
        > prio2(all_p.begin(), all_p.end(), cmp_by_need(cur_p));

        llong cur_r = cur_remain;
        for (; len(prio2) and nseat > 1; --nseat) {
            auto& top = prio2.top().get();
            prio2.pop();
            llong need = top.need_to_catch(cur_p);
            if (need > cur_r) break;
            cur_r -= need;
            top.voted += need;
            top.allocated_seat ++;
            prio2.push(top);
        }
        llong last_need = prio2.top().get().need_to_catch(cur_p);
        if (last_need <= cur_r) return false;
        cur_p.allocated_seat = 0;
        for (auto& u: all_p) u.allocated_seat = 0;

        prio = priority_queue<rw<party>>(all_p.begin(), all_p.end());
        prio.push(cur_p);
        nseat = m;
        for (; len(prio) and nseat > 0; nseat--) {
            auto& t = prio.top().get();
            prio.pop();
            t.allocated_seat ++;
            prio.push(t);
        }
        return cur_p.allocated_seat >= estimated;
    };

    llong l = 0, r = m;
    while (l < r) {
        llong mid = (l + r + 1) / 2;
        if (ok(mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> v >> n >> m;
    required = (v + 19) / 20;

    remain = v;

    parties.resize(n);
    rep(i, n) {
        parties[i].id = i;
        cin >> parties[i].voted;
        remain -= parties[i].voted;
    }

    rep(i, n) {
        auto p = parties;
        p[i].voted += remain;
        priority_queue<rw<party>> prio;
        for (auto& party: p) {
            if (party.voted < required) continue;
            prio.push(party);
        }
        for (int nseat = m; len(prio) and nseat--; ) {
            party& top = prio.top();
            prio.pop();
            top.allocated_seat ++;
            prio.push(top);
        }
        cout << p[i].allocated_seat << ' ';
    }
    cout << '\n';

    rep(i, n) {
        auto current = parties[i];
        if (current.voted < required) {
            cout << "0 ";
            continue;
        }
        vector<party> safe_parties, in_danger_parties;
        rep(f, n) {
            if (f == i) continue;
            if (parties[f].voted < required) in_danger_parties.push_back(parties[f]);
            else safe_parties.push_back(parties[f]);
        }

        sort(in_danger_parties.begin(), in_danger_parties.end(), [](const party& x, const party& y) {
                if (x.voted == y.voted) return x.id > y.id;
                return x.voted < y.voted;
        });

        llong ans = LLONG_MAX;
        for (llong cur_remain = remain; cur_remain >= 0; ) {
            ans = min(ans, solve(current, cur_remain, safe_parties));
            if (!len(in_danger_parties)) break;
            cur_remain -= required - in_danger_parties.back().voted;
            in_danger_parties.back().voted = required;
            safe_parties.push_back(in_danger_parties.back());
            in_danger_parties.pop_back();
        }
        cout << ans << ' ';
    }


    return 0;
}

// Here is a little intuition why greedy works.
//
// More formally, Suppose that we already added the votes to some party, except for 1 party i.
// Let x be the number of seats the group i will have.
//
// I want to prove these 2 following process are the same:
// - First, we add the votes. After adding the votes, we need to distrubutes k seats.
// - Initially, we don't add the votes. We just do seat distribution right away. 
//   After distribute some seats, and the party i now have y < x seats, then we can still
//   add the votes, and continue process until we have k seats.
//
// These 2 processes will procduce the same set of seats each parties need.
//
// Well we can rephrase the process like following: for each party j with 
// the number of votes v[i], we draw points v[j] / 1, v[j] / 2, v[j] / 3, ... on the Ox line.
// - For the first process, we need to increase some of v[j], and then pick the first k points.
// - For the second process, we need to pick some first points untils the i-th party has y points.
//   After that we increase v[j] and pick need points that has not been picked until we reach k points.
// If the condition y < x, holds, then intuitively, you can see that either moving first or after,
// or whenever you want, will produre the same sets of the first k points needs to be pick.
//
// vim: foldmethod=marker

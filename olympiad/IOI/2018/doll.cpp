/**
 * Author: Tran Quang Loc
 * Problem: https://oj.uz/problem/view/IOI18_doll
 * Idea:
 * How do we make a counter for 2^n number with this kind of circuit?
 * Well, just make a complete binary tree with 2^n leaf and each nodes is a switch.
 * The order we meet each leaf is the number, represented in binary by its path from 
 * it to the root. (X is 0 and Y is 1)
 * So it is really easy to see that we can just build a complete binary tree for 2^n numbers.
 * But how about the other case. I suggest the following way to build:
 * - We make n places for each number, then we fill the number later.
 * - We group pairs of places into 1 switch. If there is 1 place left, then we also
 *   make connect it from 1 switch, with the other output go to the root.
 * - Repeat the process for the group until we got only 1 group left.
 * - Fill the number into the places by simulation.
 * Note: we should left the single place/group in the first place, in order to ensure that 
 * the switch go to this group/place will have state X at the end.
 *
 * BIG NOTE: also I lied, for the case 2^n, we actually need 1 more output to the origin. 
 * So I modify the array a little bit: first, connect origin to the first number. Delete the first number
 * from the list, then append the origin at the end of the list. This also works for the other cases.
 * 
 * Also, the case where N = 1 should be handle separately.
 */

#include<bits/stdc++.h>
#include "doll.h"

using namespace std;
using namespace std::placeholders;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()
// #define rand __rand
// mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
// template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    // return (T)(rng() % range);
// }

void create_circuit(int m, std::vector<int> a) {
    int n = len(a);
    vector<int> x, y;
    vector<int> groups(n, 0);
    int fake_root = INT_MIN;
    while (len(groups) > 1) {
        vector<int> new_groups;
        if (len(groups) & 1) {
            new_groups.push_back(~len(x));
            x.push_back(fake_root);
            y.push_back(groups[0]);
        }
        for (int i = len(groups) & 1; i + 1 < len(groups); i += 2) {
            new_groups.push_back(~len(x));
            x.push_back(groups[i]);
            y.push_back(groups[i + 1]);
        }
        swap(groups, new_groups);
    }
    
    int root = ~(len(x) - 1);
    replace(x.begin(), x.end(), fake_root, root);

    std::vector<int> c(m + 1, root);
    c[0] = a[0];
    if (n > 1) { 
        a.erase(a.begin(), a.begin() + 1);
        a.push_back(0);
        vector<int> states(len(x));
        rep(i, n) {
            int cur = ~(len(x) - 1), prev = cur;
            while (cur < 0) {
                prev = cur;
                cur = states[~cur] ? y[~cur] : x[~cur];
                states[~prev] ^= 1;
            }
            (states[~prev] ? x[~prev] : y[~prev]) = a[i];
        }
    } else {
        c[a[0]] = 0;
    }
    answer(c, x, y);
}

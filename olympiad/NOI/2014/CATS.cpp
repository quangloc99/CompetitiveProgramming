/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/NOI14_cats
 *
 * Of course this problem is solvable with pattern finding. I find my solution with both observation and
 * simulation. So here is the idea:
 *
 * - When you do simulation, the answer must be 2*k*N or 2*k*N + 1 where k is the smallest number such that
 *   2 * k * N > L. I see this observation with simulation on paper.
 *
 * - 2*k*N and 2*k*N + 1 seems to alternate in some period. The period is a bit hard to be seen. When using simulation,
 *   the period change when L is equals to 2*i*N + 1. In fact, the period is double by 2, and the new part is the 
 *   period is the "compliment" of the old period. Example for the first few period (I just write the parities, and 
 *   the spaces between the numbers just for the eyes).
 *     0110
 *     0110 1001
 *     0110 1001 1001 0110
 *     ...
 * 
 * - From these 2 facts above I think this problem is solved. But here is a little intuition why the periods are like so.
 *   When doing simulation, the first number we got will always be even, and suppose we are having k elements in S1. 
 *   We can see that a period is done when we consumed all k elements in S1. So when k is increased, that means the 
 *   period has to be changed. Doing more simulations, you can also see that why the period is the power of 2.
 *
 */
#include<bits/stdc++.h>
using namespace std;

// not needed in actual solution.
void simulate(long long x, long long l, long long n) {
    vector<long long> s1, s2;
    bool s1_inv = 0;

    while (x) {
        if (!s1.size()) s1.push_back(0);
        if (!s2.size()) s2.push_back(0);
        s2.push_back(s1.back() ^ s1_inv);
        s1.pop_back();
        s1_inv ^= 1;
        if (s2.back() > l) {
            cout << (s2.back() & 1);
            --x;
        } else {
            s2.back() += 2ll * n;
            s1.push_back(s2.back() ^ s1_inv);
            s1.push_back(s2.back() ^ s1_inv);
            s2.pop_back();
            if (s2.size()) s2.pop_back();
        }
    }
}

const long long l2_inf = 62;
int main() {
    int ntest; cin >> ntest;  
    while (ntest--) {  
        long long x, l, n; cin >> x >> l >> n;  
        long long mul = l / n;  
        mul += 1 + (~mul & 1);  

        long long l2_period = min(l2_inf, mul / 2 + 1);
        --x;  
        x %= 1ll << l2_period;  

        cout << mul * n + (__builtin_popcountll(x) & 1) << '\n';  
    }  
    // cout << endl;  
    // simulate(1000, 9, 2);   
    return 0;
}

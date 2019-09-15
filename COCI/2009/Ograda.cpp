/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://wcipeg.com/problem/coci094p4
 * Idea: it is really easy to calculate the first number (of the output). 
 * For each number, we can see that there are about x ranges that cover this number.
 * For each range we calculate the minimum high of the planks in that ranges.
 * And the highest cell we can paint the current plank is the maximum of those minimums we 
 * just computed. This can be done by using deque 2 times.
 * If the numbers are different, calculate the second number is also easy. 
 * For the case where there are same numbers, first we can just paint from the first plank.
 * Then when we see when there is a plank that has different highest *paintable* cell, 
 * then we need to chose different starting point. We can just do it greedily, that is, choose
 * the furthest start point that when we point from that point, we can still archive the 
 * same highest *paintable* cell. Sorry for complicated English. The code is not that long so
 * you can just also read the code :))).
 */
#include <bits/stdc++.h>
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

#define maxn 1010101
int n, x;
int a[maxn];
int min_ranges[maxn];
unordered_set<int> swoop_points;

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> x;
    rep(i, n) cin >> a[i];

    deque<int> qu;
    rep(i, n - x + 1) {
        while (len(qu) and qu.front() < i) qu.pop_front();
        while (!len(qu) or qu.back() != i + x - 1) {
            int nxt = !len(qu) ? i : qu.back() + 1;
            while (len(qu) and a[qu.back()] > a[nxt]) qu.pop_back();
            qu.push_back(nxt);
        }
        min_ranges[i] = qu.front();
    }

    llong min_paint_needed = 0;
    qu.clear();
    rep(i, n) {
        while (len(qu) and qu.front() < i - x + 1) qu.pop_front();
        if (i <= n - x) {
            while (len(qu) and a[min_ranges[qu.back()]] < a[min_ranges[i]]) qu.pop_back();
            qu.push_back(i);
        }
        if (!swoop_points.count(qu.front())) {
            while (len(qu) > 1 and a[min_ranges[qu[0]]] == a[min_ranges[qu[1]]])
                qu.pop_front();
            swoop_points.insert(qu.front());
        }
        min_paint_needed += a[i] - a[min_ranges[qu.front()]];
    }

    cout << min_paint_needed << '\n' << swoop_points.size();
    
    return 0;
}

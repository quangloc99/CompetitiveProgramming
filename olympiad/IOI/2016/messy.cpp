/**
 * Author: Tran Quang Loc (darkkcyan)
 * Link to problem + online submitting: https://oj.uz/problem/view/IOI16_messy
 * Official solution: https://ioinformatics.org/files/ioi2016solutions.pdf
 * My idea is really similar to the official, but I kinda do it in reverse way.
 * In the official solution, they was considering the bit from the most to less significant bit. 
 * (the first step, they divided the number into 2 groups, the "0" group has most sigificant bit = 0, and the reverse for "1" group)
 * In my solution, I do it from less to most significant bit.
 */
#include <bits/stdc++.h>

#include "messy.h"

using namespace std;
typedef bitset<128> BS;

string reverse_string(string s) {
    reverse(s.begin(), s.end());
    return s;
}

void add_element(BS const& bs, int n) {
    add_element(reverse_string(bs.to_string()).substr(0, n));
}

bool check_element(BS const& bs, int n) {
    return check_element(reverse_string(bs.to_string()).substr(0, n));
}

int log2(int n) {
    if (n == 1) return 0;
    return 1 + log2(n / 2);
}

const BS one = 1;
void add_elements(int n) {
    BS prev;
    int n_bit = log2(n);
    
    for (int i = 1; i < n; i += 2) {
        prev[i] = 1;
        add_element(one << i, n);
    }

    for (int cur_bit = 1; cur_bit < n_bit; ++cur_bit) {
        BS cur;
        for (int i = 0; i < n; ++i) {
            if ((~i >> cur_bit) & 1) continue;
            cur[i] = 1;
            add_element((prev[i] ? ~prev : prev) | (one << i), n);
        }
        prev = move(cur);
    }
}

vector<int> do_restore(int n) {
    vector<int> ans(n);

    BS prev;
    for (int i = 0; i < n; ++i) {
        if (!check_element(one << i, n)) continue;
        prev[i] = 1;
        ans[i] = 1;
    }

    int n_bit = log2(n);
    for (int cur_bit = 1; cur_bit < n_bit; ++cur_bit) {
        BS cur;
        for (int i = 0; i < n; ++i) {
            if (!check_element((prev[i] ? ~prev : prev) | (one << i), n)) continue;
            cur[i] = 1;
            ans[i] |= 1 << cur_bit;
        }
        prev = move(cur);
    }
    return ans;
}

std::vector<int> restore_permutation(int n, int w, int r) {
    add_elements(n);
    compile_set();
    return do_restore(n);
}

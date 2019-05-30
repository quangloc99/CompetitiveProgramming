/**
 * Author: Tran Quang Loc (darkkcyan)
 * Offical solution: https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/COCI/official/2008/olympiad_solutions/solutions.pdf
 * Idea: my solution is slightly difference than the offical solution, and actually a bit complicated, since I could not come up with
 * the first observation in the offical solution.
 * 
 * Let's call f(x) be the number of number with self-product is less or equals than x. So the result is f(b) - f(a - 1).
 * To calculate f(x), for each product of 2, 3, 5, 7, let's call it `s`, we need to count the number that has the digit-product 
 * equals to `s`. In order to do that, I used digit dp. Instead of the dp state like this:
 * dp[position][lower][count2][count3][count5][count7]
 * I convert count2, count3, count5, count7 into a number, then the dp state now has only 3 componenets, and also I stored the count*
 * variables into an array, called `state`. My `cal_key` function is used to convert the number, and `count_le` function does the dp part.
 * Also in order to calculate and checking the dp state faster (well, shorter code, not really faster), I used valarray instead of vector.
 *
 * And also checkout the code with the test generator with bruteforce solution in the same folder.
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
#define rand __rand
mt19937_64 rand(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64

#define div div___
int div[] = {2, 3, 5, 7};
valarray<int> div_list[10];
void gen_div() {
    rep1(i, 9) {
        int t = i;
        div_list[i] = valarray<int>(4);
        rep(f, 4) {
            while (t % div[f] == 0) {
                t /= div[f];
                div_list[i][f]++;
            }
        }
    }
}

#define max_digit 20
#define max2 32
#define max3 32
#define max5 12
#define max7 11
#define max_state (max2 * max3 * max5 * max7)
int cal_key(valarray<int> const& state) {
    return ((state[0] * max3 + state[1]) * max5 + state[2]) * max7 + state[3];
}

unsigned llong dp[max_digit][2][max_state];
bool lower_calculated[max_digit][max_state];

unsigned llong count_le(const string& s, int pos, bool lower, valarray<int> const& state) {
    if (state[state < 0].size()) return 0;
    if (pos == len(s)) return state[state == 0].size() == 4;

    int key = cal_key(state);
    auto& ans = dp[pos][lower][key];
    if (lower and lower_calculated[pos][key]) return ans;

    ans = 0;
    int d = lower ? 9 : (s[pos] - '0');
    rep1(i, d) {
        ans += count_le(s, pos + 1, i < d or lower, state - div_list[i]);
    }
    if (lower) lower_calculated[pos][key] = 1;
    return ans;
}

string to_str(unsigned llong num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

unsigned llong count_le(unsigned llong a) {
    if (a <= 0) return 0;
    llong ans = 0;
    int n2, n3, n5, n7;
    unsigned llong s2, s3, s5, s7;
    for (n2 = 0, s2 =  1; s2 <= a and n2 < max2; s2 *= 2, ++n2) 
    for (n3 = 0, s3 = s2; s3 <= a and n3 < max3; s3 *= 3, ++n3) 
    for (n5 = 0, s5 = s3; s5 <= a and n5 < max5; s5 *= 5, ++n5) 
    for (n7 = 0, s7 = s5; s7 <= a and n7 < max7; s7 *= 7, ++n7) {
        auto s = to_str(a / s7);
        int pad0 = max_digit - len(s);
        s = string(pad0, '0') + s;

        llong loc_ans = 0;
        for (int i = pad0 + 1; i < len(s); ++i)
            loc_ans += count_le(s, i, true, {n2, n3, n5, n7});
        loc_ans += count_le(s, pad0, false, {n2, n3, n5, n7});
        ans += loc_ans;
    }
    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    gen_div();
    memset(lower_calculated, 0, sizeof lower_calculated);
    unsigned llong a, b;
    cin >> a >> b;
    cout << count_le(b) - count_le(a - 1);

    return 0;
}

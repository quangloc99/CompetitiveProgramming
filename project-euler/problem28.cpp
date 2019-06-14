/**
 * Author: Tran Quang Loc (darkkcyan)
 * Solution is accepted on HackerRank as well as producing the correct answer for the original problem on Project Euler.
 * The answer for the original problem is written to the stderr.
 * Idea:
 *   For each layer, we can see that the top right corner is a perfect square number of an odd number (1, 9, 25, ...)
 *   So for the k-th layer, the sum of the 4 conners is:
 *      (2k - 1)^2 + (2k - 1)^2 - (2k - 2) + (2k - 1)^2 - (2k-2) * 2 + (2k - 1)^2 - (2k-2) *3
 *      = (2k-1)^2 * 4 - (2k - 1) * 6
 *      = 16 * k^2 - 28*k + 16
 * Here, I break the sum into 3 parts:
 * x = 16 * (1^2 + 2^2 + 2^3 + ... + k ^ 2) = 16 * k * (k + 1) * (2k + 1) / 6
 * y = 28 * (1 + 2 + 3 + ... + k)
 * z = 16 * k
 * And the final answer is x - y + z
 * But wait, there is more:
 * The very first layer is not correct for the above formua. Using above formula, we got 4, but the right answer is 1
 * So final `correct` result is x - y + z - 3.
 * This approach is O(1) per testcase.
 */
#include<bits/stdc++.h>
using namespace std;

#define llong long long 
#define rem ((llong) 1e9+7)

constexpr llong mpow(llong b, llong ex) {
    llong ans = 1;
    for (; ex > 0; ex >>= 1) {
        if (ex & 1) (ans *= b) %= rem;
        (b *= b) %= rem;
    }
    return ans;
}

const llong inv6 = mpow(6, rem - 2);
const llong inv2 = mpow(2, rem - 2);

llong sum_square(llong n) {
    n %= rem;
    return n * (n + 1) % rem * (2 * n + 1) % rem * inv6 % rem;
}

llong sum_linear(llong n) {
    n %= rem;
    return n * (n + 1) % rem * inv2 % rem;
}

llong solve(llong n) {
    assert(n % 2 == 1);
    llong k = n / 2 + 1;
    llong x = 16 * sum_square(k) % rem;
    llong y = 28 * sum_linear(k) % rem;
    llong z = 16 * k % rem;
    return (x + (rem - y) + z + rem - 3) % rem;
}


int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int ntest; cin >> ntest;
    while (ntest--) {
        llong n; cin >> n;
        cout << solve(n) << '\n';
    }

    clog << solve(1001);

    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * This solution is designed to get accepted on HackerRank as well as produce the correct answer for the orignal problem.
 * To use for the original problem, first enter 1000 to STDIN, as it is also the input format on HackerRank
 * The output for the orignal problem is the first number Written to STDERR. The second number is the largest number of prime
 * that can be produce by the formula.
 *
 * Idea: with n = 0, b must be a prime.
 *       with n = 1, 1 + a + b must also be a prime
 * Therefore, we can first find `b` by a loop through prime numbers, then also find `a` with another loop through the prime numbers.
 * We can also notice that `n` (not big `N`) cannot exceed `b`, see the explaination in the statement for the reason.
 * So the complexity is about O(K^3), where K is the number of prime less than N (the input).
 * This complexity is acceptable, since the number of prime that less than 2000 is about 280.
 */
#include <bits/stdc++.h>
using namespace std;

int n;
int n_primes = 1000000;  // large, because we also need to check whether the number produced by the formula is a prime or not.
vector<int> primes;
vector<bool> is_prime;
void prime_sieve() {    
    is_prime.resize(n_primes + 1, 1);
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i <= n_primes; ++i) {
        if (!is_prime[i]) continue;
        primes.push_back(i);
        for (long long f = 1ll * i * i; f <= n_primes; f += i) 
            is_prime[f] = 0;
    }
}

int count_valid(int a, int b) {
    int cnt = 0;
    do {
        long long  t = 1ll * cnt * cnt + 1ll * cnt * a + b;
        if (t < 0) break;
        if (!is_prime[t]) break;
        ++cnt;
    } while (true);
    return cnt;
} 


int main() {
    cin >> n;
    prime_sieve();
    tuple<int, int, int> ans(-1, -1, -1);
    for (auto b: primes) {
        if (b > n) continue;
        for (auto semi_a : primes) {
            int a = semi_a - b - 1;
            if (a < -n or a > n) continue;
            ans = max(ans, {count_valid(a, b), a, b});
        }
    }

    cout << get<1>(ans) << ' ' << get<2>(ans);
    clog << 1ll * get<1>(ans) * get<2>(ans) << ' ' << get<0>(ans);
    return 0;
}

/**
 * Author: Tran Quang Loc (darkkcyan)
 * Some note:
 * - I first find the maximum sum of divisions of i, for i less than 10000, since the number might exceed 10000.
 *   But it turns out that this step is not really necessary.
 * - Remember that d(a) = b and d(b) = a, but a and b must be *different* (a != b).
 */
#include <bits/stdc++.h>
using namespace std;

vector<int> get_sum_divs(int n) {
	vector<int> sum_divs(n + 1);
	for (int i = 1; i <= n; ++i) {
		for (int f = i * 2; f <= n; f += i) {
			sum_divs[f] += i;
		}
	}
	return move(sum_divs);
}

int main() {
	int n = 10000;
	
	auto t_sum_divs = get_sum_divs(n);
	int x = *max_element(t_sum_divs.begin(), t_sum_divs.end());
	auto true_sum_divs = get_sum_divs(x + 1);
	long long ans = 0;
	for (int i = 1; i <= n; ++i) {
		if (true_sum_divs[true_sum_divs[i]] == i and true_sum_divs[i] != i) {
			ans += i;
		}
	}
	cout << ans;
}

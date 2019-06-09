/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem link: https://projecteuler.net/problem=20
 * This solution used no external library.
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
	int n = 100;
	
	vector<int> prod(1, 1);
	for (int i = 1; i <= n; ++i) {
		int carry = 0;
		for (int f = 0; f < prod.size() or carry; ++f) {
			if (prod.size() == f) prod.push_back(0);
			carry += i * prod[f];
			prod[f] = carry % 10;
			carry /= 10;
		}
	}
	
	int ans = 0;
	for (auto i: prod) ans += i;
	cout << ans;
	return 0;
}

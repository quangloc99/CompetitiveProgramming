#include <bits/stdc++.h>
using namespace std;
 
int main() {
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
 
	int n;
	#define ullong unsigned long long
	#define rem ((ullong)1e9+8181)      // I attemped to use custom hash function, but then I realized that I can use standard lib.
 
	auto hs = hash<string>();
	while (cin >> n) {
		ullong t = 0;
		while (n--) {
			string s; cin >> s;
			t ^= hs(s);
		}
		cout << (!t ? "second" : "first") << " player\n";
	}
 
 
	return 0;
}

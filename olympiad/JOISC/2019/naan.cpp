/**
 * Author: Tran Quang Loc (darkkcyan)
 * Problem: https://oj.uz/problem/view/JOI19_naan
 * Code solution + test data: https://www.ioi-jp.org/camp/2019/2019-sp-tasks/index.html
 * Well this problem is surely not easy as it seem, and sadly there was not English
 * solution. This solution below is from the code solution and the commentary slide with google translate.
 * The idea is actually surprisingly simple:
 * - First, we need to pick the first person. This person must have minimum length of nan among
 *   the other people. After that we remove this person and the part of the nan that he pick
 *   and recursively solve the problem with the rest of people and the rest of nan.
 * That is, that the whole solution. Because of not knowing Japanese, I don't know how to proof
 * this. But the solution below has different implementation with another proof that is a lots
 * simpler and easier for both understanding and implementation. 
 * The solution above also have a big problem though, and it is mentioned in the slide. That is,
 * rational number calculation, so that solution can not obtain 100% score.
 *
 * So here is the **BETTER** solution: 
 * - Let's call sum[i] = sum of all v[i]
 * - For each person i, we calculate array split[i][], such that if we use this array
 *   for splitting the nan, we can get n part that equals to sum[i] / n;
 * - So we do as above with a little modification. We first pick the first person i
 *   having the smallest split[i][1]. Remove this person. Then we pick the second person
 *   i having the smallest split[i][2]. Then remove this person. In other word, for finding
 *   the f-th person, we need to find the person i that is not removed, and having the smallest
 *   split[i][f].
 * Why this works? I don't really have a formal proof, and also because of the Japanese. But
 * I think the reason is actually simple. If we pick such person that has smallest[i][1], 
 * then the other still have split[j][2] behind split[i][1] (because split[j][1] is behind
 * split[i][1], and split[j][2] is behinds split[j][1]). So for the same reasons, if we 
 * already picked (f - 1) person, and can always pick the f-th person, since all of the split[u][f]
 * (where u is not picked person), are behinds split[v][1..f - 1] (v is picked person).
 *
 * This proof is also prove that there is always a solution.
 *
 * Note: my code is 0-based index.
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

struct frac_with_num {
    llong int_part, numerator, denominator;
    frac_with_num(llong num = 0): int_part(num), numerator(0), denominator(1) {}
    frac_with_num(llong whole, llong num, llong den): int_part(whole), numerator(num), denominator(den) {
        norm();
    }

    frac_with_num(llong num, llong den): frac_with_num(0, num, den) {}

    frac_with_num& norm() {
        if (numerator > denominator) {
            int_part += numerator / denominator;
            numerator %= denominator;
        }
        llong g = __gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        return *this;
    }
};

bool operator < (const frac_with_num& u, const frac_with_num& v) {
    if (u.int_part == v.int_part) return u.numerator * v.denominator < u.denominator * v.numerator;
    return u.int_part < v.int_part;
}

ostream& operator<<(ostream& cout, const frac_with_num& u) {
    return cout << u.int_part * u.denominator + u.numerator << ' ' << u.denominator;
}


#define maxn 2020
int n, l;
llong v[maxn][maxn];
llong sum[maxn];
vector<frac_with_num> split[maxn];

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> n >> l;
    rep(i, n) {
        sum[i] = 0;
        rep(f, l) {
            cin >> v[i][f];
            sum[i] += v[i][f];
        }
    }

    rep(i, n) {
        llong cur_sum_nan = 0;
        int cur_nan = 0;
        rep1(num_cur_part, n - 1) {
            for (; (cur_sum_nan + v[i][cur_nan]) * n <= sum[i] * num_cur_part; ++cur_nan) 
                cur_sum_nan += v[i][cur_nan];
            llong rest = sum[i] * num_cur_part - cur_sum_nan * n;
            split[i].emplace_back(cur_nan, rest, n * v[i][cur_nan]);
            // clog << cur_nan << ' ' << rest << ' ' << cur_sum_nan << endl;
        }
        split[i].emplace_back(l);

        // clog << i << ": ";
        // for (auto f: split[i]) clog << f << "; ";
        // clog << endl;
    }

    vector<int> ans;
    set<int> has;
    rep(i, n) has.insert(i);

    rep(cur_part, n) {
        auto cur = *min_element(has.begin(), has.end(), [&](int lhs, int rhs) {return split[lhs][cur_part] < split[rhs][cur_part]; } );
        ans.emplace_back(cur);
        has.erase(cur);
        if (cur_part != n - 1) cout << split[cur][cur_part] << '\n';
    }

    
    for (auto i: ans) cout << i + 1 << ' ';


    return 0;
}

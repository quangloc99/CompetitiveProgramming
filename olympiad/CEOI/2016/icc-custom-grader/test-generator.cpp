/**
 * Author: Tran Quang Loc (darkkcyan)
 */
const int DEFAULT_M = 1625;

#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;

#define rand __rand
mt19937 rng(chrono::system_clock::now().time_since_epoch().count());  // or mt19937_64
template<class T = int> T rand(T range = numeric_limits<T>::max()) {
    return (T)(rng() % range);
}

bool all_digit(const string& s) { return all_of(s.begin(), s.end(), ::isdigit); }

template<class T>
T to_num(const string& s) {
    stringstream ss(s);
    T res; ss >> res;
    return res;
}

void print_usage_then_exit() {
    cerr << (
        "Usage:\n"
        "    test-generator N [M]\n"
        "\n"
        "Arguments:\n"
        "    N: the number of cities.  Required.\n"
        "    M: the number of queries. Default: 1625.\n"
    );
    exit(0);
}

int main(int n_arg, char** args) {
    if (n_arg == 1) print_usage_then_exit();
    if (!all_digit(args[1])) print_usage_then_exit();
    if (n_arg > 2 and !all_digit(args[2])) print_usage_then_exit();

    int n = to_num<int>(args[1]);
    int m = n_arg == 2 ? DEFAULT_M : to_num<int>(args[2]);

    cout << n << ' ' << m << endl;
    vector<int> nodes(n);
    for (int i = 0; i < n; ++i) {
        nodes[i] = i + 1;
    }

    random_shuffle(nodes.begin(), nodes.end(), rand<int>);
    
    vector<pair<int, int>> edge_list;
    for (int cur_edge = 1; cur_edge < n; ++cur_edge) {
        int u = nodes[cur_edge];
        int v = nodes[rand(cur_edge)];
        if (rand(2)) swap(u, v);
        edge_list.emplace_back(u, v);
    }

    random_shuffle(edge_list.begin(), edge_list.end(), rand<int>);
    for (auto i: edge_list) {
        cout << i.first << ' ' << i.second << endl;
    }

    return 0;
}

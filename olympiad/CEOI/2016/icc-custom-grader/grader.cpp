/**
 * Author: Tran Quang Loc (darkkcyan)
 */
#include<bits/stdc++.h>
#include "icc.h"
using namespace std;

template<class T>
pair<T, T> ordered(pair<T, T> a) {
    if (a.first > a.second) swap(a.first, a.second);
    return a;
}

class Dsu {
private:
    int n;
    vector<int> set_head;
public:
    Dsu(int n_) : n(n_), set_head(n) {
        for (int i = n; i--; ) {
            set_head[i] = i;
        }
    }

    int find_set(int u) { return u == set_head[u] ? u : set_head[u] = find_set(set_head[u]); }
    bool same_set(int u, int v) {
        return find_set(u) == find_set(v);
    }

    void join(int u, int v) {
        if (rand() & 1) swap(u, v);
        set_head[find_set(u)] = find_set(v);
    }
};

class Grader {
private:
    int n, m;
    int current_edge_id;
    vector<pair<int, int>> edge_list;
    vector<vector<bool>> adjacency_matrix;

    char err_buf[100];

public:
    template<class InputIterator>
    Grader(int n_, int m_, InputIterator edge_list_begin, InputIterator edge_list_end)
        : n(n_) , m(m_)
        , current_edge_id(0)
        , edge_list(edge_list_begin, edge_list_end)
    {
        check_input();

        adjacency_matrix.resize(n, vector<bool>(n));
        set_edge(edge_list[0]);
    }
private:
    void set_edge(pair<int, int> const& u) {
        adjacency_matrix[u.first - 1][u.second - 1] = 1;
        adjacency_matrix[u.second - 1][u.first - 1] = 1;
    }

public:
    void check_input() {
        if (n < 1) throw "N must be a positive integer";
        if (m < 1) throw "M must be a positive integer";
        if ((int)edge_list.size() != n - 1) throw "There must be exactly N - 1 edges";
        Dsu dsu(n);
        for (auto i: edge_list) {
            if (i.first < 1 or i.first > n or i.second < 1 or i.second > n) {
                throw "Nodes must be positive integers and not greater than N";
            }

            if (dsu.same_set(i.first - 1, i.second - 1)) {
                throw "Edges must not form a cycle";
            }

            dsu.join(i.first - 1, i.second - 1);
        }
    }

    int query(int size_a, int size_b, int a[], int b[]) {
        if (--m <= 0) throw "query(): the number of queries exceed";
        for (int i = 0; i < size_a; ++i) {
            if (a[i] < 0 or a[i] > n) throw "query(): number in the array must be a number not greater than N";
        }
        for (int i = 0; i < size_b; ++i) {
            if (b[i] < 0 or b[i] > n) throw "query(): number in the array must be a number not greater than N";
        }
        for (int i = 0; i < size_a; ++i) 
        for (int f = 0; f < size_b; ++f) {
            if (a[i] == b[f]) {
                throw "query(): 2 sets must be disjoint";
            }
        }

        for (int i = 0; i < size_a; ++i)
        for (int f = 0; f < size_b; ++f) {
            if (adjacency_matrix[a[i] - 1][b[f] - 1]) {
                return 1;
            }
        }
        return 0;
    }

    void setRoad(int a, int b) {
        if (a < 0 or a > n or b < 0 or b > n) throw "setRoad(): input must be positive integers not greater than N";
        assert(current_edge_id < n - 1);
        auto current_edge = edge_list[current_edge_id++];
        if (ordered(current_edge) != ordered<int>({a, b})) {
            sprintf(err_buf, "setRoad(): WA at edge #%d. Expected (%d, %d). Result (%d, %d)", current_edge_id, current_edge.first, current_edge.second, a, b);
            throw err_buf;
        }
        if (current_edge_id == n - 1) {
            throw "AC";
        }
        set_edge(edge_list[current_edge_id]);
    }
};

unique_ptr<Grader> grader;

int main(void) {
    int n, m;
    if (!(cin >> n)) n = -1;
    if (!(cin >> m)) m = -1;

    vector<pair<int, int>> edge_list;
    int u, v;
    while ((cin >> u >> v)) {
        edge_list.emplace_back(u, v);
    }

    try {
        grader.reset(new Grader(n, m, edge_list.begin(), edge_list.end()));
    } catch (const char* msg) {
        cout << "In correct input format: " << msg;
        return 0;
    }

    try {
        run(n);
        throw "WA: run() did not find all the edges";
    } catch (const char* msg) {
        cout << msg;
    }
    return 0;
}

int query(int size_a, int size_b, int a[], int b[]) {
    return grader->query(size_a, size_b, a, b);
}

void setRoad(int a, int b) {
    grader->setRoad(a, b);
}


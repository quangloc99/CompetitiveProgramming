/**
 * Author: Tran Quang Loc (darkkcyan)
 * This code need the standard library inorder to run.
 *
 * This code is designed to run with graph with multiple disconnected components.
 * This code is not desgined to return the direction for each edges, but it is still be done with a little modification.
 *
 * A little note that because of pointer, this code is a little "danger" to use, for example when
 * passing the pointer of the first element of a vector, and then changing the vector size, the
 * argument's memory will be automatically free.
 */
 
struct hierholzer {
  int n_nodes, n_edges;
  vector<pair<int, int>>* gr;     // first: the adjacency node; second: the id of the edges.
  vector<bool> rem;
  vector<int> stack, res;

  hierholzer(int n_nodes_, int n_edges_, vector<pair<int, int>>* gr_)
    :n_nodes(n_nodes_), n_edges(n_edges_), gr(gr_), rem(n_edges)
  { }

  void run(int start_node = 0) {
    stack.push_back(start_node);
    for (; len(stack); ) {
      int u = stack.back();
      while (len(gr[u]) and rem[gr[u].back().yy]) gr[u].pop_back();
      if (len(gr[u]) == 0) {
        res.push_back(u);
        stack.pop_back();
        continue;
      } 
      stack.push_back(gr[u].back().xx);
      rem[gr[u].back().yy] = 1;
    }
  }
};

/**
 * @file subtrees_and_paths.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Subtrees And [sic] Paths"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/subtrees-and-paths/problem
 *
 * @note The problem states that we are given a rooted tree, but this is a lie.
 *  Some of the inputs, e.g. 2.in, 3.in, result in a forest being created.
 */

#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

// TODO: put in shared header
#ifdef PDHKR_NO_OUTPUT_FILE
#undef PDHKR_OUTPUT_FILE
#endif  // PDHKR_NO_OUTPUT_FILE

// only necessary when writing to output file locally
#ifdef PDHKR_OUTPUT_FILE
#include <filesystem>
#include <fstream>
#endif  // PDHKR_OUTPUT_FILE

namespace {

// see note. define this to use the original tree_node implementation and
// related methods that work only if input results in an actual tree
#if defined(USE_TREE_NODE)
/**
 * Simple tree node with unique ownership over children.
 */
class tree_node {
public:
  /**
   * Ctor.
   *
   * Construct the tree with a given ID and value.
   */
  tree_node(
    unsigned int id,
    int value,
    std::unique_ptr<tree_node>&& left = nullptr,
    std::unique_ptr<tree_node>&& right = nullptr)
    : id_{id},
      value_{value},
      left_{std::move(left)},
      right_{std::move(right)}
  {}

  /**
   * Ctor.
   *
   * Construct the tree with a given ID, zero value.
   */
  tree_node(
    unsigned int id,
    std::unique_ptr<tree_node>&& left = nullptr,
    std::unique_ptr<tree_node>&& right = nullptr)
    : tree_node{id, 0, std::move(left), std::move(right)}
  {}

  /**
   * Return the node ID.
   */
  auto id() const noexcept { return id_; }

  /**
   * Return the node value.
   */
  auto value() const noexcept { return value_; }

  /**
   * Return reference to the node value.
   */
  auto& value() noexcept { return value_; }

  /**
   * Return const reference to left child.
   */
  const auto& left() const noexcept { return left_; }

  /**
   * Return const reference to right child.
   */
  const auto& right() const noexcept { return right_; }

  /**
   * Return reference to left child.
   */
  auto& left() noexcept { return left_; }

  /**
   * Return reference to right child.
   */
  auto& right() noexcept { return right_; }

private:
  unsigned int id_;
  int value_;
  std::unique_ptr<tree_node> left_;
  std::unique_ptr<tree_node> right_;
};

/**
 * Return address to a specific node in the tree.
 *
 * If the node with the given ID is not found, a reference to a
 * statically-allocated empty unique pointer will be returned.
 */
auto& get_node(std::unique_ptr<tree_node>& root, unsigned int id)
{
  // empty root
  static std::unique_ptr<tree_node> empty_node;
  if (!root)
    return empty_node;
  // ID matched
  if (root->id() == id)
    return root;
  // search subtrees
  auto& left_res = get_node(root->left(), id);
  auto& right_res = get_node(root->right(), id);
  // NOTE: we assume that there are no nodes with duplicate IDs
  if (left_res)
    return left_res;
  if (right_res)
    return right_res;
  return empty_node;
}

/**
 * Return address to specific node in the tree and the search path.
 *
 * If the node with the given ID is not found, a reference to a
 * statically-allocated empty unique pointer will be returned.
 */
auto& get_node(
  std::unique_ptr<tree_node>& root,
  std::deque<std::pair<unsigned int, int>>& path,
  unsigned int id)
{
  // empty root
  static std::unique_ptr<tree_node> empty_node;
  if (!root)
    return empty_node;
  // ID matched
  if (root->id() == id) {
    path.emplace_back(root->id(), root->value());
    return root;
  }
  // first search left subtree. copy original path to prevent clobbering
  auto left_path = path;
  left_path.emplace_back(root->id(), root->value());
  auto& left_res = get_node(root->left(), left_path, id);
  // found
  if (left_res) {
    path = left_path;
    return left_res;
  }
  // now search right subtree, again need copy
  auto right_path = path;
  right_path.emplace_back(root->id(), root->value());
  auto& right_res = get_node(root->right(), right_path, id);
  // found
  if (right_res) {
    path = right_path;
    return right_res;
  }
  return empty_node;
}

/**
 * Increment the value of all nodes in the tree by the given amount.
 */
void blanket_add(std::unique_ptr<tree_node>& root, int value)
{
  if (!root)
    return;
  // increment self and children
  root->value() += value;
  blanket_add(root->left(), value);
  blanket_add(root->right(), value);
}

/**
 * Return the max value of the nodes on the path between the given nodes.
 */
auto max_value(
  std::unique_ptr<tree_node>& root, unsigned int id_a, unsigned int id_b)
{
  static constexpr auto min_val = std::numeric_limits<int>::min();
  // if not root, return min value
  if (!root)
    return min_val;
  // find path to id_a
  std::deque<std::pair<unsigned int, int>> path_a;
  auto& node_a = get_node(root, path_a, id_a);
  if (!node_a)
    return min_val;
  // find path to id_b
  decltype(path_a) path_b;
  auto& node_b = get_node(root, path_b, id_b);
  if (!node_b)
    return min_val;
  // shared ancestor between id_a, id_b
  auto head = std::make_pair(root->id(), root->value());
  // prune shared ancestors from path_a, path_b
  while (path_a.size() && path_b.size()) {
    // if ancestors differ, break
    if (path_a.front().first != path_b.front().first)
      break;
    // otherwise, update nearest ancestor + prune
    head = path_a.front();
    path_a.pop_front();
    path_b.pop_front();
  }
  // tentatively, shared ancestor is max value
  auto res = head.second;
  // if path_a nonempty, take max with res
  if (path_a.size()) {
    auto a_max = std::max_element(
      path_a.begin(),
      path_a.end(),
      [](const auto& a, const auto& b) { return a.second < b.second; }
    );
    res = std::max(res, a_max->second);
  }
  // if path_b nonempty, take max with res
  if (path_b.size()) {
    auto b_max = std::max_element(
      path_b.begin(),
      path_b.end(),
      [](const auto& a, const auto& b) { return a.second < b.second; }
    );
    res = std::max(res, b_max->second);
  }
  return res;
}
// use more general implementation that treats problem as a graph problem.
// TODO: this is slower than tree when run on 13.in but doesn't fail on 2.in
#else
/**
 * Type alias for the edge map.
 *
 * Since there are only 100000 nodes max a 64-bit unsigned int more than
 * sufficient to keep track of the nodes. We just shift by 32 bits.
 */
using edge_map_type = std::unordered_map<std::uint64_t, bool>;

/**
 * Type alias for the node map.
 *
 * This tracks both the node and its value (an integer).
 */
using node_map_type = std::unordered_map<std::uint32_t, int>;

/**
 * Simple graph type for this problem.
 *
 * Nodes have values and edges are unweighted but directed. There is an
 * underlying assumption given in the problem that all nodes have unique IDs
 * and are numbered 1 through N, so N is the maximum number of nodes.
 */
class simple_graph {
public:
  /**
   * Return const reference to node map.
   */
  const auto& nodes() const noexcept { return nodes_; }

  /**
   * Return const reference to edge map.
   */
  const auto& edges() const noexcept { return edges_;}

  /**
   * Insert the specified node into the graph with a weight of zero.
   *
   * If the node ID already exists, nothing is done.
   */
  void insert_node(std::uint32_t id)
  {
    if (!has_node(id))
      nodes_[id] = 0;
  }

  /**
   * Insert the specified edge into the graph.
   *
   * If the edge already exists, nothing is done.
   */
  void insert_edge(std::uint32_t id_a, std::uint32_t id_b)
  {
    auto edge = make_edge(id_a, id_b);
    if (!has_edge(edge))
      edges_[edge] = true;
  }

  /**
   * Check if the graph contains the specified node.
   */
  bool has_node(std::uint32_t id) const
  {
    return nodes_.find(id) != nodes_.end();
  }

  /**
   * Check if the graph contains the specified edge.
   */
  bool has_edge(std::uint32_t id_a, std::uint32_t id_b) const
  {
    return edges_.find(make_edge(id_a, id_b)) != edges_.end();
  }

  /**
   * Check if the graph contains the specified edge.
   */
  bool has_edge(std::uint64_t edge) const
  {
    return edges_.find(edge) != edges_.end();
  }

  /**
   * Return number of nodes.
   */
  auto n_nodes() const
  {
    return nodes_.size();
  }

  /**
   * Return number of edges.
   */
  auto n_edges() const
  {
    return edges_.size();
  }

  /**
   * Return the value of the node.
   *
   * If the node does not exist an exception will be thrown.
   */
  auto value(std::uint32_t id) const
  {
    return nodes_.at(id);
  }

  /**
   * Return a reference to the value of the node.
   *
   * If the node does not exist an exception will be thrown.
   */
  auto& value(std::uint32_t id)
  {
    return nodes_.at(id);
  }

  /**
   * Convert two 32-bit node IDs into the 64-bit value representing an edge.
   *
   * @param id_a First node ID, will be left shifted
   * @param id_b Second node ID, not shifted
   */
  static std::uint64_t make_edge(std::uint32_t id_a, std::uint32_t id_b)
  {
    // copy into 64-bit value first to prevent overflow
    std::uint64_t edge = id_a;
    // NB: parentheses unnecesary but helpful for code clarity
    return (edge << CHAR_BIT * sizeof id_a) + id_b;
  }

private:
  node_map_type nodes_;
  edge_map_type edges_;
};

/**
 * Increment the value of all nodes by the given amount starting from a root.
 *
 * There is an underlying assumption that any subgraph starting from root is
 * a DAG, which for this problem should be true.
 */
void blanket_add(simple_graph& graph, std::uint32_t root, int value)
{
  // do nothing if root doesn't exist in the graph
  if (!graph.has_node(root))
    return;
  // node queue
  std::deque<std::uint32_t> nodes{root};
  // perform BFS
  while (nodes.size()) {
    // current node to consider
    auto cur = nodes.front();
    nodes.pop_front();
    // increment node value in graph
    graph.value(cur) += value;
    // add children to queue if connected
    for (decltype(graph.n_nodes()) i = 0; i < graph.n_nodes(); i++) {
      if (graph.has_edge(cur, i + 1))
        nodes.push_back(i + 1);
    }
  }
}

/**
 * Return the path from the root to the given node.
 *
 * This function treats the edges as undirected.
 */
std::vector<std::uint32_t> compute_path(
  const simple_graph& graph, std::uint32_t root, std::uint32_t tgt)
{
  // empty if no root
  if (!graph.has_node(root))
    return {};
  // node stack. we use the last element of the vector as "top" of stack
  std::vector<std::uint32_t> stack{root};
  // map of visited nodes so we don't revisit
  std::unordered_map<std::uint32_t, bool> visited;
  // perform DFS
  while (stack.size()) {
    // current node to consider
    auto cur = stack.back();
    visited[cur] = true;
    // if found, success
    if (cur == tgt)
      break;
    // add next unvisited child
    for (decltype(graph.n_nodes()) i = 0; i < graph.n_nodes(); i++) {
      // TODO: maybe need to add undirected edges in general?
      if (
        visited.find(i + 1) == visited.end() &&
        (graph.has_edge(cur, i + 1) || graph.has_edge(i + 1, cur))
      ) {
        stack.push_back(i + 1);
        break;
      }
    }
    // failed to add any children. pop from stack
    if (cur == stack.back())
      stack.pop_back();
  }
  // stack contains the search path
  return stack;
}

/**
 * Return the max value of the nodes on the path between root and given nodes.
 */
auto max_value(
  const simple_graph& graph,
  std::uint32_t root,
  std::uint32_t id_a,
  std::uint32_t id_b)
{
  // min value (failure)
  static constexpr auto min_value = std::numeric_limits<int>::min();
  // find paths for A and B
  auto path_a = compute_path(graph, root, id_a);
  auto path_b = compute_path(graph, root, id_b);
  // if either is empty, return min
  if (path_a.empty() || path_b.empty())
    return min_value;
  // otherwise, find closest ancestor. this is originally the root
  auto ancestor = root;
  // iterators for the two paths
  auto it_a = path_a.begin();
  auto it_b = path_b.begin();
  // iterate
  while (it_a != path_a.end() && it_b != path_b.end()) {
    // if the nodes are different, break
    if (*it_a != *it_b)
      break;
    // otherwise, update ancestor + continue
    ancestor = *it_a;
    it_a++;
    it_b++;
  }
  // start taking max values. start with ancestor
  auto res = graph.value(ancestor);
  // if partial path_a non-empty, get max
  if (std::distance(it_a, path_a.end())) {
    auto res_it = std::max_element(
      it_a,
      path_a.end(),
      [&graph](const auto& a, const auto& b)
      {
        return graph.value(a) < graph.value(b);
      }
    );
    res = std::max(res, graph.value(*res_it));
  }
  // repeat for partial path_b
  if (std::distance(it_b, path_b.end())) {
    auto res_it = std::max_element(
      it_b,
      path_b.end(),
      [&graph](const auto& a, const auto& b)
      {
        return graph.value(a) < graph.value(b);
      }
    );
    res = std::max(res, graph.value(*res_it));
  }
  // return result
  return res;
}
#endif  // !defined(USE_TREE_NODE)

}  // namespace

// PDHKR_OUTPUT_FILE defined for local builds to enable automated testing
#if defined(PDHKR_OUTPUT_FILE)
int main(int argc, char *argv[])
#else
int main()
#endif  // !defined(PDHKR_OUTPUT_FILE)
{
  // output stream
#if defined(PDHKR_OUTPUT_FILE)
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " OUTFILE < INFILE" << std::endl;
    return EXIT_SUCCESS;
  }
  std::ofstream out{argv[1]};
  auto& sink = out;
  // auto& sink
#else
  auto& sink = std::cout;
#endif  // !defined(PDHKR_OUTPUT_FILE)
  // number of tree nodes
  unsigned int n_nodes;
  std::cin >> n_nodes;
  // root node only if using tree_node, otherwise use graph
#if defined(USE_TREE_NODE)
  auto root = std::make_unique<tree_node>(1);
#else
  simple_graph graph;
#endif  // !defined(USE_TREE_NODE)
  // handle edges
  for (decltype(n_nodes) i = 0; i < n_nodes - 1; i++) {
    // read node IDs (1-indexed)
#if defined(USE_TREE_NODE)
    decltype(n_nodes) id_a, id_b;
#else
    std::uint32_t id_a, id_b;
#endif  // !defined(USE_TREE_NODE)
    std::cin >> id_a;
    std::cin >> id_b;
    // if b < a, swap them. we treat a as the parent (lower ID)
    if (id_b < id_a)
      std::swap(id_a, id_b);
#if defined(USE_TREE_NODE)
    // find node a
    auto& node_a = get_node(root, id_a);
    if (!node_a) {
      std::cerr << "Failed to find node with ID " << id_a << std::endl;
      return EXIT_FAILURE;
    }
    // otherwise, insert. this is very simplistic, fill either left or right
    // child depending on whether or not left is already full
    if (!node_a->left())
      node_a->left() = std::make_unique<tree_node>(id_b);
    else if (!node_a->right())
      node_a->right() = std::make_unique<tree_node>(id_b);
    else {
      std::cerr << "Both children of node with ID " << id_a << " are filled" <<
        std::endl;
      return EXIT_FAILURE;
    }
#else
  // insert nodes. nodes always start with value zero
  graph.insert_node(id_a);
  graph.insert_node(id_b);
  // insert edge. this is a directed edge
  graph.insert_edge(id_a, id_b);
#endif  // !defined(USE_TREE_NODE)
  }
  // number of queries
  unsigned int n_queries;
  std::cin >> n_queries;
  // handle queries
  for (decltype(n_queries) i = 0; i < n_queries; i++) {
    // read query type
    std::string query;
    std::cin >> query;
    // add value to all nodes rooted at subtree
    if (query == "add") {
      // read node ID and value
#if defined(USE_TREE_NODE)
      decltype(n_nodes) id;
#else
      std::uint32_t id;
#endif  // !defined(USE_TREE_NODE)
      int value;
      std::cin >> id;
      std::cin >> value;
      // find node + add values
#if defined(USE_TREE_NODE)
      auto& node = get_node(root, id);
      blanket_add(node, value);
#else
      blanket_add(graph, id, value);
#endif  // !defined(USE_TREE_NODE)
    }
    // report max value on path from a to b
    else if (query == "max") {
      // read node IDs
      decltype(n_nodes) id_a, id_b;
      std::cin >> id_a;
      std::cin >> id_b;
      // print max value in path
#if defined(USE_TREE_NODE)
      sink << max_value(root, id_a, id_b) << std::endl;
      // TODO
#else
      // we assumed 1 is always the root in this problem
      sink << max_value(graph, 1, id_a, id_b) << std::endl;
#endif  // !defined(USE_TREE_NODE)
    }
    // unknown, error
    else {
      std::cerr << "Error: unknown query " << query << std::endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

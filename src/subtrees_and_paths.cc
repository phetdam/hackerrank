/**
 * @file subtrees_and_paths.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Subtrees And [sic] Paths"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/subtrees-and-paths/problem
 */

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

namespace {

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

}  // namespace

int main()
{
  // number of tree nodes
  unsigned int n_nodes;
  std::cin >> n_nodes;
  // root node
  auto root = std::make_unique<tree_node>(1);
  // handle edges
  for (decltype(n_nodes) i = 0; i < n_nodes - 1; i++) {
    // read node IDs (1-indexed)
    decltype(n_nodes) id_a, id_b;
    std::cin >> id_a;
    std::cin >> id_b;
    // if b < a, swap them. we treat a as the parent (lower ID)
    if (id_b < id_a)
      std::swap(id_a, id_b);
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
      std::cerr << "Both children of node with ID " << id_a <<
        " are filled" << std::endl;
      return EXIT_FAILURE;
    }
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
      decltype(n_nodes) id;
      int value;
      std::cin >> id;
      std::cin >> value;
      // find node + add values
      auto& node = get_node(root, id);
      blanket_add(node, value);
    }
    // report max value on path from a to b
    else if (query == "max") {
      // read node IDs
      decltype(n_nodes) id_a, id_b;
      std::cin >> id_a;
      std::cin >> id_b;
      // print max value in path
      std::cout << max_value(root, id_a, id_b) << std::endl;
    }
    // unknown, error
    else {
      std::cerr << "Error: unknown query " << query << std::endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

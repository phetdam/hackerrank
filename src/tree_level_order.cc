/**
 * @file tree_level_order.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Tree: Level Order Traversal"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/tree-level-order-traversal/problem
 */

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <fstream>
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

namespace {

/**
 * Binary tree node class template with unique child ownership.
 *
 * @note This implementation is copied from `tree_top_view.cc`.
 *
 * @tparam T value
 */
template <typename T>
class binary_tree_node {
public:
  using value_type = T;

  /**
   * Ctor.
   *
   * @param value Node value
   * @param left Node left child
   * @param right Node right child
   */
  binary_tree_node(
    T&& value,
    std::unique_ptr<binary_tree_node<T>>&& left = nullptr,
    std::unique_ptr<binary_tree_node<T>>&& right = nullptr)
    : value_{std::move(value)}, left_{std::move(left)}, right_{std::move(right)}
  {}

  /**
   * Return const reference to value.
   */
  const auto& value() const noexcept
  {
    return value_;
  }

  /**
   * Return reference to value.
   */
  auto& value() noexcept
  {
    return value_;
  }

  /**
   * Return const reference to left child.
   */
  const auto& left() const noexcept
  {
    return left_;
  }

  /**
   * Return reference to left child.
   */
  auto& left() noexcept
  {
    return left_;
  }

  /**
   * Return const reference to right child.
   */
  const auto& right() const noexcept
  {
    return right_;
  }

  /**
   * Return reference to right child.
   */
  auto& right() noexcept
  {
    return right_;
  }

private:
  T value_;
  std::unique_ptr<binary_tree_node<T>> left_;
  std::unique_ptr<binary_tree_node<T>> right_;
};

/**
 * Perform a binary search tree insertion of a value into a binary tree.
 *
 * @note This implementation is copied from `tree_top_view.cc`.
 *
 * @tparam T value type
 *
 * @param root Tree root
 * @param value Value to insert
 */
template <typename T>
void bst_insert(std::unique_ptr<binary_tree_node<T>>& root, T&& value)
{
  if (!root)
    throw std::runtime_error{"tree root is nullptr"};
  // left insert
  if (value <= root->value()) {
    // if no left child, insert, otherwise recurse
    if (!root->left())
      root->left() = std::make_unique<binary_tree_node<T>>(std::move(value));
    else
      bst_insert(root->left(), std::move(value));
  }
  // right insert
  else {
    // if no right child, insert, otherwise recurse
    if (!root->right())
      root->right() = std::make_unique<binary_tree_node<T>>(std::move(value));
    else
      bst_insert(root->right(), std::move(value));
  }
}

/**
 * Wrapper class for streaming a BFS ordering of a `binary_tree_node<T>` tree.
 *
 * The BFS ordering is equivalent to saying the ordering is level first.
 *
 * @tparam T value type
 */
template <typename T>
class binary_tree_bfs_view {
public:
  /**
   * Ctor.
   *
   * @param root Reference to tree root
   */
  binary_tree_bfs_view(const std::unique_ptr<binary_tree_node<T>>& root)
    : root_{root}
  {}

  /**
   * Return const reference to the tree root.
   */
  const auto& root() const noexcept
  {
    return root_;
  }

private:
  const std::unique_ptr<binary_tree_node<T>>& root_;
};

/**
 * Print the BFS, or level order, view of a binary tree to a stream.
 *
 * @tparam T value type
 *
 * @param view Level order view wrapper with reference to the tree root
 */
template <typename T>
auto& operator<<(std::ostream& out, const binary_tree_bfs_view<T>& view)
{
  // reference to the tree root
  const auto& root = view.root();
  // empty tree
  if (!root)
    return out;
  // perform BFS and print nodes
  std::deque<decltype(root.get())> queue{root.get()};
  while (queue.size()) {
    // current node
    auto cur = queue.front();
    queue.pop_front();
    // print extra space if root was printed already
    if (cur != root.get())
        out << " ";
    // print current value + add children
    out << cur->value();
    if (cur->left())
      queue.push_back(cur->left().get());
    if (cur->right())
      queue.push_back(cur->right().get());
  }
  return out;
}

}  // namespace

// note: main is a near-identical copy of the tree_top_view.cc main
int main()
{
// building as standalone test program
#if defined(PDHKR_TEST)
  // write to stringstream, read from PDHKR_TEST_INPUT
  std::stringstream fout;
  std::ifstream fin{PDHKR_TEST_INPUT};
  // fans provides the expected output
  std::ifstream fans{PDHKR_TEST_OUTPUT};
#else
  // for local run, this is simply std::cout
  auto& fout = std::cout;
  auto& fin = std::cin;
#endif  // !defined(PDHKR_TEST)
  // number of nodes to insert
  unsigned int n_nodes;
  fin >> n_nodes;
  // tree root
  std::unique_ptr<binary_tree_node<unsigned int>> root;
  // insert nodes into tree
  for (decltype(n_nodes) i = 0; i < n_nodes; i++) {
    // read from stream
    decltype(root)::element_type::value_type value;
    fin >> value;
    // create root
    if (!root)
      root = std::make_unique<decltype(root)::element_type>(std::move(value));
    // otherwise insert
    else
      bst_insert(root, std::move(value));
  }
  // write level order view to stream
  fout << binary_tree_bfs_view{root} << std::endl;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  // std::vector used here to trigger vector comparison
  using value_type = std::vector<decltype(root)::element_type::value_type>;
  return pdhkr::exit_compare<value_type>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

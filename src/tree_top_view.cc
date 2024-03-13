/**
 * @file tree_top_view.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Tree: Top View"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/tree-top-view/problem
 *
 * @note The BST node insertion and input format were inferred from the
 *  HackerRank submission C++11 template code and reworked. In particular no
 *  `Solution` class is necessary and type signedness is correct.
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

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
 * @note Duplicates are left-inserted to mirror original problem's logic.
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
 * Wrapper class for streaming a `binary_tree_node<T>` tree's top view.
 */
template <typename T>
class binary_tree_top_view {
public:
  /**
   * Ctor.
   *
   * @param root Reference to tree root
   */
  binary_tree_top_view(const std::unique_ptr<binary_tree_node<T>>& root)
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
 * Print the top view of a binary tree to a stream.
 *
 * This is an inorder traversal of all the leftmost children that can be seen
 * if looking down from above on tree, the root, and the rightmost children.
 * All values are printed separated by spaces as required by the problem.
 *
 * @note Need some way to identify nodes that are outside the "shadow" of their
 *  parent as those somehow are counted as being "visible".
 *
 * @tparam T value type
 *
 * @param view Top view wrapper with reference to the tree root
 */
template <typename T>
auto& operator<<(std::ostream& out, const binary_tree_top_view<T>& view)
{
  // reference to the tree root
  const auto& root = view.root();
  // empty tree
  if (!root)
    return out;
  // vector to hold left children + root values (for storing DFS progress)
  std::vector<T> values;
  // current pointer to tree node
  auto cur = root.get();
  // add root + all left children values as we go deeper
  while (cur) {
    values.push_back(cur->value());
    cur = cur->left().get();
  }
  // going backwards, print deepest left child all the way through root
  for (auto it = values.rbegin(); it != values.rend(); it++) {
    if (std::distance(it, values.rbegin()))
      out << " ";
    out << *it;
  }
  // reset cur to root's right child
  cur = root->right().get();
  // print all right children values as we go deeper
  while (cur) {
    out << " " << cur->value();
    cur = cur->right().get();
  }
  return out;
}

}  // namespace

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
  // write top view to stream
  fout << binary_tree_top_view{root} << std::endl;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  // std::vector used here to trigger vector comparison
  using value_type = std::vector<decltype(root)::element_type::value_type>;
  return pdhkr::exit_compare<value_type>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

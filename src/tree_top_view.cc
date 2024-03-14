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
 * Write the values of a container to a stream separated by spaces.
 *
 * This is useful for printing the problem outputs as well as debugging.
 *
 * @tparam Container *Container* with streamable values
 *
 * @param out Output stream
 * @param values Values to write
 */
template <typename Container>
void write_container(std::ostream& out, const Container& values)
{
  for (auto it = values.begin(); it != values.end(); it++) {
    if (std::distance(values.begin(), it))
      out << " ";
    out << *it;
  }
}

/**
 * Return the tree's "canopy" and the range of moves from root it covers.
 *
 * That is, we return the inorder traversal of all nodes that are left children
 * of left subtrees of the root and all nodes that right children of right
 * subtrees of the root. We also return the range of the canopy, which is the
 * interval centered at root that indicates the canopy's coverage.
 *
 * One can get a visual representation of the canopy with the below tree:
 *
 *     8
 *    / \
 *   4   10
 *    \   \
 *     6   13
 *        /
 *      12
 *
 * Here the tree canopy consists of 4, 8, 10, 13 and has a range of [-1, 2].
 *
 * @tparam T value type
 *
 * @param root Tree root
 */
template <typename T>
auto compute_canopy(const std::unique_ptr<binary_tree_node<T>>& root)
{
  // pair holding the canopy nodes and canopy coverage
  std::pair<std::vector<T>, std::pair<int, int>> res{{}, {0, 0}};
  // nothing interesting here
  if (!root)
    return res;
  // current pointer to root's left child
  auto cur = root->left().get();
  // add root + all left children values as we go deeper + decrement left limit
  while (cur) {
    res.first.push_back(cur->value());
    res.second.first--;
    cur = cur->left().get();
  }
  // reverse the nodes (in backwards order right now)
  // note: could use deque so no reversal is necessary
  std::reverse(res.first.begin(), res.first.end());
  // add root's value to the canopy
  res.first.push_back(root->value());
  // reset cur to root's right child
  cur = root->right().get();
  // add all right children values as we go deeper + increment right limit
  while (cur) {
    res.first.push_back(cur->value());
    res.second.second++;
    cur = cur->right().get();
  }
  // complete with canopy nodes + canopy range
  return res;
}

/**
 * Return the rebel nodes that stick out from underneath the canopy.
 *
 * This is the implementation function that performs a DFS and adds only those
 * nodes whose position is outside the prescribed canopy range.
 *
 * See below for details on what the rebel nodes are.
 *
 * @tparam T value type
 *
 * @param root Tree root
 * @param canopy_range Tree canopy range (assumed valid)
 * @param pos Current position relative to root
 * @param rebels Pair holding the left and right rebel nodes
 */
template <typename T>
void compute_rebels(
  const std::unique_ptr<binary_tree_node<T>>& root,
  const std::pair<int, int>& canopy_range,
  int pos,
  std::pair<std::vector<T>, std::vector<T>>& rebels)
{
  if (!root)
    return;
  // add left subtree rebels
  compute_rebels(root->left(), canopy_range, pos - 1, rebels);
  // if outside of left range limit, add to left rebels
  if (pos < canopy_range.first)
    rebels.first.push_back(root->value());
  // else if outside of right range limit, add to right rebels
  else if (pos > canopy_range.second)
    rebels.second.push_back(root->value());
  // add right subtree rebels
  compute_rebels(root->right(), canopy_range, pos + 1, rebels);
}

/**
 * Return the rebel nodes that stick out from underneath the canopy.
 *
 * That is, given the canopy range as computed by `compute_canopy`, return the
 * inorder traversal of all tree nodes that are outside the canopy's range.
 * These are separated into the left and right orderings to make it easy to
 * produce the entire "top view" of the binary tree.
 *
 * One can get a visual representation of the rebel nodes with the below tree:
 *
 *     8
 *    / \
 *   1   10
 *    \   \
 *     6   20
 *    /   /
 *   5  12
 *  /     \
 * 4       14
 *          \
 *           16
 *
 * Here the rebel nodes are 4, 16 and split into left rebels [4] and right
 * rebels [16]. The canopy range in this case is [-1, 2].
 *
 * @tparam T value type
 *
 * @param root Tree root
 * @param canopy_range Tree canopy range (assumed valid)
 */
template <typename T>
auto compute_rebels(
  const std::unique_ptr<binary_tree_node<T>>& root,
  const std::pair<int, int>& canopy_range)
{
  // pair holding the left and right rebel nodes
  std::pair<std::vector<T>, std::vector<T>> rebels;
  // compute rebels and return
  compute_rebels(root, canopy_range, 0, rebels);
  return rebels;
}

/**
 * Wrapper class for streaming a `binary_tree_node<T>` tree's top view.
 *
 * @tparam T value type
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
 * @note Still has issues on some test cases likely because "rebels" are being
 *  printed that are actually hidden by "sub-canopy" nodes.
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
  // compute tree canopy values and range
  auto [canopy_values, canopy_range] = compute_canopy(root);
  // compute left and right rebels
  auto [left_rebels, right_rebels] = compute_rebels(root, canopy_range);
  // write left rebels
  write_container(out, left_rebels);
  // write canopy values. prepend with space if there are left rebels
  if (left_rebels.size() && canopy_values.size())
    out << " ";
  write_container(out, canopy_values);
  // write right rebels. prepend with space if there are right rebels
  // note: resulted in some UB if just the space is written (without a value)
  if (right_rebels.size() && canopy_values.size())
    out << " ";
  write_container(out, right_rebels);
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

/**
 * @file swap_tree_nodes.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Swap Nodes [Algo]"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/swap-nodes-algo/problem
 *
 * @note The "[Algo]" is part of the actual title.
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <ostream>
#include <memory>
#include <utility>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

namespace {

/**
 * Simple binary tree node with unique child ownership that holds a node ID.
 */
class binary_tree_node {
public:
  /**
   * Ctor.
   *
   * @param id Node ID
   * @param left Left child to own
   * @param right Right child to own
   */
  binary_tree_node(
    unsigned int id,
    std::unique_ptr<binary_tree_node>&& left = nullptr,
    std::unique_ptr<binary_tree_node>&& right = nullptr)
    : id_{id}, left_{std::move(left)}, right_{std::move(right)}
  {}

  /**
   * Return node ID.
   */
  auto id() const noexcept
  {
    return id_;
  }

  /**
   * Return reference to the node ID.
   */
  auto& id() noexcept
  {
    return id_;
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
  unsigned int id_;
  std::unique_ptr<binary_tree_node> left_;
  std::unique_ptr<binary_tree_node> right_;
};

/**
 * Find the node with the specified ID in the binary tree.
 *
 * If the node cannot be found then a reference to an empty node is returned.
 *
 * @param root Tree root node
 * @param id Target node ID
 */
auto& find_node(std::unique_ptr<binary_tree_node>& root, unsigned int id)
{
  // empty root to use if unable to find the right node
  static std::unique_ptr<binary_tree_node> empty;
  // root empty
  if (!root)
    return empty;
  // found target ID
  if (root->id() == id)
    return root;
  // check left subtree, return if success
  auto& left = find_node(root->left(), id);
  if (left)
    return left;
  // check right subtree, return if success
  auto& right = find_node(root->right(), id);
  if (right)
    return right;
  // failed, return empty
  return empty;
}

/**
 * Retrieve tree value IDs in the inorder depth-first traversal order.
 *
 * @param root Tree root node
 * @param ids Vector to store the retrieved IDs in
 */
void inorder_tree_ids(
  const std::unique_ptr<binary_tree_node>& root, std::vector<unsigned int>& ids)
{
  if (!root)
    return;
  // add left subtree IDs, own ID, and then right subtree IDs
  inorder_tree_ids(root->left(), ids);
  ids.push_back(root->id());
  inorder_tree_ids(root->right(), ids);
}

/**
 * Retrieve tree value IDs in the inorder depth-first traversal order.
 *
 * @param root Tree root node
 */
inline auto inorder_tree_ids(const std::unique_ptr<binary_tree_node>& root)
{
  std::vector<unsigned int> ids;
  inorder_tree_ids(root, ids);
  return ids;
}

/**
 * Object print wrapper class.
 *
 * This helpers to separate `operator<<` overloading for `printer<T>` versus
 * the `T` itself by requiring explicit `printer<T>` overloads.
 *
 * @tparam T type
 */
template <typename T>
class printer {
public:
  /**
   * Ctor.
   *
   * @param obj Const reference to an object
   */
  printer(const T& obj) : obj_{obj} {}

  /**
   * Return const reference to the object.
   */
  const auto& obj() const noexcept
  {
    return obj_;
  }

  /**
   * Return const pointer to the object.
   */
  const T* operator->() const noexcept
  {
    return &obj_;
  }

private:
  const T& obj_;
};

/**
 * Stream a `printer<std::vector<T>` to the output stream.
 *
 * @tparam T Vector element type
 *
 * @param out Output stream
 * @param values Vector printer to print
 */
template <typename T>
auto& operator<<(std::ostream& out, const printer<std::vector<T>>& values)
{
  for (auto it = values->begin(); it != values->end(); it++) {
    if (std::distance(values->begin(), it))
      out << " ";
    out << *it;
  }
  return out;
}

/**
 * Swap the subtrees for the nodes at the specified depth.
 *
 * @note The depth of the root node is zero as is standard.
 *
 * @param root Tree root node
 * @param depth Depth of nodes whose subtrees should be swapped. If the depth
 *  requested exceeds the maximum depth of the tree, nothing is done.
 */
void swap_subtrees(std::unique_ptr<binary_tree_node>& root, unsigned int depth)
{
  // skip if empty
  if (!root)
    return;
  // swap subtrees of current node
  if (!depth)
    std::swap(root->left(), root->right());
  // call on children
  depth--;
  swap_subtrees(root->left(), depth);
  swap_subtrees(root->right(), depth);
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
#elif defined(PDHKR_LOCAL)
  // for local run, this is simply std::cout
  auto& fout = std::cout;
  auto& fin = std::cin;
#else
  // as-is from HackerRank but with std:: prefix. this is unsafe
  std::ofstream fout(std::getenv("OUTPUT_PATH"));
  auto& fin = std::cin;
#endif  // !defined(PDHKR_TEST) && !defined(PDHKR_LOCAL)
  // number of nodes
  unsigned int n_nodes;
  fin >> n_nodes;
  // root node
  std::unique_ptr<binary_tree_node> root;
  // read children for each node's children. IDs start from 1
  for (decltype(n_nodes) i = 0; i < n_nodes; i++) {
    // populate root if necessary
    if (!root)
      root = std::make_unique<decltype(root)::element_type>(i + 1);
    // read child node IDs. can be -1 to represent null
    int left_id;
    int right_id;
    fin >> left_id;
    fin >> right_id;
    // find parent and fill in children (if not negative)
    auto& parent = find_node(root, i + 1);
    if (left_id > 0)
      parent->left() = std::make_unique<decltype(root)::element_type>(left_id);
    if (right_id > 0)
      parent->right() = std::make_unique<decltype(root)::element_type>(right_id);
  }
  // number of subtree swaps
  unsigned int n_swaps;
  fin >> n_swaps;
  // for each swap depth, swap all node subtrees, print the inorder traversal
  for (decltype(n_swaps) i = 0; i < n_swaps; i++) {
    // read swap depth as depth starting from 1
    decltype(i) swap_depth;
    fin >> swap_depth;
    // swap node subtrees for multiples of the swap depth. -1 to standardize
    // note: probably more efficient if we took the maximum depth of the tree
    // first but this was still fast enough for all the test cases
    for (decltype(n_nodes) j = 1; j * swap_depth <= n_nodes; j++)
      swap_subtrees(root, j * swap_depth - 1);
    fout << printer{inorder_tree_ids(root)} << std::endl;
  }
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(inorder_tree_ids(nullptr))>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

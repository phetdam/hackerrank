/**
 * @file node_from_tail.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Get Node Value"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/get-the-value-of-the-node-at-a-
 *  specific-position-from-the-tail/problem
 *
 * @note The URL of the problem is more descriptive than the title.
 */

#include <cstdlib>
#include <istream>
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
 * Linked list node with unique ownership semantics.
 *
 * @note This implementation is copied from `merge_sorted_linked_lists.cc`.
 *
 * @tparam T Node value type
 */
template <typename T>
class list_node {
public:
  using value_type = T;

  /**
   * Ctor.
   *
   * @param value Node value
   */
  list_node(T value) : value_{std::move(value)} {}

  /**
   * Ctor.
   *
   * @param value Node value
   * @param next Next node
   */
  list_node(T value, std::unique_ptr<list_node<T>>&& next)
    : value_{std::move(value)}, next_{std::move(next)}
  {}

  /**
   * Return const reference to node value.
   */
  const auto& value() const noexcept
  {
    return value_;
  }

  /**
   * Return reference to node value.
   */
  auto& value() noexcept
  {
    return value_;
  }

  /**
   * Return const reference to next node.
   */
  const auto& next() const noexcept
  {
    return next_;
  }

  /**
   * Return reference to next node.
   */
  auto& next() noexcept
  {
    return next_;
  }

private:
  T value_;
  std::unique_ptr<list_node<T>> next_;
};

/**
 * Create a linked list and fill it with values read from a stream.
 *
 * The input format is as described in the problem.
 *
 * @note This implementation is copied from `merge_sorted_linked_lists.cc`.
 *
 * @param in Input stream
 */
auto create_list(std::istream& in)
{
  // linked list length, next value to read
  unsigned int list_len;
  unsigned int value;
  // linked list + pointer to last node
  std::unique_ptr<list_node<decltype(value)>> head;
  auto tail = head.get();
  // read inputs and insert into linked list
  in >> list_len;
  for (decltype(list_len) j = 0; j < list_len; j++) {
    // read and create node
    in >> value;
    auto new_node = std::make_unique<decltype(head)::element_type>(value);
    // create list head + set tail
    if (!head) {
      head = std::move(new_node);
      tail = head.get();
    }
    // add node + update tail
    else {
      tail->next() = std::move(new_node);
      tail = tail->next().get();
    }
  }
  return head;
}

/**
 * Return value that if `tail_pos` from the tail of the linked list.
 *
 * @tparam T Node value type
 *
 * @param tail_pos Offset from the linked list tail
 */
template <typename T>
auto position_from_tail(const list_node<T>& head, unsigned int tail_pos)
{
  // cursor to current node address
  auto cur = &head;
  // compute list length
  unsigned int list_len = 0;
  while (cur) {
    list_len++;
    cur = cur->next().get();
  }
  // although guaranteed in the problem this could happen in reality
  if (tail_pos >= list_len)
    throw std::runtime_error{"tail_pos indexes past linked list head"};
  // compute position from head. if tail_pos is 0, the pos is list_len - 1
  auto pos = list_len - tail_pos - 1;
  // find pos element and return value
  cur = &head;
  for (decltype(pos) i = 0; i < pos; i++)
    cur = cur->next().get();
  return cur->value();
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
  // number of test cases
  unsigned int n_cases;
  fin >> n_cases;
  // handle test cases
  for (decltype(n_cases) i = 0; i < n_cases; i++) {
    // create linked list
    auto head = create_list(fin);
    // read position from tail
    unsigned int tail_pos;
    fin >> tail_pos;
    // write target value
    fout << position_from_tail(*head, tail_pos) << "\n";
  }
  // flush fout when done
  fout << std::flush;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  // value_type type member from the list_node template
  using value_type = decltype(create_list(fin))::element_type::value_type;
  // using std::vector<value_type> triggers the vector compare routine
  return pdhkr::exit_compare<std::vector<value_type>>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

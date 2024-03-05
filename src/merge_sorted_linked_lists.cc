
/**
 * @file merge_sorted_linked_lists.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Merge two sorted linked lists"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/merge-two-sorted-linked-lists/problem
 */

#include <cstdlib>
#include <istream>
#include <iostream>
#include <ostream>
#include <memory>
#include <type_traits>
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
 * Print an entire linked list.
 *
 * Values are space-separated and no trailing `(null)` is printed.
 *
 * @param out Output stream
 * @param head Head of linked list to print
 */
template <typename T>
auto& operator<<(
  std::ostream& out,
  const std::unique_ptr<list_node<T>>& head)
{
  auto cur = head.get();
  while (cur) {
    // print separating space after head
    if (cur != head.get())
      out << " ";
    out << cur->value();
    cur = cur->next().get();
  }
  return out;
}

/**
 * Merge two sorted linked lists into a new one.
 *
 * @param head_a First sorted linked list
 * @param head_b Second sorted linked list
 */
template <typename T>
auto merge_lists(
  const std::unique_ptr<list_node<T>>& head_a,
  const std::unique_ptr<list_node<T>>& head_b)
{
  // new list
  std::unique_ptr<list_node<T>> head;
  // cursors
  auto cur_a = head_a.get();
  auto cur_b = head_b.get();
  auto cur = head.get();
  // lambda for inserting list a, list b values into new list
  auto insert_new_node = [&cur, &head](auto new_value)
  {
    auto new_node = std::make_unique<list_node<T>>(new_value);
    // head unset
    if (!head) {
      head = std::move(new_node);
      cur = head.get();
    }
    // append to head
    else {
      cur->next() = std::move(new_node);
      cur = cur->next().get();
    }
  };
  // while we aren't done with both lists
  while (cur_a && cur_b) {
    // insert cur_a before cur_b while possible
    while (cur_a && cur_b && cur_a->value() <= cur_b->value()) {
      insert_new_node(cur_a->value());
      cur_a = cur_a->next().get();
    }
    // insert cur_b before cur_a while possible
    while (cur_a && cur_b && cur_b->value() <= cur_a->value()) {
      insert_new_node(cur_b->value());
      cur_b = cur_b->next().get();
    }
  }
  // finish list a. head may still be empty
  while (cur_a) {
    insert_new_node(cur_a->value());
    cur_a = cur_a->next().get();
  }
  // finish list b. head may still be empty
  while (cur_b) {
    insert_new_node(cur_b->value());
    cur_b = cur_b->next().get();
  }
  return head;
}

/**
 * Create a linked list and fill it with values read from a stream.
 *
 * The input format is as described in the problem.
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
  // read inputs and insert into first linked list
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
    // create linked lists
    auto head_a = create_list(fin);
    auto head_b = create_list(fin);
    // create merged list + print
    auto head = merge_lists(head_a, head_b);
    fout << head << std::endl;
  }
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

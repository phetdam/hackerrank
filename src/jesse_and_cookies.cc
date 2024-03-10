/**
 * @file jesse_and_cookies.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Jesse and Cookies"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/jesse-and-cookies/problem
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <type_traits>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

namespace {

/**
 * Return number of times values are mixed before they all reach the target.
 *
 * Letting, `m_1` and `m_2` be the lowest two values at any iteration, mixing
 * results in the `m_1 + 2 * m_2` value being added to the value vector while
 * removing the original `m_1` and `m_2` values from the vector. This means
 * that the returned mixing count value will never exceed `values.size() - 1`.
 *
 * @param values Values vector
 * @param target Target value to reach via mixing
 * @returns Mixing count, -1 if not possible
 */
int mixing_count(const std::vector<unsigned int>& values, unsigned long target)
{
  // not possible if values is empty
  if (values.empty())
    return -1;
  // mixing count
  int count = 0;
  // insert elements into min queue
  using queue_type = std::priority_queue<
    std::decay_t<decltype(values)>::value_type,
    std::decay_t<decltype(values)>,
    std::greater<std::decay_t<decltype(values)>::value_type>
  >;
  queue_type queue{values.begin(), values.end()};
  // perform mixing while we can still mix
  while (queue.size() >= 2) {
    // if all remaining values >= target, we are done
    if (queue.top() >= target)
      return count;
    // get minimum + delete from values
    auto m_1 = queue.top();
    queue.pop();
    auto m_2 = queue.top();
    queue.pop();
    // add mixed value and increment count
    queue.push(m_1 + 2 * m_2);
    count++;
  }
  if (queue.top() >= target)
    return count;
  // otherwise failed
  return -1;
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
  // read number of values + target value
  unsigned int n_values;
  decltype(n_values) target;
  fin >> n_values;
  fin >> target;
  // read values
  std::vector<unsigned int> values(n_values);
  for (auto& value : values)
    fin >> value;
  // write mixing count (-1 if not possible)
  fout << mixing_count(values, target) << std::endl;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(mixing_count(values, 0))>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

/**
 * @file array_manipulation.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Array Manipulation"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/crush/problem
 *
 * @note This naive solution times out on the larger test cases. Probably the
 *  right approach here is to perform an O(nlog(n)) merging of the affected
 *  array intervals to produce a final set of array intervals with updated
 *  values and to then take the maximum value across this set.
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <fstream>
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

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
  auto& fout = std::cout;
  auto& fin = std::cin;
#endif  // !defined(PDHKR_TEST)
  // array size
  unsigned int array_size;
  fin >> array_size;
  // number of operations
  unsigned int n_ops;
  fin >> n_ops;
  // create array and perform operations. we need long long (64 bits) type
  // since based on the problem constraints, max array value can be 2e14
  std::vector<unsigned long long> values(array_size, 0UL);
  for (decltype(n_ops) i = 0; i < n_ops; i++) {
    // left index, right index, summand
    unsigned int l_index, r_index, summand;
    fin >> l_index;
    fin >> r_index;
    fin >> summand;
    // apply to array
    std::transform(
      values.begin() + (l_index - 1),
      values.begin() + r_index,
      values.begin() + (l_index - 1),
      [summand](const auto& v) { return v + summand; }
    );
  }
  // write result
  fout << *std::max_element(values.begin(), values.end());
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(values)::value_type>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
  return EXIT_SUCCESS;
}

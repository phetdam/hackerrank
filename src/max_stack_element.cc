/**
 * @file max_stack_element.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Maximum Element"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/maximum-element/problem
 */

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
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
#elif defined(PDHKR_LOCAL)
  // for local run, this is simply std::cout
  auto& fout = std::cout;
  auto& fin = std::cin;
#else
  // as-is from HackerRank but with std:: prefix. this is unsafe
  std::ofstream fout(std::getenv("OUTPUT_PATH"));
  auto& fin = std::cin;
#endif  // !defined(PDHKR_TEST) && !defined(PDHKR_LOCAL)
  // read number of queries
  unsigned int n_queries;
  fin >> n_queries;
  // stack of values
  std::deque<unsigned int> stack;
  // read queries into stack
  for (decltype(n_queries) i = 0; i < n_queries; i++) {
    // query and possibly-used argument
    unsigned int query;
    decltype(stack)::value_type arg;
    // read query and switch off value
    fin >> query;
    switch (query) {
      // push value onto stack
      case 1:
        fin >> arg;
        stack.push_back(arg);
        break;
      // pop value from stack
      case 2:
        stack.pop_back();
        break;
      // print max value in stack
      case 3:
        fout << *std::max_element(stack.begin(), stack.end()) << "\n";
    }
  }
  // flush anything left in buffer
  fout << std::flush;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(stack)::value_type>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

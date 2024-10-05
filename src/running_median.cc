/**
 * @file running_median.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Find the Running Median"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/find-the-running-median/problem
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
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
 * Return vector of running medians given an input vector of values.
 *
 * @tparam T Arithmetic type
 *
 * @param values Values to compute running medians for
 */
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
auto running_medians(const std::vector<T>& values)
{
  // vector of running medians
  std::vector<double> medians;
  // sorted vector of the values
  std::vector<T> sorted_values;
  // for each value in values
  for (const auto& value : values) {
    // find place to insert value. since it is sorted, we can use upper_bound
    // for logarithmic instead of linear complexity as with find_if
    sorted_values.insert(
      std::upper_bound(sorted_values.begin(), sorted_values.end(), value),
      value
    );
    // add median to medians (even number of values)
    if (sorted_values.size() % 2 == 0) {
      auto mid = sorted_values.size() / 2;
      medians.push_back(0.5 * (sorted_values[mid - 1] + sorted_values[mid]));
    }
    // add median to medians (odd number of values)
    else
      medians.push_back(sorted_values[sorted_values.size() / 2]);
  }
  return medians;
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
  std::ofstream fout{std::getenv("OUTPUT_PATH")};
  auto& fin = std::cin;
#endif  // !defined(PDHKR_TEST) && !defined(PDHKR_LOCAL)
  // number of inputs
  unsigned int n_values;
  fin >> n_values;
  // read values
  std::vector<int> values(n_values);
  for (decltype(n_values) i = 0; i < n_values; i++)
    fin >> values[i];
  // compute running medians + print
  auto medians = running_medians(values);
  for (auto median : medians)
    fout << std::fixed << std::setprecision(1) << median << "\n";
  fout << std::flush;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(values)::value_type>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

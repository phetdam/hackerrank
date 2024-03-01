/**
 * @file compare.h
 * @author Derek Huang
 * @brief C++ header implementing comparisong functions for HackerRank output
 * @copyright MIT License
 */

#ifndef PDHKR_COMPARE_H_
#define PDHKR_COMPARE_H_

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace pdhkr {

/**
 * Compare the expected and actual results.
 *
 * @tparam T Non-floating type to compare equality for
 *
 * @param expected Vector of expected valeus
 * @param actual Vector of actual values
 * @param fans Input stream containing expected result
 * @param fout Input stream containing actual result
 * @returns `true` if results match, `false` otherwise
 */
template <typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
bool compare(const std::vector<T>& expected, const std::vector<T>& actual)
{
  // width of the output stream when printing line number. this is enough to
  // hold 2 ^ 26 (67,108,864 lines) so usually no file will have more lines
  // than this. this is signed int since std::setw takes a signed int
  static constexpr auto lineno_width = 8;
  // flag to indicate success/failure
  bool test_success = true;
  // lower/upper container sizes to help with size reporting if size conflict
  auto lower_size = std::min(expected.size(), actual.size());
  auto upper_size = std::max(expected.size(), actual.size());
  // check each value
  for (decltype(upper_size) i = 0; i < upper_size; i++) {
    // if i >= lower_size, error about value missing
    if (i >= lower_size) {
      // print depending on who has smaller size
      if (i >= expected.size())
        std::cout << std::setw(lineno_width) << i + 1 <<
          ": ERROR: expected N/A, actual " << actual[i] << std::endl;
      else
        std::cout << std::setw(lineno_width) << i + 1 <<
          ": ERROR: expected " << expected[i] << ", actual N/A" << std::endl;
      test_success = false;
    }
    // no size issue and unequal
    else if (expected[i] != actual[i]) {
      std::cout << std::setw(lineno_width) << i + 1 << ": ERROR: expected " <<
        expected[i] << ", actual " << actual[i] << std::endl;
      test_success = false;
    }
  }
  return test_success;
}

/**
 * Compare the expected and actual results.
 *
 * @tparam T Type to compare equality for
 *
 * @param fans Input stream containing expected result
 * @param fout Input stream containing actual result
 * @returns `true` if results match, `false` otherwise
 */
template <typename T>
bool compare(std::ifstream& fans, std::stringstream& fout)
{
  // expected values
  std::vector<T> expected;
  for (std::string line; std::getline(fans, line); )
    expected.push_back(std::stoll(line));
  // actual values
  decltype(expected) actual;
  for (std::string line; std::getline(fout, line); )
    actual.push_back(std::stoll(line));
  // return result of comparison, writing error messages to stream
  return compare(expected, actual);
}

}  // namespace pdhkr

#endif  // PDHKR_COMPARE_H_

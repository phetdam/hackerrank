/**
 * @file compare.hh
 * @author Derek Huang
 * @brief C++ header implementing comparison functions for HackerRank output
 * @copyright MIT License
 */

#ifndef PDHKR_COMPARE_HH_
#define PDHKR_COMPARE_HH_

#include <algorithm>
#include <cstdlib>
#include <istream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "pdhkr/string.hh"
#include "pdhkr/type_traits.hh"

namespace pdhkr {

/**
 * Field width for the line number field used by the `compare` overloads.
 *
 * Current value is enough to hold 2 ^ 26 (67,108,864) lines which is enough
 * for most files. This is an `int` because `std::setw` takes a signed int.
 */
inline constexpr auto compare_lineno_width = 8;

/**
 * Compare expected values against actual values.
 *
 * Mismatches result in error messages being written to the output stream.
 *
 * @tparam T Non-floating type to compare equality for
 *
 * @param out Output stream to write messages to
 * @param expected Vector of expected values
 * @param actual Vector of actual values
 * @returns `true` if results match, `false` otherwise
 */
template <typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
bool compare(
  std::ostream& out,
  const std::vector<T>& expected,
  const std::vector<T>& actual)
{
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
      // TODO: print "ERROR" in red text
      if (i >= expected.size())
        out << std::setw(compare_lineno_width) << i + 1 <<
          ": ERROR: expected N/A, actual " << actual[i] << std::endl;
      else
        out << std::setw(compare_lineno_width) << i + 1 <<
          ": ERROR: expected " << expected[i] << ", actual N/A" << std::endl;
      test_success = false;
    }
    // no size issue and values equal
    // TODO: print "OK" in green text
    else if (expected[i] == actual[i])
      out << std::setw(compare_lineno_width) << i + 1 << ": OK" << std::endl;
    // no size issue and unequal
    else {
      out << std::setw(compare_lineno_width) << i + 1 << ": ERROR: expected " <<
        expected[i] << ", actual " << actual[i] << std::endl;
      test_success = false;
    }
  }
  return test_success;
}

/**
 * Compare expected values against actual values.
 *
 * Mismatches result in error messages being written to `std::cout`.
 *
 * @tparam T Non-floating type to compare equality for
 *
 * @param expected Vector of expected values
 * @param actual Vector of actual values
 * @returns `true` if results match, `false` otherwise
 */
template <typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
inline bool compare(const std::vector<T>& expected, const std::vector<T>& actual)
{
  return compare(std::cout, expected, actual);
}

/**
 * Compare expected vector values against actual values.
 *
 * Mismatches result in error messages being written to the output stream.
 *
 * @tparam T Non-floating type to compare equality for
 *
 * @param out Output stream to write messages to
 * @param expected Vector of expected values
 * @param actual Vector of actual values
 * @returns `true` if results match, `false` otherwise
 */
template <typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
bool compare(
  std::ostream& out,
  const std::vector<std::vector<T>>& expected,
  const std::vector<std::vector<T>>& actual)
{
  // flag to indicate success/failure
  bool test_success = true;
  // lower/upper container sizes to help with size reporting if size conflict
  auto lower_size = std::min(expected.size(), actual.size());
  auto upper_size = std::max(expected.size(), actual.size());
  // check each vector value
  for (decltype(upper_size) i = 0; i < upper_size; i++) {
    // if i >= lower_size, error about value missing
    if (i >= lower_size) {
      // print depending on who has smaller size
      // TODO: print "ERROR" in red text
      if (i >= expected.size())
        out << std::setw(compare_lineno_width) << i + 1 <<
          ": ERROR: expected N/A, actual " << to_string(actual[i]) << std::endl;
      else
        out << std::setw(compare_lineno_width) << i + 1 <<
          ": ERROR: expected " << to_string(expected[i]) << ", actual N/A" <<
          std::endl;
      test_success = false;
    }
    // no size issue and values equal
    // TODO: print "OK" in green text
    else if (expected[i] == actual[i])
      out << std::setw(compare_lineno_width) << i + 1 << ": OK" << std::endl;
    // no size issue and unequal. here we also break down mismatched values
    else {
      out << std::setw(compare_lineno_width) << i + 1 << ": ERROR: expected " <<
        to_string(expected[i]) << ", actual " << to_string(actual[i]) << std::endl;
      // lower/upper container sizes for size reporting
      auto lower_size_i = std::min(expected[i].size(), actual[i].size());
      auto upper_size_i = std::max(expected[i].size(), actual[i].size());
      // size label. this differs if lower_size_i != upper_size_i
      auto size_label = (lower_size_i == upper_size_i) ?
        std::to_string(upper_size_i) :
        "max(" + std::to_string(expected[i].size()) + ", " +
          std::to_string(actual[i].size()) + ")";
      // check each vector value
      for (decltype(upper_size_i) j = 0; j < upper_size_i; j++) {
        // if j >= lower_size_i, error about value missing
        if (j >= lower_size_i) {
          // print depending on who has smaller size
          if (j >= expected[i].size())
            out << std::setw(compare_lineno_width) << i + 1 << ": " <<
              std::setw(compare_lineno_width) << j + 1 << "/" << size_label <<
              ": expected N/A, actual " << actual[i][j] << std::endl;
          else
            out << std::setw(compare_lineno_width) << i + 1 << ": index " <<
              std::setw(compare_lineno_width) << j + 1 << "/" << size_label <<
              ": expected " << expected[i][j] << ", actual N/A" << std::endl;
        }
        // no size issue and unequal values
        else if (expected[i][j] != actual[i][j])
          out << std::setw(compare_lineno_width) << i + 1 << ": index " <<
            std::setw(compare_lineno_width) << j + 1 << "/" << size_label <<
            ": expected " << expected[i][j] << ", actual " << actual[i][j] <<
            std::endl;
        // no size issue and values equal
      }
      test_success = false;
    }
  }
  return test_success;
}

/**
 * Compare expected vector values against actual values.
 *
 * Mismatches result in error messages being written to `std::cout`.
 *
 * @tparam T Non-floating type to compare equality for
 *
 * @param expected Vector of expected values
 * @param actual Vector of actual values
 * @returns `true` if results match, `false` otherwise
 */
template <typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
inline bool compare(
  const std::vector<std::vector<T>>& expected,
  const std::vector<std::vector<T>>& actual)
{
  return compare(std::cout, expected, actual);
}

/**
 * Convert a string to the specified integral type.
 *
 * If `T` is not one of the standard arithmetic types, a
 * `std::numeric_limits<T>` specialization should be added.
 *
 * @tparam T Integral type
 */
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T to_number(const std::string& str, int base = 0)
{
  // specific integral types
  if constexpr (std::is_same_v<T, int>)
    return std::stoi(str, nullptr, base);
  else if constexpr (std::is_same_v<T, long>)
    return std::stol(str, nullptr, base);
  else if constexpr (std::is_same_v<T, long long>)
    return std::stoll(str, nullptr, base);
  else if constexpr (std::is_same_v<T, unsigned long>)
    return std::stoul(str, nullptr, base);
  else if constexpr (std::is_same_v<T, unsigned long long>)
    return std::stoull(str, nullptr, base);
  // unknown unsigned integral type
  else if constexpr (std::is_unsigned_v<T>) {
    auto val = std::stoull(str, nullptr, base);
    // error if exceeds limits of specified type
    if (val > std::numeric_limits<T>::max())
      throw std::overflow_error{
        "Value above unsigned type maximum " +
        std::to_string(static_cast<decltype(val)>(std::numeric_limits<T>::max()))
      };
    return static_cast<T>(val);
  }
  // unknown, assumed signed integral type
  else {
    auto val = std::stoll(str, nullptr, base);
    // error if exceeds limits of specified type
    if (val < std::numeric_limits<T>::min())
      throw std::overflow_error{
        "Value below signed type minimum " +
        std::to_string(static_cast<decltype(val)>(std::numeric_limits<T>::min()))
      };
    if (val > std::numeric_limits<T>::max())
      throw std::overflow_error{
        "Value above signed type maximum " +
        std::to_string(static_cast<decltype(val)>(std::numeric_limits<T>::max()))
      };
    return static_cast<T>(val);
  }
}

/**
 * Convert a string to the specified floating type.
 *
 * If `T` is not one of the standard arithmetic types, a
 * `std::numeric_limits<T>` specialization should be added.
 *
 * @tparam T Floating type
 */
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
T to_number(const std::string& str)
{
  // specific floating types
  if constexpr (std::is_same_v<T, float>)
    return std::stof(str);
  else if constexpr (std::is_same_v<T, double>)
    return std::stod(str);
  else if constexpr (std::is_same_v<T, long double>)
    return std::stold(str);
  // unknown floating type
  else {
    auto val = std::stold(str);
    // error if exceeds limits of specified type
    if (val < std::numeric_limits<T>::lowest())
      throw std::overflow_error{
        "Value below floating type minimum " +
        std::to_string(static_cast<decltype(val)>(std::numeric_limits<T>::lowest()))
      };
    if (val > std::numeric_limits<T>::max())
      throw std::overflow_error{
        "Value below floating type maximum " +
        std::to_string(static_cast<decltype(val)>(std::numeric_limits<T>::max()))
      };
    return static_cast<T>(val);
  }
}

/**
 * Compare expected values against actual values.
 *
 * Each value must be on its own line which is typical for most HackerRank
 * problems. Mismatches result in messages being written to the output stream.
 *
 * @tparam T Scalar type to compare equality for
 *
 * @param out Output stream to write messages to
 * @param ein Input stream containing expected result
 * @param ain Input stream containing actual result
 * @returns `true` if results match, `false` otherwise
 */
template <typename T>
bool scalar_compare(std::ostream& out, std::istream& ein, std::istream& ain)
{
  // expected values
  std::vector<T> expected;
  for (std::string line; std::getline(ein, line); )
    expected.push_back(to_number<T>(line));
  // actual values
  decltype(expected) actual;
  for (std::string line; std::getline(ain, line); )
    actual.push_back(to_number<T>(line));
  // return result of comparison, writing error messages to stream
  return compare(out, expected, actual);
}

/**
 * Compare expected vector values against actual values.
 *
 * Each vector value must be on its own line, consisting of space-delimited
 * values, which is typical for most HackerRank problems. Mismatches result in
 * messages being written to the output stream.
 *
 * @note This function can obvsiouly scale poorly if the results are large.
 *
 * @tparam T Vector value type to compare equality for
 *
 * @param out Output stream to write messages to
 * @param ein Input stream containing expected result
 * @param ain Input stream containing actual result
 * @returns `true` if results match, `false` otherwise
 */
template <typename T>
bool vector_compare(std::ostream& out, std::istream& ein, std::istream& ain)
{
  // lambda for reading vector inputs from stream into vector of vectors
  auto read_values = [](std::istream& in)
  {
    std::vector<std::vector<T>> values;
    for (std::string line; std::getline(in, line); ) {
      // line goes into stream that we read from. can move from string in C++20
      std::stringstream ss{line};
      values.push_back({});
      // read until EOF
      T value;
      while (!ss.eof()) {
        ss >> value;
        values.back().push_back(value);
      }
    }
    return values;
  };
  // return result of comparison, writing error messages to stream
  return compare(out, read_values(ein), read_values(ain));
}

/**
 * Compare expected values against actual values.
 *
 * Mismatches result in messages being written to the output stream.
 *
 * @tparam T Scalar/object type or `std::vector<U>` to compare equality for
 *
 * @param out Output stream to write messages to
 * @param ein Input stream containing expected result
 * @param ain Input stream containing actual result
 * @returns `true` if results match, `false` otherwise
 */
template <typename T>
bool compare(std::ostream& out, std::istream& ein, std::istream& ain)
{
  // if type is vector, use vector_compare
  // note: this doesn't work if the vector is not using the default allocator
  if constexpr (is_std_vector_v<T>)
    return vector_compare<typename T::value_type>(out, ein, ain);
  // else use scalar_compare
  else
    return scalar_compare<T>(out, ein, ain);
}

/**
 * Compare expected values against actual values.
 *
 * Each value must be on its own line which is typical for most HackerRank
 * problems. Mismatches result in messages being written to `std::cout`.
 *
 * @tparam T Scalar/object type or `std::vector<U>` to compare equality for
 *
 * @param ein Input stream containing expected result
 * @param ain Input stream containing actual result
 * @returns `true` if results match, `false` otherwise
 */
template <typename T>
inline bool compare(std::istream& ein, std::istream& ain)
{
  return compare<T>(std::cout, ein, ain);
}

/**
 * Compare expected values against actual values.
 *
 * Each value must be on its own line which is typical for most HackerRank
 * problems. Mismatches result in error messages being written to the output
 * stream. The return value of this function can be returned from `main`.
 *
 * @tparam T Scalar/object type or `std::vector<U>` to compare equality for
 *
 * @param out Output stream to write messages to
 * @param ein Input stream containing expected result
 * @param ain Input stream containing actual result
 * @returns `EXIT_SUCCESS` if results match, `EXIT_FAILURE` otherwise
 */
template <typename T>
inline int exit_compare(std::ostream& out, std::istream& ein, std::istream& ain)
{
  return (compare<T>(out, ein, ain)) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/**
 * Compare expected values against actual values.
 *
 * Each value must be on its own line which is typical for most HackerRank
 * problems. Mismatches result in error messages being written to `std::cout`.
 * The return value of this function can be returned from `main`.
 *
 * @tparam T Scalar/object type or `std::vector<U>` to compare equality for
 *
 * @param ein Input stream containing expected result
 * @param ain Input stream containing actual result
 * @returns `EXIT_SUCCESS` if results match, `EXIT_FAILURE` otherwise
 */
template <typename T>
inline auto exit_compare(std::istream& ein, std::istream& ain)
{
  return exit_compare<T>(std::cout, ein, ain);
}

}  // namespace pdhkr

#endif  // PDHKR_COMPARE_HH_

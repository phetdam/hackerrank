/**
 * @file string.hh
 * @author Derek Huang
 * @brief C++ header for string helpers and formatters
 * @copyright MIT License
 */

#ifndef PDHKR_STRING_HH_
#define PDHKR_STRING_HH_

#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace pdhkr {

/**
 * Return a string representation for a container.
 *
 * @tparam Container Container with value type for which `operator<<` exists
 *
 * @param values Container of values
 * @param l_delim Left delimiter string for the values
 * @param delim Delimiter string separating each value
 * @param r_delim Right delimiter string for the values
 */
template <typename Container>
auto to_string(
  const Container& values,
  const std::string_view& l_delim,
  const std::string_view& delim,
  const std::string_view& r_delim)
{
  std::stringstream ss;
  ss << l_delim;
  for (auto it = values.begin(); it != values.end(); it++) {
    if (std::distance(values.begin(), it))
      ss << delim;
    ss << *it;
  }
  ss << r_delim;
  return ss.str();
}

/**
 * Return a string representation for a container.
 *
 * The delimiter separating each value is `", "` in this case.
 *
 * @tparam Container Container with value type for which `operator<<` exists
 *
 * @param values Container of values
 * @param l_delim Left delimiter string for the values
 * @param r_delim Right delimiter string for the values
 */
template <typename Container>
inline auto to_string(
  const Container& values,
  const std::string_view& l_delim,
  const std::string_view& r_delim)
{
  return to_string(values, l_delim, ", ", r_delim);
}

/**
 * Return a string representation for a vector.
 *
 * The left and right delimiters are `"["` and `"]"` respectively.
 *
 * @tparam T Type for which `operator<<` exists
 * @tparam Allocator Vector allocator
 *
 * @param values Vector of values
 * @param delim Delimiter string separating each value, `", "` by default
 */
template <typename T, typename Allocator>
inline auto to_string(
  const std::vector<T, Allocator>& values, const std::string_view& delim = ", ")
{
  return to_string(values, "[", delim, "]");
}

}  // namespace pdhkr

#endif  // PDHKR_STRING_HH_

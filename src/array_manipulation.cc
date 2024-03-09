/**
 * @file array_manipulation.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Array Manipulation"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/crush/problem
 *
 * @note This naive solution times out on the larger test cases. We can sort
 *  the affected uniform intervals in O(nlog(n)) but since we need to track the
 *  values updating is not linear and falls back into worst-case O(n^2).
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <fstream>
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

// don't use naive array updating solution
#ifndef USE_ARRAY_UPDATE
namespace {

/**
 * Class representing an interval with a uniform value over the interval.
 */
class uniform_interval {
public:
  /**
   * Ctor.
   *
   * Construct an interval by reading 3 unsigned values from an input stream.
   *
   * @param in Input stream to read left limit, right limit, and value from
   */
  uniform_interval(std::istream& in)
  {
    in >> left_;
    in >> right_;
    in >> value_;  // will never exceed 1e9
  }

  /**
   * Ctor.
   *
   * @param left Left interval limit
   * @param right Right interval limit
   * @param value Interval value
   */
  uniform_interval(
    unsigned int left, unsigned int right, unsigned long long value = 0)
    : left_{left}, right_{right}, value_{value}
  {
    if (right_ < left_)
      throw std::runtime_error{"Right interval limit precedes left limit"};
  }

  /**
   * Return left interval limit.
   */
  auto left() const noexcept
  {
    return left_;
  }

  /**
   * Return right interval limit.
   */
  auto right() const noexcept
  {
    return right_;
  }

  /**
   * Return interval value.
   */
  auto value() const noexcept
  {
    return value_;
  }

private:
  unsigned int left_;
  unsigned int right_;
  // we need long long (64 bits) type since problem's max value can be 2e14
  unsigned long long value_;
};

/**
 * Class representing an interval with non-uniform values over the interval.
 */
class multivalued_interval {
public:
  /**
   * Ctor.
   *
   * @param interval Uniform interval to construct from
   */
  multivalued_interval(const uniform_interval& interval)
    : left_{interval.left()},
      right_{interval.right()},
      values_(right_ - left_ + 1, interval.value())
  {}

  /**
   * Return left interval limit.
   */
  auto left() const noexcept
  {
    return left_;
  }

  /**
   * Return right interval limit.
   */
  auto right() const noexcept
  {
    return right_;
  }

  /**
   * Return interval values.
   */
  const auto& values() const noexcept
  {
    return values_;
  }

  /**
   * Return iterator to the leftmost value of the interval.
   */
  auto begin() const noexcept
  {
    return values_.begin();
  }

  /**
   * Return iterator to one past the rightmost value of the interval.
   */
  auto end() const noexcept
  {
    return values_.end();
  }

  /**
   * Merge in a uniform interval overlapping the right interval limit.
   *
   * The given interval's left limit must be between `left()` and `right()`.
   *
   * @param interval Uniform interval to merge
   */
  void right_merge(const uniform_interval& interval)
  {
    // check that left endpoint is contained
    if (interval.left() < left_ || interval.left() > right_)
      throw std::runtime_error{"Given interval cannot be used for right merge"};
    // number of values to modify
    auto n_modify = std::min(right_, interval.right()) - interval.left() + 1;
    // modify overlapping range
    for (decltype(n_modify) i = 0; i < n_modify; i++)
      values_[i + interval.left() - left_] += interval.value();
    // append non-overlapping if any
    for (
      decltype(n_modify) i = 0;
      i < interval.right() - interval.left() - n_modify + 1;
      i++
    )
      values_.push_back(interval.value());
    // update right_ if necessary
    right_ = std::max(right_, interval.right());
  }

private:
  unsigned int left_;
  unsigned int right_;
  std::vector<unsigned long long> values_;
};

/**
 * Merge a vector of valued interval objects into a set of disjoint intervals.
 *
 * The disjoint intervals are `multivalued_intervals`.
 *
 * @param intervals Vector of valued intervals to merge
 */
auto merge_uniform_intervals(std::vector<uniform_interval> intervals)
{
  // unnecessary for this problem but should be added
  if (intervals.empty())
    return std::vector<multivalued_interval>{};
  // sort by left endpoint
  std::sort(
    intervals.begin(),
    intervals.end(),
    [](const auto& a, const auto& b) { return a.left() < b.left(); }
  );
  // merged intervals
  std::vector<multivalued_interval> merged_intervals{intervals.front()};
  // loop through sorted intervals
  for (decltype(intervals.size()) i = 1; i < intervals.size(); i++) {
    // last merged and current uniform interval to consider
    auto& last_merged = merged_intervals.back();
    const auto& interval = intervals[i];
    // overlap means we need to merge
    if (last_merged.right() >= interval.left())
      last_merged.right_merge(interval);
    // no overlap so add as new interval
    else
      merged_intervals.emplace_back(interval);
  }
  return merged_intervals;
}

}  // namespace
#endif  // USE_ARRAY_UPDATE

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
// use naive array updating solution
#if defined(USE_ARRAY_UPDATE)
  // create array and perform operations. we need long long (64 bits) type
  // since based on the problem constraints, max array value can be 2e14
  std::vector<unsigned long long> values(array_size, 0UL);
#else
  std::vector<uniform_interval> intervals;
  intervals.reserve(array_size);
#endif  // !defined(USE_ARRAY_UPDATE)
  for (decltype(n_ops) i = 0; i < n_ops; i++) {
#if defined(USE_ARRAY_UPDATE)
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
#else
    intervals.emplace_back(fin);
#endif  // !defined(USE_ARRAY_UPDATE)
  }
  // write result
#if defined(USE_ARRAY_UPDATE)
  fout << *std::max_element(values.begin(), values.end()) << std::endl;
#else
  auto merged_intervals = merge_uniform_intervals(std::move(intervals));
  auto max_it = std::max_element(
    merged_intervals.begin(),
    merged_intervals.end(),
    [](const auto& a, const auto& b)
    {
      auto max_a = *std::max_element(a.begin(), a.end());
      auto max_b = *std::max_element(b.begin(), b.end());
      return max_a < max_b;
    }
  );
  fout << *std::max_element(max_it->begin(), max_it->end()) << std::endl;
#endif  // !defined(USE_ARRAY_UPDATE)
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
#if defined(USE_ARRAY_UPDATE)
  return pdhkr::exit_compare<decltype(values)::value_type>(fans, fout);
#else
  // or std::remove_const_t<std::remove_reference_t<...>>
  using value_type = std::decay_t<decltype(max_it->values())>::value_type;
  return pdhkr::exit_compare<value_type>(fans, fout);
#endif  // !defined(USE_ARRAY_UPDATE)
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
  return EXIT_SUCCESS;
}

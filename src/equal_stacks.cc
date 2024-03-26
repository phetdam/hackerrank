/**
 * @file equal_stack.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Equal Stacks"
 * @copyright MIT License
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

namespace {

/**
 * Return maximum equalized height of the three "stacks" after element removal.
 *
 * Elements are treated as being removed from the front of the vectors, i.e.
 * from the "top" of the "stack". Nothing is actually physically removed and
 * instead we track the sum of the stack values, incrementally removing until
 * we are able to equalize the heights or until a stack in empty (zero).
 *
 * @param stack_1 First "stack"
 * @param stack_2 Second "stack"
 * @param stack_3 Third "stack"
 */
auto max_equal_height(
  const std::vector<unsigned int>& stack_1,
  const std::vector<unsigned int>& stack_2,
  const std::vector<unsigned int>& stack_3)
{
  // if any is empty, zero
  if (stack_1.empty() || stack_2.empty() || stack_3.empty())
    return 0U;
  // compute totals for each "stack"
  auto total_1 = std::accumulate(stack_1.begin(), stack_1.end(), 0U);
  auto total_2 = std::accumulate(stack_2.begin(), stack_2.end(), 0U);
  auto total_3 = std::accumulate(stack_3.begin(), stack_3.end(), 0U);
  // iterators tracking the "top" of each "stack"
  auto top_it_1 = stack_1.begin();
  auto top_it_2 = stack_2.begin();
  auto top_it_3 = stack_3.begin();
  // while the totals are unequal
  while (total_1 != total_2 || total_2 != total_3) {
    // if a stack runs empty, must return zero
    if (
      top_it_1 == stack_1.end() ||
      top_it_2 == stack_2.end() ||
      top_it_3 == stack_3.end()
    )
      return 0U;
    // compute total of totals and floor divide by 3 to estimate max height.
    // no need to worry about addition overflow given the problem limits
    auto est_max = (total_1 + total_2 + total_3) / 3U;
    // if a total exceeds est_max, remove an element and adjust iterator
    if (total_1 > est_max)
      total_1 -= *top_it_1++;
    if (total_2 > est_max)
      total_2 -= *top_it_2++;
    if (total_3 > est_max)
      total_3 -= *top_it_3++;
  }
  // all totals same when done (first picked arbitrarily)
  return total_1;
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
  // read number of items in each stack
  unsigned int n_1, n_2, n_3;
  fin >> n_1;
  fin >> n_2;
  fin >> n_3;
  // read values into "stacks". we use vectors here to hold everything since we
  // won't even need to physically remove elements from them
  std::vector<unsigned int> stack_1(n_1), stack_2(n_2), stack_3(n_3);
  for (decltype(n_1) i = 0; i < n_1; i++)
    fin >> stack_1[i];
  for (decltype(n_2) i = 0; i < n_2; i++)
    fin >> stack_2[i];
  for (decltype(n_3) i = 0; i < n_3; i++)
    fin >> stack_3[i];
  // write max height of equalized stacks
  fout << max_equal_height(stack_1, stack_2, stack_3) << std::endl;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(max_equal_height({}, {}, {}))>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

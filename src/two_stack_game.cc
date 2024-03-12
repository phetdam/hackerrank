/**
 * @file two_stack_game.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Game of Two Stacks"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/game-of-two-stacks/problem
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

namespace {

/**
 * Return max number of values removed from stacks for the given sum limit.
 *
 * We determine this value by first removing as many values as possible from
 * one stack and then remove as many values from the other stack for each value
 * we put back from the first stack. The max total removed is our answer.
 *
 * @param stack_a First value stack where the first index is the head
 * @param stack_b Second value stack where the second index is the head
 * @param max_sum Maximum sum of the removed values
 */
auto max_game_score(
  const std::vector<unsigned int>& stack_a,
  const std::vector<unsigned int>& stack_b,
  unsigned int max_sum)
{
  // sums of values removed if only stacks A and B are individually considered
  unsigned int a_sum = 0;
  decltype(a_sum) b_sum = 0;
  // number of values removed from each stack
  std::size_t rem_a = 0;
  decltype(rem_a) rem_b = 0;
  // advance a_sum until max_sum is exceeded or elements are exhausted
  while (a_sum <= max_sum && rem_a < stack_a.size())
    a_sum += stack_a[rem_a++];
  // if we exceeded max_sum, retreat
  if (a_sum > max_sum)
    a_sum -= stack_a[--rem_a];
  // max removed values + number of values removed if only A was used
  decltype(rem_a) max_rem = rem_a;
  decltype(rem_a) max_rem_a = rem_a;
  // for each element of A
  for (decltype(rem_a) i = 0; i < max_rem_a; i++) {
    // increment as many elements of B as possible
    while (a_sum + b_sum <= max_sum && rem_b < stack_b.size())
      b_sum += stack_b[rem_b++];
    // if we exceed max_sum, retreat
    if (a_sum + b_sum > max_sum)
      b_sum -= stack_b[--rem_b];
    // take max with max_rem + update a_sum and number of removed A values
    max_rem = std::max(max_rem, rem_a + rem_b);
    a_sum -= stack_a[--rem_a];
  }
  // advance b_sum until max_sum is exceeded or elements are exhausted
  while (a_sum + b_sum <= max_sum && rem_b < stack_b.size())
    b_sum += stack_b[rem_b++];
  // if we exceeded max_sum, retreat (no need to adjust b_sum here actually)
  if (a_sum + b_sum > max_sum)
    b_sum -= stack_b[--rem_b];
  // take final max
  return std::max(max_rem, rem_a + rem_b);
  // max elements removed
  //
  /*
  // number of removed values, total sum of values
  unsigned int n_removed = 0;
  unsigned int cur_sum = 0;
  // iterators to first elements of stack A and B
  auto a_it = stack_a.begin();
  auto b_it = stack_b.begin();
  // while max_sum has not been exceed and stacks are non-empty
  while (cur_sum <= max_sum && a_it != stack_a.end() && b_it != stack_b.end()) {
    // remove stack A top element if it is smaller
    if (*a_it <= *b_it) {
      cur_sum += *a_it;
      a_it++;
      n_removed++;
    }
    // remove stack B top element otherwise
    else {
      cur_sum += *b_it;
      b_it++;
      n_removed++;
    }
  }
  // remove remaining from stack_a if possible
  while (cur_sum <= max_sum && a_it != stack_a.end()) {
    cur_sum += *a_it;
    a_it++;
    n_removed++;
  }
  // remove remainng from stack_b if possible
  while (cur_sum <= max_sum && b_it != stack_b.end()) {
    cur_sum += *b_it;
    b_it++;
    n_removed++;
  }
  // if cur_sum exceeds max_sum, need to adjust down by 1
  return (cur_sum > max_sum) ? n_removed - 1 : n_removed;
  */
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
  // number of games
  unsigned short n_games;
  fin >> n_games;
  // read input for each game
  for (decltype(n_games) i = 0; i < n_games; i++) {
    // stack A, stack B starting sizes
    unsigned int n_values_a;
    unsigned int n_values_b;
    fin >> n_values_a;
    fin >> n_values_b;
    // max sum of removed values
    unsigned int max_sum;
    fin >> max_sum;
    // read values for stack A
    std::vector<unsigned int> stack_a(n_values_a);
    for (decltype(n_values_a) j = 0; j < n_values_a; j++)
      fin >> stack_a[j];
    // read values for stack B
    decltype(stack_a) stack_b(n_values_b);
    for (decltype(n_values_b) j = 0; j < n_values_b; j++)
      fin >> stack_b[j];
    // write result to output stream
    fout << max_game_score(stack_a, stack_b, max_sum) << "\n";
  }
  // final flush for the stream
  fout << std::flush;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(max_game_score({}, {}, 0))>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

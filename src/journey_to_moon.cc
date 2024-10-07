/**
 * @file journey_to_moon.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Journey to the Moon"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/journey-to-the-moon/problem
 */

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// only used when compiling as standalone test program
#ifdef PDHKR_TEST
#include <sstream>

#include "pdhkr/compare.hh"
#include "pdhkr/testing.hh"
#endif  // PDHKR_TEST

/**
 * Adjacency list class.
 *
 * We have constant lookup for both the start and end nodes and can quickly
 * iterate through the desired neighbors of a particular node.
 *
 * This is much faster than an adjacency matrix since finding neighbors is no
 * longer fully linear in the number of nodes.
 *
 * @note This is modified from `roads_and_libraries.cc` as it fits the problem.
 */
template <typename T>
class adjacency_list {
public:
  using integral_type = T;

  /**
   * Insert a directed edge into the adjacency list.
   *
   * If the edge already exists, nothing is done.
   *
   * @param start ID of start node
   * @param end ID of end node
   */
  void insert(T start, T end)
  {
    // create map if it does not exist
    if (edges_.find(start) == edges_.end())
      edges_[start] = {};
    // insert end into neighbor set
    if (edges_[start].find(end) == edges_[start].end())
      edges_[start].insert(end);
  }

  /**
   * Check if the directed edge from `start` to `end` exists.
   *
   * @param start ID of start node
   * @param end ID of end node
   */
  bool contains(T start, T end) const
  {
    // get iterator to the list of nodes start is connected to
    auto nodes_it = edges_.find(start);
    // start has no edges, no edge
    if (nodes_it == edges_.end())
      return false;
    // check if end is in nodes_it->second
    return nodes_it->second.find(end) != nodes_it->second.end();
  }

  /**
   * Look up neighbor set for the specified node.
   *
   * If no neighbor set exists then an empty one is returned.
   */
  const auto& neighbors(T start) const
  {
    auto it = edges_.find(start);
    // couldn't find, return reference to static
    if (it == edges_.end()) {
      static typename decltype(edges_)::mapped_type empty;
      return empty;
    }
    return it->second;
  }

  /**
   * Iterator to first element in map.
   */
  auto begin() noexcept
  {
    return edges_.begin();
  }

  /**
   * Iterator to one-past-the end of map elements.
   */
  auto end() noexcept
  {
    return edges_.end();
  }

  /**
   * Const iterator to first element in map.
   */
  auto begin() const noexcept
  {
    return edges_.begin();
  }

  /**
   * Const iterator to one-past-the end of map elements.
   */
  auto end() const noexcept
  {
    return edges_.end();
  }

private:
  std::unordered_map<T, std::unordered_set<T>> edges_;
};

/**
 * Convenience type alias for the vector of pairs representing edges.
 */
using edge_vector = std::vector<std::pair<unsigned int, unsigned int>>;

// part of HackerRank template code
////////////////////////////////////////////////////////////////////////////////
/*
 * Complete the 'journeyToMoon' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY astronaut
 */
////////////////////////////////////////////////////////////////////////////////
// renamed from journeyToMoon, return type auto (unsigned long long), type of n
// now unsigned int, astronaut parameter renamed to a_pairs with type changed
// to vector of unsigned int pairs. we need a long long return type as for test
// case 11, the answer overflows 32 bits, and long is still 32 bits when
// compiling for x86, e.g. when building PE32 binaries on Windows
auto journey_to_moon(unsigned int n, const edge_vector& a_pairs)
{
  // adjacency list to hold connection graph between astronauts
  adjacency_list<decltype(n)> edges;
  // insert each astronaut pair (edge) as undirected edge
  for (const auto& a_pair : a_pairs) {
    edges.insert(a_pair.first, a_pair.second);
    edges.insert(a_pair.second, a_pair.first);
  }
  // set of visited nodes (astronauts)
  std::unordered_set<decltype(n)> visited;
  // vector where index is country, value is astronauts from said country
  std::vector<decltype(n)> countries;
  // perform BFS for each unvisited node to fill in countries
  for (const auto& [node, neighbors] : edges) {
    // skip if visited
    if (visited.find(node) != visited.end())
      continue;
    // otherwise, allocate new country
    countries.push_back(0);
    // perform BFS
    std::deque<decltype(n)> queue{node};
    while (queue.size()) {
      // get + pop current
      auto cur = queue.front();
      queue.pop_front();
      // only mark as visited and increment country count if unvisited
      if (visited.find(cur) == visited.end()) {
        visited.insert(cur);
        countries.back() += 1;
      }
      // insert unvisited neighbors if edge exists
      for (const auto neighbor : edges.neighbors(cur))
        if (visited.find(neighbor) == visited.end())
          queue.push_back(neighbor);
    }
  }
  // compute sum of values in countries (number of astronauts). the difference
  // with size of countries indicates the new countries with 1 astronaut to add
  auto n_countries = std::accumulate(countries.begin(), countries.end(), 0U);
  if (n_countries < n) {
    for (auto i = n_countries; i < n; i++)
      countries.push_back(1);
  }
  // number of pairs we can select
  unsigned long long n_pairs = 0;
  // do double loop through countries to compute total number of pairs
  //
  // note:
  //
  // with -O0 on WSL1 Ubuntu 22.04 there are a few specific optimizations that
  // are made because the unoptimized runtime is dominated by calls to size()
  // and operator[]. this may be due to function call overhead and/or repeated
  // memory accesses made to retrieve the results of said calls.
  //
  // there are 3 key optimizations that were made to greatly reduce runtime:
  //
  // 1. computing countries.size() once
  // 2. computing countries[i] once per outer loop
  // 3. indexing directly into countries.data() in the inner loop
  // 4. improve cache locality by using raw pointer as iterator
  //
  // here we can show the time(1) results of various optimizations made using
  // feedback from valgrind --tool=callgrind ./build/journey_to_moon_11:
  //
  // real    0m43.421s (original)
  // real    0m31.547s (with 1)
  // real    0m18.229s (with 1, 2)
  // real    0m12.483s (with 1, 2, 3)
  // real    0m10.109s (with 1, 2, 3, 4)
  //
  // it is pretty clear that 2 and 3 have the greatest impact as by computing
  // values to a temporary, reducing memory lookups, and reducing the function
  // call overhead that is present in operator[] we have some big wins.
  //
  // however, it is important to note that with optimization turned on, the
  // compiler can do this (and so much more) as the optimized runtime is only
  // around 1 second. here we show the time(1) with -O2 compilation:
  //
  // real    0m1.057s (original)
  // real    0m1.065s (with 1, 2, 3, 4)
  //
  // since the original code is more idiomatic, it is possible that the
  // compiler's program analysis is easier and so it knows how to better
  // optimize, e.g. with register allocation, constant propagation.
  //
  // as an addendum, note that these optimizations also improved runtime when
  // compiling using the Visual Studio 2022 C++ compiler. journey_to_moon_11
  // used to run in around 15 or so seconds but with these changes, the runtime
  // when compiling without optimization has dropped to around 5 seconds.
  //
// optimized code uses the original "vanilla" code
#if defined(NDEBUG)
  for (decltype(countries.size()) i = 0; i < countries.size() - 1; i++)
    for (decltype(i) j = i + 1; j < countries.size(); j++)
      n_pairs += countries[i] * countries[j];
// unoptimized code uses mentioned optimizations to cut down runtime
#else
  // key optimization 1: writing size() to a temporary
  auto countries_size = countries.size();
  // raw data pointer + raw "end" iterator
  auto c_data = countries.data();
  auto c_data_end = c_data + countries_size;
  // computes total number of pairs
  for (decltype(countries_size) i = 0; i < countries_size - 1; i++) {
    // key optimization 2: writing countries[i] to a temporary
    // note: since we already have c_data we also use to index in outer loop
    auto c_i = c_data[i];
    // key optimization 4: improve cache locality by using raw pointer iterator
    for (auto it_j = c_data + i + 1; it_j < c_data_end; it_j++)
      n_pairs += c_i * *it_j;
  }
#endif  // !defined(NDEBUG)
  return n_pairs;
}

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
  // number of astronauts
  unsigned int n_astronauts;
  fin >> n_astronauts;
  // number of astronaut pairs (edges)
  unsigned int n_pairs;
  fin >> n_pairs;
  // edge vector
  edge_vector edges;
  edges.reserve(n_pairs);
  // read edge and insert
  for (decltype(n_pairs) i = 0; i < n_pairs; i++) {
    decltype(edges)::value_type edge;
    fin >> edge.first;
    fin >> edge.second;
    edges.push_back(std::move(edge));
  }
  // write result to output stream
  fout << journey_to_moon(n_astronauts, edges) << std::endl;
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<decltype(journey_to_moon(0, {}))>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

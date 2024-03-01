/**
 * @file journey_to_moon.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Journey to the Moon"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/journey-to-the-moon/problem
 */

// part of HackerRank template code
////////////////////////////////////////////////////////////////////////////////
// #include <bits/stdc++.h>
////////////////////////////////////////////////////////////////////////////////

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
#include <vector>

// part of HackerRank template code
////////////////////////////////////////////////////////////////////////////////
using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);
////////////////////////////////////////////////////////////////////////////////

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
// now unsigned int, and astronaut parameter renamed to a_pairs. we need a long
// long return type as for test case 11, the answer overflows 32 bits, and long
// is still 32 bits when compiling for x86, e.g. PE32 on Windows
auto journey_to_moon(unsigned int n, const std::vector<std::vector<int>>& a_pairs)
{
  // adjacency list to hold connection graph between astronauts
  adjacency_list<unsigned int> edges;
  // insert each astronaut pair (edge) as undirected edge
  for (const auto& a_pair : a_pairs) {
    edges.insert(a_pair[0], a_pair[1]);
    edges.insert(a_pair[1], a_pair[0]);
  }
  // set of visited nodes (astronauts)
  std::unordered_set<unsigned int> visited;
  // vector where index is country, value is astronauts from said country
  std::vector<unsigned int> countries;
  // perform BFS for each unvisited node to fill in countries
  for (const auto& [node, neighbors] : edges) {
    // skip if visited
    if (visited.find(node) != visited.end())
      continue;
    // otherwise, allocate new country
    countries.push_back(0);
    // perform BFS
    std::deque<unsigned int> queue{node};
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
  unsigned long n_pairs = 0;
  // do double loop through countries to compute total number of pairs
  for (decltype(countries.size()) i = 0; i < countries.size() - 1; i++)
    for (decltype(i) j = i + 1; j < countries.size(); j++)
      n_pairs += countries[i] * countries[j];
  return n_pairs;
}

int main()
{
#if defined(PDHKR_LOCAL_BUILD)
  // for local run, this is simply std::cout
  auto& fout = std::cout;
#else
  // as-is from HackerRank. this is unsafe
    ofstream fout(getenv("OUTPUT_PATH"));
#endif  // !defined(PDHKR_LOCAL_BUILD)

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int p = stoi(first_multiple_input[1]);

    vector<vector<int>> astronaut(p);

    for (int i = 0; i < p; i++) {
        astronaut[i].resize(2);

        string astronaut_row_temp_temp;
        getline(cin, astronaut_row_temp_temp);

        vector<string> astronaut_row_temp = split(rtrim(astronaut_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int astronaut_row_item = stoi(astronaut_row_temp[j]);

            astronaut[i][j] = astronaut_row_item;
        }
    }

    // use auto instead of requiring int result. turns out that for test case
    // 11 int will overflow since the answer exceeds 32 bits in representation
    /*int*/ auto result = journey_to_moon(n, astronaut);

    fout << result << "\n";
// can't close std::cout since it is not a std::ofstream
#ifndef PDHKR_LOCAL_BUILD
    fout.close();
#endif  // PDHKR_LOCAL_BUILD

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}

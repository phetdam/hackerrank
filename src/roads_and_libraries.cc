/**
 * @file roads_and_libraries.cc
 * @author Derek Huang
 * @brief C++ submission for HackerRank problem "Roads and Libraries"
 * @copyright MIT License
 *
 * URL: https://www.hackerrank.com/challenges/torque-and-development/problem
 *
 * @note Not sure why the URL doesn't match the problem title.
 */

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
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
 */
class adjacency_list {
public:
  /**
   * Insert a directed edge into the adjacency list.
   *
   * If the edge already exists, nothing is done.
   *
   * @param start ID of start node
   * @param end ID of end node
   */
  void insert(int start, int end)
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
  bool contains(int start, int end) const
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
  const auto& neighbors(int start) const
  {
    auto it = edges_.find(start);
    // couldn't find, return reference to static
    if (it == edges_.end()) {
      static decltype(edges_)::mapped_type empty;
      return empty;
    }
    return it->second;
  }

private:
  std::unordered_map<int, std::unordered_set<int>> edges_;
};

// part of HackerRank template code
////////////////////////////////////////////////////////////////////////////////
/*
 * Complete the 'roadsAndLibraries' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER c_lib
 *  3. INTEGER c_road
 *  4. 2D_INTEGER_ARRAY cities
 */
////////////////////////////////////////////////////////////////////////////////
// renamed from roadsAndLibraries. use long long to prevent overflow for x86
// and cities type is just vector of pairs instead of vector of vector size 2
long long roads_and_libraries(
  int n, int c_lib, int c_road, const std::vector<std::pair<int, int>>& cities)
{
  // set for unvisited nodes
  std::unordered_set<int> unvisited;
  for (auto i = 0; i < n; i++)
    unvisited.insert(i);
  // adjacency list of edges (indexed from 0). fill cities as undirected edges
  adjacency_list edges;
  for (const auto& edge : cities) {
    edges.insert(edge.first - 1, edge.second - 1);
    edges.insert(edge.second - 1, edge.first - 1);
  }
  // total cost
  long long total = 0;
  // current set of connected nodes. this is used to detect whether or not a
  // new node we are searching from is disconnected from the others
  std::unordered_set<int> connected;
  // deque of nodes to search through (start from 0)
  std::deque<int> queue{0};
  // until every city has been connected
  while (queue.size() || unvisited.size()) {
    // starting node for BFS. since road costs are the same (c_road), it
    // doesn't matter which node we choose to use (same priority)
    auto root = [&queue, &unvisited]
    {
      // queue is non-empty
      if (queue.size()) {
        auto head = queue.front();
        queue.pop_front();
        return head;
      }
      // otherwise, need to grab something from the unvisited set
      return *unvisited.begin();
    }();
    // mark as visited
    unvisited.erase(root);
    // if not connected we need to clear connected + build a library
    if (connected.find(root) == connected.end()) {
      // clear only if not empty
      if (connected.size())
        connected.clear();
      // build library + mark root as connected
      total += c_lib;
      connected.insert(root);
    }
    // for each neighbor (can be empty set)
    for (const auto node : edges.neighbors(root)) {
      // undirected edge has a connection + not visited + not connected
      if (
        unvisited.find(node) != unvisited.end() &&
        connected.find(node) == connected.end() &&
        (edges.contains(root, node) || edges.contains(node, root))
      ) {
        // increment cost by c_road (build road) + add to connected
        total += c_road;
        connected.insert(node);
        // add nodes to search through
        queue.push_back(node);
      }
    }
  }
  // return total cost of one library + minimum spanning forest vs. all
  // libraries without building any roads at all. note that cast has to be done
  // before multiplying by n to ensure correct widening
  return std::min(static_cast<decltype(total)>(c_lib) * n, total);
}

// note: used to be mostly HackerRank template code but i rewrote it
int main()
{
#if defined(PDHKR_LOCAL_BUILD)
// building as standalone test program
#if defined(PDHKR_TEST)
  // write to stringstream, read from PDHKR_TEST_INPUT
  std::stringstream fout;
  std::ifstream fin{PDHKR_TEST_INPUT};
  // fans provides the expected output
  std::ifstream fans{PDHKR_TEST_OUTPUT};
#else
  // for local run, this is simply std::cout
  auto& fout = std::cout;
  auto& fin = std::cin;
#endif  // !defined(PDHKR_TEST)
#else
  // as-is from HackerRank but with std:: prefix. this is unsafe
  std::ofstream fout(getenv("OUTPUT_PATH"));
  // input stream is std::cin
  auto &fin = std::cin;
#endif  // !defined(PDHKR_LOCAL_BUILD)
  // number of queries
  unsigned int n_queries;
  fin >> n_queries;
  // handle queries
  for (decltype(n_queries) i = 0; i < n_queries; i++) {
    // number of cities
    unsigned int n_cities;
    fin >> n_cities;
    // number of edges connecting cities
    unsigned int n_edges;
    fin >> n_edges;
    // library cost
    unsigned int lib_cost;
    fin >> lib_cost;
    // road cost
    unsigned int road_cost;
    fin >> road_cost;
    // edge vector for this query
    std::vector<std::pair<int, int>> edges;
    edges.reserve(n_edges);
    // read edge and insert
    for (decltype(n_edges) j = 0; j < n_edges; j++) {
      decltype(edges)::value_type edge;
      fin >> edge.first;
      fin >> edge.second;
      // actually, moving is just a copy since pair has fundamental types
      edges.push_back(std::move(edge));
    }
    // write result to output stream with newline (no flush)
    fout << roads_and_libraries(n_cities, lib_cost, road_cost, edges) << "\n";
  }
// can't close std::cout since it is not a std::ofstream
#if defined(PDHKR_LOCAL_BUILD)
  fout << std::flush;
#else
  fout.close();
#endif  // !defined(PDHKR_LOCAL_BUILD)
// if testing, do comparison in the program itself
#if defined(PDHKR_TEST)
  return pdhkr::exit_compare<long long>(fans, fout);
#else
  return EXIT_SUCCESS;
#endif  // !defined(PDHKR_TEST)
}

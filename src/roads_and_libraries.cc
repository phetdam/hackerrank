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

// part of HackerRank template code
////////////////////////////////////////////////////////////////////////////////
// #include <bits/stdc++.h>
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
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
    // insert end into submap
    if (edges_[start].find(end) == edges_[start].end())
      edges_[start][end] = true;
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
   * Look up neighbor map for the specified node.
   *
   * If no neighbor map exists then an empty one is returned.
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
  std::unordered_map<int, std::unordered_map<int, bool>> edges_;
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
// renamed from roadsAndLibraries
long roads_and_libraries(
  int n, int c_lib, int c_road, const std::vector<std::vector<int>>& cities)
{
  // map for unvisited nodes
  std::unordered_map<int, bool> unvisited;
  for (auto i = 0; i < n; i++)
    unvisited[i] = true;
  // adjacency list of edges (indexed from 0). fill cities as undirected edges
  adjacency_list edges;
  for (const auto& edge : cities) {
    edges.insert(edge[0] - 1, edge[1] - 1);
    edges.insert(edge[1] - 1, edge[0] - 1);
  }
  // total cost
  long total = 0;
  // current set of connected nodes. this is used to detect whether or not a
  // new node we are searching from is disconnected from the others
  std::unordered_map<int, bool> connected;
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
      // otherwise, need to grab something from the unvisited map
      return unvisited.begin()->first;
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
      connected[root] = true;
    }
    // for each neighbor (can be empty list)
    for (const auto [node, _] : edges.neighbors(root)) {
      // undirected edge has a connection + not visited + not connected
      if (
        unvisited.find(node) != unvisited.end() &&
        connected.find(node) == connected.end() &&
        (edges.contains(root, node) || edges.contains(node, root))
      ) {
        // increment cost by c_road (build road) + add to connected
        total += c_road;
        connected[node] = true;
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

// part of HackerRank template code
////////////////////////////////////////////////////////////////////////////////
int main()
{
#if defined(PDHKR_LOCAL_BUILD)
  // for local run, this is simply std::cout
  auto& fout = std::cout;
#else
  // as-is from HackerRank. this is unsafe
    ofstream fout(getenv("OUTPUT_PATH"));
#endif  // !defined(PDHKR_LOCAL_BUILD)
    string q_temp;
    getline(cin, q_temp);

    int q = stoi(ltrim(rtrim(q_temp)));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string first_multiple_input_temp;
        getline(cin, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int n = stoi(first_multiple_input[0]);

        int m = stoi(first_multiple_input[1]);

        int c_lib = stoi(first_multiple_input[2]);

        int c_road = stoi(first_multiple_input[3]);

        vector<vector<int>> cities(m);

        for (int i = 0; i < m; i++) {
            cities[i].resize(2);

            string cities_row_temp_temp;
            getline(cin, cities_row_temp_temp);

            vector<string> cities_row_temp = split(rtrim(cities_row_temp_temp));

            for (int j = 0; j < 2; j++) {
                int cities_row_item = stoi(cities_row_temp[j]);

                cities[i][j] = cities_row_item;
            }
        }

        // was roadsAndLibraries, i hate camelCase
        long result = roads_and_libraries(n, c_lib, c_road, cities);

        fout << result << "\n";
    }
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
////////////////////////////////////////////////////////////////////////////////

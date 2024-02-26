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
#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);
////////////////////////////////////////////////////////////////////////////////

/**
 * 2D matrix abstraction with flat storage.
 */
template <typename T>
class flat_matrix {
public:
  using value_type = T;

  /**
   * Ctor.
   *
   * Storage is default-initialized.
   */
  flat_matrix(std::size_t n_rows, std::size_t n_cols)
    : n_rows_{n_rows}, n_cols_{n_cols}, values_(n_rows * n_cols)
  {}

  /**
   * Return number of rows.
   */
  auto n_rows() const noexcept { return n_rows_; }

  /**
   * Return number of columns.
   */
  auto n_cols() const noexcept { return n_cols_; }

  /**
   * Return const reference to values.
   */
  const auto& values() const noexcept { return values_; }

  /**
   * Return reference to `(i, j)` element.
   */
  auto& operator()(std::size_t row, std::size_t col)
  {
    return values_.at(row * n_cols_ + col);
  }

  /**
   * Return const reference to `(i, j)` element.
   */
  const auto& operator()(std::size_t row, std::size_t col) const
  {
    return values_.at(row * n_cols_ + col);
  }

private:
  std::size_t n_rows_;
  std::size_t n_cols_;
  std::vector<T> values_;
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
long roadsAndLibraries(
  int n, int c_lib, int c_road, const vector<vector<int>>& cities)
{
  // otherwise, build a minimal spanning forest for the cities.
  // we first use a map that indicates unvisited nodes (start 0)
  std::unordered_map<int, bool> unvisited;
  for (auto i = 0; i < n; i++)
    unvisited[i] = true;
  // adjacency matrix of edges (start 0). fill cities as undirected edges
  flat_matrix<unsigned int> edges(n, n);
  for (const auto& edge : cities) {
    edges(edge[0] - 1, edge[1] - 1) = 1;
    edges(edge[1] - 1, edge[0] - 1) = 1;
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
    // for each edge root is connected to
    for (auto i = 0; i < n; i++) {
      // undirected edge has a connection + not visited + not connected
      if (
        unvisited.find(i) != unvisited.end() &&
        connected.find(i) == connected.end() &&
        (edges(i, root) || edges(root, i))
      ) {
        // increment cost by c_road (build road) + add to connected
        total += c_road;
        connected[i] = true;
        // add nodes to search through
        queue.push_back(i);
      }
    }
  }
  // return total cost of one library + minimum spanning forest vs. all
  // libraries without building any roads at all
  return std::min(static_cast<decltype(total)>(c_lib * n), total);
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

        long result = roadsAndLibraries(n, c_lib, c_road, cities);

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

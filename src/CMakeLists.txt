cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# all programs compile with this defined. this allows us to add extra checking
# or features that are completely disabled when compiled on HackerRank.
# note: test programs define PDHKR_TEST which takes precedence over PDHKR_LOCAL
add_compile_definitions(PDHKR_LOCAL)

# https://www.hackerrank.com/challenges/torque-and-development/problem
add_executable(roads_and_libraries roads_and_libraries.cc)
# register test cases (programs)
#
# TODO
#
# Update pdhkr_add_tests documentation with the following content.
#
# Each of the test programs should replace reading from std::cin and writing to
# std::cout or OUTPUT_PATH used by some HackerRank problems by reading input
# from the .in file, writing program output to a std::stringstream, and reading
# expected output from the .out file. The programs should return the value of
# pdhkr::exit_compare from main on the expected and actual values.
pdhkr_add_tests(TARGET roads_and_libraries TEST_CASES 0 2r 2s 3)

# https://www.hackerrank.com/challenges/journey-to-the-moon/problem
add_executable(journey_to_moon journey_to_moon.cc)
# note: maybe disable 11 since with -O0 it takes nearly 20 seconds to run
pdhkr_add_tests(TARGET journey_to_moon TEST_CASES 0 1r 1s 4 11)

# https://www.hackerrank.com/challenges/subtrees-and-paths/problem
add_executable(subtrees_and_paths subtrees_and_paths.cc)
# note: 2, 3 are currently failing
pdhkr_add_tests(TARGET subtrees_and_paths TEST_CASES 0 1 2 3)
# uncomment to use the tree_node class which only works if the input results in
# an actual tree. some inputs result in a forest being created.
# target_compile_definitions(subtrees_and_paths PRIVATE USE_TREE_NODE)

# https://www.hackerrank.com/challenges/merge-two-sorted-linked-lists/problem
add_executable(merge_sorted_linked_lists merge_sorted_linked_lists.cc)
pdhkr_add_tests(TARGET merge_sorted_linked_lists TEST_CASES 0 3 5)

# https://www.hackerrank.com/challenges/get-the-value-of-the-node-at-a-
# specific-position-from-the-tail/problem
add_executable(node_from_tail node_from_tail.cc)
pdhkr_add_tests(TARGET node_from_tail TEST_CASES 0 4 5)
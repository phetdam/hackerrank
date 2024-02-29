cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# by default, PDHKR_OUTPUT_FILE is defined so that each program will write
# their output to a file. this is useful for testing and should be overridden
# on a per-target basis by defining PDHKR_NO_OUTPUT_FILE
# add_compile_definitions(PDHKR_OUTPUT_FILE)
# all programs compile with this defined. this allows us to add extra checking
# or features that are completely disabled when compiled on HackerRank
add_compile_definitions(PDHKR_LOCAL_BUILD)

# https://www.hackerrank.com/challenges/torque-and-development/problem
add_executable(roads_and_libraries roads_and_libraries.cc)

# https://www.hackerrank.com/challenges/journey-to-the-moon/problem
add_executable(journey_to_moon journey_to_moon.cc)

# https://www.hackerrank.com/challenges/subtrees-and-paths/problem
add_executable(subtrees_and_paths subtrees_and_paths.cc)
# uncomment to use the tree_node class which only works if the input results in
# an actual tree. some inputs result in a forest being created.
# target_compile_definitions(subtrees_and_paths PRIVATE USE_TREE_NODE)
# uncomment to have program allow writing to output file
# target_compile_definitions(subtrees_and_paths PRIVATE PDHKR_OUTPUT_FILE)

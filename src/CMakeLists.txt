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
# register test cases (programs)
#
# TODO
#
# add a CMake function to simplify this process. the idea is to
# conditionally compile the program with the following macros:
#
#   PDHKR_TEST
#   PDHKR_TEST_INPUT
#   PDHKR_TEST_OUTPUT
#
# the latter two macros should take string paths that are absolute or relative
# to the top-level repo dir, indicating the .in input and .out output files for
# the specified test case. each test program is suffixed with the stem of the
# test input case, which has the naming pattern [0-9]+[rs]?. the r and s are
# used to disambiguate the "Run Code" version of a test case versus the
# "Submit Code" version of a test case on HackerRank. these can be different.
#
# each of the test programs should replace reading from std::cin and writing to
# std::cout or some OUTPUT_PATH used by some HackerRank problems by reading
# from the .in and .out files, and doing something in the program input to
# allow comparison with the .out files. have not yet decided on whether some
# standardized diffing mechanism is the best or if ad-hoc diffing is better.
#
# here is roads_and_libraries_0, roads_and_libraries_2s as ilustrations.
#
add_executable(roads_and_libraries_0 roads_and_libraries.cc)
# conditionally compile the same source
target_compile_definitions(
    roads_and_libraries_0 PRIVATE
    PDHKR_TEST
    # quotes are needed to pass a string value
    PDHKR_TEST_INPUT="${CMAKE_SOURCE_DIR}/data/roads_and_libraries/0.in"
    PDHKR_TEST_OUTPUT="${CMAKE_SOURCE_DIR}/data/roads_and_libraries/0.out"
)
# also register as CTest test. WORKING_DIRECTORY optional for absolute path
add_test(NAME roads_and_libraries_0 COMMAND roads_and_libraries_0)

add_executable(roads_and_libraries_2s roads_and_libraries.cc)
target_compile_definitions(
    roads_and_libraries_2s PRIVATE
    PDHKR_TEST
    # quotes are needed to pass a string value
    PDHKR_TEST_INPUT="${CMAKE_SOURCE_DIR}/data/roads_and_libraries/2s.in"
    PDHKR_TEST_OUTPUT="${CMAKE_SOURCE_DIR}/data/roads_and_libraries/2s.out"
)
add_test(NAME roads_and_libraries_2s COMMAND roads_and_libraries_2s)

add_executable(roads_and_libraries_3 roads_and_libraries.cc)
target_compile_definitions(
    roads_and_libraries_3 PRIVATE
    PDHKR_TEST
    # quotes are needed to pass a string value
    PDHKR_TEST_INPUT="${CMAKE_SOURCE_DIR}/data/roads_and_libraries/3.in"
    PDHKR_TEST_OUTPUT="${CMAKE_SOURCE_DIR}/data/roads_and_libraries/3.out"
)
add_test(NAME roads_and_libraries_3 COMMAND roads_and_libraries_3)

# https://www.hackerrank.com/challenges/journey-to-the-moon/problem
add_executable(journey_to_moon journey_to_moon.cc)

# https://www.hackerrank.com/challenges/subtrees-and-paths/problem
add_executable(subtrees_and_paths subtrees_and_paths.cc)
# uncomment to use the tree_node class which only works if the input results in
# an actual tree. some inputs result in a forest being created.
# target_compile_definitions(subtrees_and_paths PRIVATE USE_TREE_NODE)
# uncomment to have program allow writing to output file
# target_compile_definitions(subtrees_and_paths PRIVATE PDHKR_OUTPUT_FILE)

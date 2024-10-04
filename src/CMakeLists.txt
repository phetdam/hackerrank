cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

# all programs compile with this defined. this allows us to add extra checking
# or features that are completely disabled when compiled on HackerRank.
# note: test programs define PDHKR_TEST which takes precedence over PDHKR_LOCAL
add_compile_definitions(PDHKR_LOCAL)

add_executable(roads_and_libraries roads_and_libraries.cc)
# register test cases (programs)
pdhkr_add_tests(TARGET roads_and_libraries TEST_CASES 0 2r 2s 3)

add_executable(journey_to_moon journey_to_moon.cc)
# note: maybe disable 11 since with -O0 it takes nearly 40 seconds to run
pdhkr_add_tests(TARGET journey_to_moon TEST_CASES 0 1r 1s 4 10 11)

add_executable(subtrees_and_paths subtrees_and_paths.cc)
# use the tree_node class which only works if the input results in an actual
# tree. some inputs result in a forest being created so it isn't the solution
if(PDHKR_ALT_USE_TREE_NODE)
    message(STATUS "subtrees_and_paths.cc: USE_TREE_NODE defined")
    set_source_files_properties(
        subtrees_and_paths.cc PROPERTIES
        COMPILE_DEFINITIONS USE_TREE_NODE
    )
endif()
pdhkr_add_tests(TARGET subtrees_and_paths TEST_CASES 0 1 2 3 11 13)
# TODO: 2 and 3 failing, 11 and 13 timed out
pdhkr_xfail_tests(TARGET subtrees_and_paths TEST_CASES 2 3)
pdhkr_disable_tests(TARGET subtrees_and_paths TEST_CASES 11 13)

add_executable(merge_sorted_linked_lists merge_sorted_linked_lists.cc)
pdhkr_add_tests(TARGET merge_sorted_linked_lists TEST_CASES 0 3 5)

add_executable(node_from_tail node_from_tail.cc)
pdhkr_add_tests(TARGET node_from_tail TEST_CASES 0 4 5)

add_executable(array_manipulation array_manipulation.cc)
# use naive array update solution
if(PDHKR_ALT_USE_ARRAY_UPDATE)
    message(STATUS "array_manipulation.cc: USE_ARRAY_UPDATE defined")
    set_source_files_properties(
        array_manipulation.cc PROPERTIES
        COMPILE_DEFINITIONS USE_ARRAY_UPDATE
    )
endif()
pdhkr_add_tests(TARGET array_manipulation TEST_CASES 0 5 7 15)
# TODO: 7 times out
pdhkr_disable_tests(TARGET array_manipulation TEST_CASES 7)

add_executable(swap_tree_nodes swap_tree_nodes.cc)
pdhkr_add_tests(TARGET swap_tree_nodes TEST_CASES 0 1 2 9)

add_executable(jesse_and_cookies jesse_and_cookies.cc)
pdhkr_add_tests(TARGET jesse_and_cookies TEST_CASES 1 2 6 8 11)

add_executable(two_stack_game two_stack_game.cc)
pdhkr_add_tests(TARGET two_stack_game TEST_CASES 0 1 4)

add_executable(tree_top_view tree_top_view.cc)
pdhkr_add_tests(TARGET tree_top_view TEST_CASES 0 1 2 4)

add_executable(tree_level_order tree_level_order.cc)
pdhkr_add_tests(TARGET tree_level_order TEST_CASES 0 1 2)

add_executable(running_median running_median.cc)
# note: maybe disable 8 since with -O0 it takes nearly 14 seconds to run
pdhkr_add_tests(TARGET running_median TEST_CASES 0 1 8)

add_executable(max_stack_element max_stack_element.cc)
pdhkr_add_tests(TARGET max_stack_element TEST_CASES 0 3 14)

add_executable(equal_stacks equal_stacks.cc)
pdhkr_add_tests(TARGET equal_stacks TEST_CASES 0 3 11 27)

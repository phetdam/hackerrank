cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

##
# Add test programs for the given test cases for a HackerRank submission.
#
# Suppose we have a HackerRank submission program target with the name
# my_program, with corresponding C++ source my_program.cc, and relevant input +
# output files in data/my_program named case_1.in, case_1.out, etc. for test
# cases case_1 through case_n. For each case_k of the test cases, this function
# adds a test program with name target_case_k, reading input from case_k.in,
# reading expected output from case_k.out, that can be run with no arguments.
#
# Arguments:
#   TARGET target
#       Name of the HackerRank submission target, the .cc file stem, as well as
#       the subdirectory name in data/ with the relevant .in, .out files
#
#   TEST_CASES test_case...
#       Test case identifiers, suffixed to the test program name after an
#       underscore, that is also the stem for the relevant .in, .out files.
#       The naming convention for the test cases follows the regex [0-9]+[rs]?
#       where the r and s are disambiguate the "Run Code" version versus the
#       "Submit Code" version of a test case on HackerRank. Surprisingly, these
#       can actually be different, which can be confusing.
#
function(pdhkr_add_tests)
    # parse TARGET parent target name, TEST_CASES test target suffices + stems
    # for the input/output files. ARGV contains all the function arguments
    set(SINGLE_VALUE_ARGS TARGET)
    set(MULTI_VALUE_ARGS TEST_CASES)
    cmake_parse_arguments(
        HOST
        "" "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGV}
    )
    # check args. both are required
    if(NOT DEFINED HOST_TARGET)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: missing TARGET")
    endif()
    if(NOT DEFINED HOST_TEST_CASES)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: missing TEST_CASES")
    endif()
    # for each of the test case stems
    foreach(CASE IN LISTS HOST_TEST_CASES)
        # test target name
        set(TEST_TARGET ${HOST_TARGET}_${CASE})
        # executable combines suffix with target
        add_executable(${TEST_TARGET} ${HOST_TARGET}.cc)
        # dependent on host target so that if host target compilation fails
        # the test targets are not compiled. if this is not done, all the test
        # targets will also fail and duplicate compile messages are shown
        add_dependencies(${TEST_TARGET} ${HOST_TARGET})
        # macro definitions for enabling testing + providing input/output files
        target_compile_definitions(
            ${TEST_TARGET} PRIVATE
            PDHKR_TEST
            # quotes are needed to pass a string value via macro. we use absolute
            # paths here so that the program can be run from different directories.
            # note: cannot break string here or else CMake sees syntax error
            PDHKR_TEST_INPUT="${CMAKE_SOURCE_DIR}/data/${HOST_TARGET}/${CASE}.in"
            PDHKR_TEST_OUTPUT="${CMAKE_SOURCE_DIR}/data/${HOST_TARGET}/${CASE}.out"
        )
        # add for CTest
        add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
    endforeach()
endfunction()

##
# Skip added tests for the given test cases for a HackerRank submission.
#
# Any CTest test that was already added with pdhkr_add_tests can be skipped
# here, e.g. if the test takes too long to run, if the test is failing, etc.
#
# Arguments:
#   TARGET target
#       Name of the HackerRank submission target
#
#   TEST_CASES test_case...
#       Test case identifiers, suffixed to the test program name after an
#       underscore, that is also the stem for the relevant .in, .out files.
#       See pdhkr_add_tests for details on the naming convention.
#
function(pdhkr_skip_tests)
    # parse TARGET parent target name, TEST_CASES test target suffices + stems
    # for the input/output files. ARGV contains all the function arguments
    set(SINGLE_VALUE_ARGS TARGET)
    set(MULTI_VALUE_ARGS TEST_CASES)
    cmake_parse_arguments(
        HOST
        "" "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGV}
    )
    # check args. both are required
    if(NOT DEFINED HOST_TARGET)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: missing TARGET")
    endif()
    if(NOT DEFINED HOST_TEST_CASES)
        message(FATAL_ERROR "${CMAKE_CURRENT_FUNCTION}: missing TEST_CASES")
    endif()
    # for each of the test case stems skip EXIT_FAILURE
    foreach(CASE IN LISTS HOST_TEST_CASES)
        set_tests_properties(${HOST_TARGET}_${CASE} PROPERTIES SKIP_RETURN_CODE 1)
    endforeach()
endfunction()

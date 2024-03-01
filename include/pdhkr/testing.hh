/**
 * @file testing.hh
 * @author Derek Huang
 * @brief C++ header for compiling HackerRank submissions as local tests
 * @copyright MIT License
 *
 * This header facilitates conditional compilation of HackerRank submissions as
 * self-contained test programs. These programs read from an input file, write
 * to a `std::stringstream`, and then compare the `std::stringstream` output
 * with the expected output read from an output file. The input and output file
 * names are hardcoded at compile time so each program takes zero arguments.
 *
 * The following macros are used to control compilation as a test program:
 *
 * `PDHKR_TEST`
 *    Defined during compilation to indicate compilation as a test program
 *
 * `PDHKR_TEST_INPUT`
 *    Defined with a string value during compilation providing an absolute path
 *    to a .in test case input file in a `data/` subdirectory
 *
 * `PDHKR_TEST_OUTPUT`
 *    Defined with a string value during compilation providing an absolute path
 *    to a .out test case output file in a `data/` subdirectory
 *
 * When this header is included when `PDHKR_TEST` is defined, preprocessor
 * checks are used to ensure that `PDHKR_TEST_INPUT` and `PDHKR_TEST_OUTPUT`
 * are both defined. If not, an error is emitted during preprocessing.
 */

#ifndef PDHKR_TESTING_H_
#define PDHKR_TESTING_H_

#include "pdhkr/compare.hh"

// building as standalone test program
#ifdef PDHKR_TEST
// must have both defined
#ifndef PDHKR_TEST_INPUT
#error "PDHKR_TEST_INPUT not defined"
#endif  // PDHKR_TEST_INPUT
#ifndef PDHKR_TEST_OUTPUT
#error "PDHKR_TEST_OUTPUT not defined"
#endif  // PDHKR_TEST_OUTPUT
#endif  // PDHKR_TEST

#endif  // PDHKR_TESTING_H_
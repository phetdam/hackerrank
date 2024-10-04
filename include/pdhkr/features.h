/**
 * @file features.h
 * @author Derek Huang
 * @brief C/C++ header for determining compiler/platform features
 * @copyright MIT License
 */

#ifndef PDHKR_FEATURES_H_
#define PDHKR_FEATURES_H_

#include "pdhkr/common.h"

// C++ standards support
#ifdef PDHKR_CPLUSPLUS
// C++17
#if PDHKR_CPLUSPLUS >= 201703L
#define PDHKR_HAS_CC_17 1
#endif  // PDHKR_CPLUSPLUS >= 201703L
// C++20
#if PDHKR_CPLUSPLUS >= 202002L
#define PDHKR_HAS_CC_20 1
#endif  // PDHKR_CPLUSPLUS >= 202002L
#endif  // PDHKR_CPLUSPLUS

// no support
#ifndef PDHKR_HAS_CC_17
#define PDHKR_HAS_CC_17 0
#endif  // PDHKR_HAS_CC_17
#ifndef PDHKR_HAS_CC_20
#define PDHKR_HAS_CC_20 0
#endif  // PDHKR_HAS_CC_20

#endif  // PDHKR_FEATURES_H_

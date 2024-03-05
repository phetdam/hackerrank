/**
 * @file common.h
 * @author Derek Huang
 * @brief C/C++ header for common shared macros
 * @copyright MIT License
 */

#ifndef PDHKR_COMMON_H_
#define PDHKR_COMMON_H_

// C++ version macro accurate even without /Zc:__cplusplus defined for MSVC
#if defined(_MSVC_LANG)
#define PDHKR_CPLUSPLUS _MSVC_LANG
#elif defined(__cplusplus)
#define PDHKR_CPLUSPLUS __cplusplus
#endif  // !defined(_MSVC_LANG) && !defined(__cplusplus)

// stringification macros. first one does not expand macro args
#define PDHKR_STRINGIFY_I(x) #x
#define PDHKR_STRINGIFY(x) PDHKR_STRINGIFY_I(x)

// concatenation macros. first one does not expand macro args
#define PDHKR_CONCAT_I(a, b) a ## b
#define PDHKR_CONCAT(a, b) PDHKR_CONCAT_I(a, b)

#endif  // PDHKR_COMMON_H_

/**
 * @file type_traits.hh
 * @author Derek Huang
 * @brief C++ header for type traits helpers
 * @copyright MIT License
 */

#ifndef PDHKR_TYPE_TRAITS_HH_
#define PDHKR_TYPE_TRAITS_HH_

#include <type_traits>
#include <vector>

#include "pdhkr/common.h"
#include "pdhkr/features.h"

namespace pdhkr {

/**
 * Helper for checking if a class has a `value_type` type member.
 *
 * @todo May be replaced by a macro so we don't repeat these definitions.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_type_member_value_type : std::false_type {};

/**
 * Partial specialization for when the type has a `value_type` type member.
 *
 * @todo May be replaced by a macro so we don't repeat these definitions.
 *
 * @tparam T type
 */
template <typename T>
struct has_type_member_value_type<T, std::void_t<typename T::value_type>>
  : std::true_type {};

/**
 * Boolean helper for checking if a class has a `value_type` type member.
 *
 * @todo May be replaced by a macro so we don't repeat these definitions.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool
has_type_member_value_type_v = has_type_member_value_type<T>::value;

/**
 * Macro expanding to boolean to indicate if `T` has a member `member`.
 *
 * @param T Target type
 * @param member Member name
 */
#define PDHKR_HAS_TYPE_MEMBER(T, member) \
  PDHKR_CONCAT(pdhkr::has_type_member_, PDHKR_CONCAT(member, _v))<T>

/**
 * Helper indicating if `T` is a `std::vector<>` specialization.
 *
 * @tparam T type
 */
template <typename T>
struct is_std_vector : std::false_type {};

/**
 * Partial specialization for when `T` is a `std::vector<>` specialization.
 *
 * @tparam T type
 * @tparam A Allocator
 */
template <typename T, typename A>
struct is_std_vector<std::vector<T, A>> : std::true_type {};

/**
 * Boolean helper for checking if `T` is a `std::vector<>` specialization.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

// no std::type_identity before C++20
#if !PDHKR_HAS_CC_20
/**
 * Type identity helper to induce a non-deduced context.
 *
 * @tparam T type
 */
template <typename T>
struct type_identity { using type = T; };

/**
 * Type identity type alias inducing a non-deduced context.
 *
 * @tparam T type
 */
template <typename T>
using type_identity_t = typename type_identity<T>::type;
#else
// use C++20 traits class
template <typename T>
using type_identity = std::type_identity<T>;

// use C++20 traits type helper
template <typename T>
using type_identity_t = std::type_identity_t<T>;
#endif  // PDHKR_HAS_CC_20

}  // namespace pdhkr

#endif  // PDHKR_TYPE_TRAITS_HH_

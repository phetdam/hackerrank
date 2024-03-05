/**
 * @file type_traits.hh
 * @author Derek Huang
 * @brief C++ header for type traits helpers
 * @copyright MIT License
 */

#ifndef PDHKR_TYPE_TRAITS_HH_
#define PDHKR_TYPE_TRAITS_HH_

#include <type_traits>

#include "pdhkr/common.h"

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
template <typename T, typename = void, typename = void>
struct is_std_vector : std::false_type {};

/**
 * Partial specialization for when `T` is a `std::vector<>` specialization.
 *
 * @note This does not work for vectors with custom allocators.
 *
 * @tparam T type
 */
template <typename T>
struct is_std_vector<T, std::void_t<typename T::value_type>>
  : std::bool_constant<
    std::is_same_v<T, std::vector<typename T::value_type>> > {};

/**
 * Boolean helper for checking if `T` is a `std::vector<>` specialization.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

}  // namespace pdhkr

#endif  // PDHKR_TYPE_TRAITS_HH_

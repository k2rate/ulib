#pragma once

#include <string>
#include <type_traits>
#include <iterator>

#include <ulib/containers/tags.h>

#include "container.h"

namespace ulib
{
    template <class T, class = void>
    struct is_std_basic_string : std::false_type
    {
    };

    template <class T>
    struct is_std_basic_string<T, std::void_t<typename T::value_type>>
        : std::bool_constant<std::is_same_v<T, std::basic_string<typename T::value_type>>>
    {
    };

    template <class T>
    inline constexpr bool is_std_basic_string_v = is_std_basic_string<T>::value;

    // -----------

    template <class T, class = void>
    struct is_std_basic_string_view : std::false_type
    {
    };

    template <class T>
    struct is_std_basic_string_view<T, std::void_t<typename T::value_type>>
        : std::bool_constant<std::is_same_v<T, std::basic_string_view<typename T::value_type>>>
    {
    };

    template <class T>
    inline constexpr bool is_std_basic_string_view_v = is_std_basic_string_view<T>::value;

    // -----------

    template <class T>
    inline constexpr bool is_std_string_kind_v = is_std_basic_string_view_v<T> || is_std_basic_string_v<T>;

    // -----------

    template <class T>
    inline constexpr bool is_ulib_string_v = is_ulib_container_tag_v<T, string_container_tag>;

    // -----------

    template <class T>
    inline constexpr bool is_string_v = is_ulib_string_v<T> || is_std_string_kind_v<T>;

} // namespace ulib
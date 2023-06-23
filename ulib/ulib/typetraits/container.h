#pragma once

#include <type_traits>
#include <iterator>
#include <ulib/containers/tags.h>

#include "iterator.h"
#include "carray.h"

namespace ulib
{
    // -----------

    template <class T, class Tag, class = void>
    struct is_ulib_container_tag : std::false_type
    {
    };

    template <class T, class Tag>
    struct is_ulib_container_tag<T, Tag, std::void_t<typename T::ContainerTagT>>
        : std::bool_constant<std::is_same_v<typename T::ContainerTagT, Tag>>
    {
    };

    template <class T, class Tag>
    inline constexpr bool is_ulib_container_tag_v = is_ulib_container_tag<T, Tag>::value;

    // -----------

    template <class T>
    inline constexpr bool is_random_accessible_v = is_c_array_v<T> || is_container_random_accessible_v<T>;

    // -----------

    template <class T, class = void>
    struct has_value_type : std::false_type
    {
    };

    template <class T>
    struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool has_value_type_v = has_value_type<T>::value;

    // -----------

    template <class T, class = void>
    struct has_container_tag : std::false_type
    {
    };

    template <class T>
    struct has_container_tag<T, std::void_t<typename T::ContainerTag>> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool has_container_tag_v = has_container_tag<T>::value;

    // -----------

    template <class T, class = void>
    struct is_ulib_container : std::false_type
    {
    };

    template <class T>
    struct is_ulib_container<T, std::void_t<typename T::ContainerTagT>> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool is_ulib_container_v = is_ulib_container<T>::value;

}
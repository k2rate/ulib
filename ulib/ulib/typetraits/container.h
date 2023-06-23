#pragma once

#include <type_traits>
#include <iterator>

#include <ulib/containers/kind.h>

namespace ulib
{
    // -----------

    template <class T, ContainerKind Kind, class = void>
    struct is_ulib_container_kind : std::false_type
    {
    };

    template <class T, ContainerKind Kind>
    struct is_ulib_container_kind<T, Kind, std::void_t<decltype(T::Kind)>>
        : std::bool_constant<std::is_same_v<std::decay_t<decltype(T::Kind)>, ContainerKind> && T::Kind == Kind>
    {
    };

    template <class T, ContainerKind Kind>
    inline constexpr bool is_ulib_container_kind_v = is_ulib_container_kind<T, Kind>::value;

    // -----------

    template <typename T, size_t N>
    constexpr bool IsCxxArray(const T (&arr)[N])
    {
        return true;
    }

    template <typename T>
    constexpr bool IsCxxArray(const T &)
    {
        return false;
    }

    template <class T>
    struct is_cxx_array : std::bool_constant<IsCxxArray(*(T *)0)>
    {
    };

    template <class T>
    inline constexpr bool is_cxx_array_v = is_cxx_array<T>::value;

    // -----------

    template <class T, class = void>
    struct is_container_random_accessible : std::false_type
    {
    };

    template <class T>
    struct is_container_random_accessible<T, std::void_t<typename T::iterator>>
        : std::bool_constant<std::is_same_v<typename T::iterator::iterator_category, std::random_access_iterator_tag>>
    {
    };

    template <class T>
    inline constexpr bool is_container_random_accessible_v = is_container_random_accessible<T>::value;

    // -----------

    template <class T>
    inline constexpr bool is_random_accessible_v = is_cxx_array_v<T> || is_container_random_accessible_v<T>;

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
    struct has_container_kind : std::false_type
    {
    };

    template <class T>
    struct has_container_kind<T, std::void_t<typename T::ContainerKind>> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool has_container_kind_v = has_container_kind<T>::value;

    // -----------

    template <class T, class = void>
    struct is_ulib_container : std::false_type
    {
    };

    template <class T>
    struct is_ulib_container<T, std::void_t<decltype(T::Kind)>>
        : std::bool_constant<std::is_same_v<std::decay_t<decltype(T::Kind)>, ContainerKind>>
    {
    };

    template <class T>
    inline constexpr bool is_ulib_container_v = is_ulib_container<T>::value;

}
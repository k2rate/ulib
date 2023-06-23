#pragma once

#include <type_traits>
#include <iterator>

namespace ulib
{
    template <class T, class Tag, class = void>
    struct is_ulib_container_iterator_tag : std::false_type
    {
    };

    template <class T, class Tag>
    struct is_ulib_container_iterator_tag<T, Tag, std::void_t<typename T::iterator>>
        : std::bool_constant<std::is_same_v<typename T::iterator::iterator_category, Tag>>
    {
    };

    template <class T, class Tag>
    inline constexpr bool is_ulib_container_iterator_tag_v = is_ulib_container_iterator_tag<T, Tag>::value;

    template <class T>
    inline constexpr bool is_container_random_accessible_v = is_ulib_container_iterator_tag_v<T, std::random_access_iterator_tag>;

    template <class T>
    inline constexpr bool is_container_bidirectional_v = is_ulib_container_iterator_tag_v<T, std::bidirectional_iterator_tag>;
}
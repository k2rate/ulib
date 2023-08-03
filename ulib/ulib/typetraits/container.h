#pragma once

#include <ulib/typetraits/field.h>
#include <ulib/typetraits/carray.h>

#include <iterator>

#include <vector>

namespace ulib
{
    // -----------

    ULIB_DEFINE_TYPE_FIELD_CHECK(container_value_type, value_type);
    ULIB_DEFINE_TYPE_FIELD_CHECK(container_iterator, iterator);
    ULIB_DEFINE_TYPE_FIELD_CHECK_T(container_iterator_tag, typename std::iterator_traits<typename T::iterator>::iterator_category);
    ULIB_DEFINE_TYPE_FIELD_CHECK_T(container_data_method, decltype(std::declval<T>().data()));
    ULIB_DEFINE_TYPE_FIELD_CHECK_T(container_size_method, decltype(std::declval<T>().size()));

    ULIB_DEFINE_TYPE_FIELD_CHECK_T(iterator_value_type, typename std::iterator_traits<T>::value_type);
    ULIB_DEFINE_TYPE_FIELD_CHECK_T(iterator_tag, typename std::iterator_traits<T>::iterator_category);

    // -----------

    template <class T>
    inline constexpr bool is_container_random_accessible_v = is_container_iterator_tag_v<T, std::random_access_iterator_tag>;

    template <class T>
    inline constexpr bool is_container_bidirectional_v = is_container_iterator_tag_v<T, std::bidirectional_iterator_tag>;

    template <class T>
    inline constexpr bool is_random_accessible_v = is_c_array_v<T> || is_container_random_accessible_v<T>;

    // -----------
}
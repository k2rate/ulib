#pragma once

#include <type_traits>
#include <vector>

#include "container.h"
#include "stdlist.h"

namespace ulib
{
    template <class T, class = void>
    struct is_std_vector : std::false_type
    {
    };

    template <class T>
    struct is_std_vector<T, std::void_t<typename T::value_type>>
        : std::bool_constant<std::is_same_v<T, std::vector<typename T::value_type>>>
    {
    };

    template <class T>
    inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

    // ----------

    template <class T>
    inline constexpr bool is_ulib_list_v = is_ulib_container_tag_v<T, list_container_tag>;

    // ----------

    template <class T>
    inline constexpr bool is_list_v = is_ulib_list_v<T>|| is_std_list_v<T> || is_std_vector_v<T> ;
}
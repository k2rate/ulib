#pragma once

#include <type_traits>
#include <list>

namespace ulib
{
    // ----------

    template <class T, class = void>
    struct is_std_list : std::false_type
    {
    };

    template <class T>
    struct is_std_list<T, std::void_t<typename T::value_type>>
        : std::bool_constant<std::is_same_v<T, std::list<typename T::value_type>>>
    {
    };

    template <class T>
    inline constexpr bool is_std_list_v = is_std_list<T>::value;
}
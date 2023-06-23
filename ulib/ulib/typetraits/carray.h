#pragma once

#include <type_traits>

namespace ulib
{
    template <class T>
    struct is_c_array : std::false_type
    {
    };

    template <class T, int N>
    struct is_c_array<T[N]> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool is_c_array_v = is_c_array<T>::value;
} // namespace ulib
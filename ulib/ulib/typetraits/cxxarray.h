#pragma once

#include <type_traits>

namespace ulib
{
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
}
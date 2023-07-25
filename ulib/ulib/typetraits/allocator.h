#pragma once

#include <memory>

namespace ulib
{
    template <class T, class = void>
    struct std_container_allocator
    {
        using Type = void;
    };

    template <class T>
    struct std_container_allocator<T, std::void_t<typename T::allocator_type>>
    {
        using Type = typename T::allocator_type;
    };

    template<class T>
    using std_container_allocator_t = typename std_container_allocator<T>::Type;


    template <class T, class = void>
    struct ulib_container_allocator
    {
        using Type = void;
    };

    template <class T>
    struct ulib_container_allocator<T, std::void_t<typename T::AllocatorT>>
    {
        using Type = typename T::AllocatorT;
    };

    template<class T>
    using ulib_container_allocator_t = typename ulib_container_allocator<T>::Type;

    template <class T>
    inline constexpr bool has_std_allocator_v = !std::is_same_v<std_container_allocator_t<T>, void>;

    template <class T>
    inline constexpr bool has_ulib_allocator_v = !std::is_same_v<ulib_container_allocator_t<T>, void>;

    template<class T>
    inline constexpr bool has_allocator_v = has_std_allocator_v<T> || has_ulib_allocator_v<T>;
}
#pragma once

#include <type_traits>

namespace ulib
{
    struct missing_type
    {
        missing_type() = delete;
        ~missing_type() = delete;
    };
} // namespace ulib

#define ULIB_DEFINE_TYPE_FIELD_CHECK_T(check_name, type_statement)                                                                                   \
    template <class T, class = void>                                                                                                                 \
    struct check_name                                                                                                                                \
    {                                                                                                                                                \
        using type = missing_type;                                                                                                                           \
    };                                                                                                                                               \
    template <class T>                                                                                                                               \
    struct check_name<T, std::void_t<type_statement>>                                                                                                \
    {                                                                                                                                                \
        using type = type_statement;                                                                                                                 \
    };                                                                                                                                               \
    template <class K>                                                                                                                               \
    using check_name##_t = typename check_name<K>::type;                                                                                             \
    template <class T>                                                                                                                               \
    inline constexpr bool has_##check_name##_v = !std::is_same_v<check_name##_t<T>, ulib::missing_type>;                                                           \
    template <class T, class T1>                                                                                                                     \
    inline constexpr bool is_##check_name##_v = std::is_same_v<check_name##_t<T>, T1>;

#define ULIB_DEFINE_TYPE_FIELD_CHECK(check_name, type_statement) ULIB_DEFINE_TYPE_FIELD_CHECK_T(check_name, typename T::type_statement)

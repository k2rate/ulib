#pragma once

#include "string.h"
#include <type_traits>
#include <ulib/encodings/literalencoding.h>


namespace ulib
{
    template <class T, class = void>
    struct string_encoding
    {
        using Type = void;
    };

    template <class T>
    struct string_encoding<T, std::void_t<typename T::EncodingT>>
    {
        using Type = typename T::EncodingT;
    };

    template <class T>
    struct string_encoding<T, std::enable_if_t<is_std_string_kind_v<T>>>
    {
        using Type = LiteralEncodingT<typename T::value_type>;
    };

    template <class T>
    using string_encoding_t = typename string_encoding<T>::Type;

    // -----------

    template <class T, class = void>
    struct argument_encoding
    {
        using Type = LiteralEncodingT<std::remove_const_t<std::remove_pointer_t<std::remove_all_extents_t<T>>>>;
    };

    template <class T>
    struct argument_encoding<T, std::enable_if_t<is_string_v<T>>>
    {
        using Type = string_encoding_t<T>;
    };

    template <class T>
    using argument_encoding_t = typename argument_encoding<T>::Type;

    template <class T>
    inline constexpr bool is_argument_encodeable_v = !std::is_same_v<argument_encoding_t<T>, void>;

} // namespace ulib
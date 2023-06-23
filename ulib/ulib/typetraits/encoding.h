#pragma once

#include <type_traits>
#include <ulib/encodings/literalencoding.h>
#include "string.h"

namespace ulib
{
    template<class T, class = void>
    struct string_encoding
    {
        using Type = void;
    };

    template<class T>
    struct string_encoding<T, std::void_t<typename T::EncodingT>>
    {
        using Type = typename T::EncodingT;
    };

    template<class T>
    struct string_encoding<T, std::enable_if_t<is_std_string_kind_v<T>>>
    {
        using Type = LiteralEncodingT<typename T::value_type>;
    };

    template<class T>
    using string_encoding_t = typename string_encoding<T>::Type;

    // -----------
}
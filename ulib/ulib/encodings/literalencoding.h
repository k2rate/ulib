#pragma once

#include "multibyte/encoding.h"
#include "wide/encoding.h"
#include "utf8/encoding.h"
#include "utf16/encoding.h"
#include "utf32/encoding.h"

#include <type_traits>

namespace ulib
{
    template <class CharT>
    struct EncodingFromValueType
    {
        using EncodingT = void;
    };

#ifdef __cpp_char8_t
    template <>
    struct EncodingFromValueType<char>
    {
        using EncodingT = MultibyteEncoding;
    };

    template <>
    struct EncodingFromValueType<char8_t>
    {
        using EncodingT = Utf8;
    };
#else
    template <>
    struct EncodingFromValueType<char>
    {
        using EncodingT = Utf8;
    };
#endif

    template <>
    struct EncodingFromValueType<char16_t>
    {
        using EncodingT = Utf16;
    };

    template <>
    struct EncodingFromValueType<char32_t>
    {
        using EncodingT = Utf32;
    };

    template <>
    struct EncodingFromValueType<wchar_t>
    {
        using EncodingT = WideEncoding;
    };

    template<class CharT>
    using LiteralEncodingT = typename EncodingFromValueType<std::remove_cv_t<CharT>>::EncodingT;
}
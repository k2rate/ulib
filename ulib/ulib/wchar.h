#pragma once

#include <type_traits>

#include <ulib/encodings/wide/string.h>
#include <ulib/encodings/wide/stringview.h>

#include <ulib/encodings/literalencoding.h>
#include <ulib/encodings/convert.h>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#include <string>

#endif

namespace ulib
{
    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline wstring wstr(const StringT &str)
    {
        return Convert<WideEncoding>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline wstring wstr(const CharT *str)
    {
        return Convert<WideEncoding>(str);
    }

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline wstring wstr(const std::basic_string<CharT> &str)
    {
        return Convert<WideEncoding>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline wstring wstr(std::basic_string_view<CharT> str)
    {
        return Convert<WideEncoding>(str);
    }

#endif
}
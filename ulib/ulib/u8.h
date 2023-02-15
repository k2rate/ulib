#pragma once

#include <type_traits>

#include <ulib/encodings/utf8/string.h>
#include <ulib/encodings/utf8/stringview.h>
#include <ulib/encodings/literalencoding.h>
#include <ulib/encodings/convert.h>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#include <string>
#endif

namespace ulib
{
    // convert multibyte string to u8 string
    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline u8string u8(const StringT &str)
    {
        return Convert<Utf8>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u8string u8(const CharT *str)
    {
        return Convert<Utf8>(str);
    }

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u8string u8(const std::basic_string<CharT> &str)
    {
        return Convert<Utf8>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u8string u8(std::basic_string_view<CharT> str)
    {
        return Convert<Utf8>(str);
    }

#endif

}
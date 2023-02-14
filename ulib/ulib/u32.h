#pragma once

#include <type_traits>

#include <ulib/encodings/utf32/string.h>
#include <ulib/encodings/utf32/stringview.h>

#include <ulib/encodings/literalencoding.h>
#include <ulib/encodings/convert.h>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#include <string>

#endif

namespace ulib
{
    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline u32string u32(const StringT &str)
    {
        return Convert<Utf32>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u32string u32(const CharT *str)
    {
        return Convert<Utf32>(str);
    }

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u32string u32(const std::basic_string<CharT> &str)
    {
        return Convert<Utf32>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u32string u32(std::basic_string_view<CharT> str)
    {
        return Convert<Utf32>(str);
    }

#endif
}
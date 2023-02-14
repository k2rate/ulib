#pragma once

#include <type_traits>

#include <ulib/encodings/utf16/string.h>
#include <ulib/encodings/utf16/stringview.h>

#include <ulib/encodings/literalencoding.h>
#include <ulib/encodings/convert.h>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#include <string>

#endif

namespace ulib
{
    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline u16string u16(const StringT &str)
    {
        return Convert<Utf16>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u16string u16(const CharT *str)
    {
        return Convert<Utf16>(str);
    }

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u16string u16(const std::basic_string<CharT> &str)
    {
        return Convert<Utf16>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u16string u16(std::basic_string_view<CharT> str)
    {
        return Convert<Utf16>(str);
    }

#endif

}
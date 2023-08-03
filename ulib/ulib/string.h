#pragma once

#include <type_traits>

#include <ulib/encodings/multibyte/string.h>
#include <ulib/encodings/multibyte/stringview.h>

#include <ulib/encodings/literalencoding.h>
#include <ulib/encodings/convert.h>

#ifdef ULIB_STD_COMPATIBILITY
#include <string_view>
#include <string>

#endif

namespace ulib
{
    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline string str(const StringT &str)
    {
        return Convert<MultibyteEncoding>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline string str(const CharT *str)
    {
        return Convert<MultibyteEncoding>(str);
    }

#ifdef ULIB_STD_COMPATIBILITY

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline string str(const std::basic_string<CharT> &str)
    {
        return Convert<MultibyteEncoding>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline string str(std::basic_string_view<CharT> str)
    {
        return Convert<MultibyteEncoding>(str);
    }

#endif
}
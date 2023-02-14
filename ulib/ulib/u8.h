#pragma once

#include <type_traits>

#include <ulib/encodings/multibyte/encoding.h>
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
    template <class StringT, class EncodingT = typename StringT::EncodingT,
              std::enable_if_t<std::is_same_v<EncodingT, MultibyteEncoding>, bool> = true>
    inline u8string u8(const StringT &str)
    {
        using CharT = typename u8string::CharT;
        return u8string((CharT *)str.data(), (CharT *)str.data() + str.size());
    }

    template <class StringT, class EncodingT = typename StringT::EncodingT,
              std::enable_if_t<!std::is_same_v<EncodingT, MultibyteEncoding>, bool> = true>
    inline u8string u8(const StringT &str)
    {
        return Convert<EncodingT, Utf8>(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u8string u8(const CharT *str)
    {
        if constexpr (std::is_same_v<CharT, char>)
        {
            using Char8T = typename u8string::CharT;
            return u8string((Char8T *)str);
        }
        else
        {
            return Convert<EncodingT, Utf8>(str);
        }
    }

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u8string u8(const std::basic_string<CharT> &str)
    {
        if constexpr (std::is_same_v<CharT, char>)
        {
            using Char8T = typename u8string::CharT;
            return u8string((Char8T *)str.data(), (Char8T *)str.data() + str.size());
        }
        else
        {
            return Convert<EncodingT, Utf8>(str);
        }
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
            std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline u8string u8(const std::basic_string_view<CharT> &str)
    {
        if constexpr (std::is_same_v<CharT, char>)
        {
            using Char8T = typename u8string::CharT;
            return u8string((Char8T *)str.data(), (Char8T *)str.data() + str.size());
        }
        else
        {
            return Convert<EncodingT, Utf8>(str);
        }
    }

#endif
}


/*
template<class StringT, class CharT = typename StringT::value_type, class AllocatorT = typename StringT::allocator_type>
struct StdStringCheck
{
    constexpr static bool kResult = std::is_same_v<StringT, std::basic_string<CharT, std::char_traits<CharT>, AllocatorT>>; 
};

template<class StringT>
inline constexpr bool is_std_string_v = StdStringCheck<StringT>::kResult;

constexpr bool kk = is_std_string_v<ulib::string>;
*/

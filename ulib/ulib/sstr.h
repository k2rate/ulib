#pragma once

#include <ulib/u8.h>
#include <ulib/wchar.h>

namespace ulib
{

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline std::string sstr(const StringT &str)
    {
        return u8(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline std::string sstr(const CharT *str)
    {
        return u8(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline std::string sstr(const std::basic_string<CharT> &str)
    {
        return u8(str);
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline std::string sstr(std::basic_string_view<CharT> str)
    {
        return u8(str);
    }

    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline std::wstring swstr(const StringT &str)
    {
        return wstr(u8(str));
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline std::wstring swstr(const CharT *str)
    {
        return wstr(u8(str));
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline std::wstring swstr(const std::basic_string<CharT> &str)
    {
        return wstr(u8(str));
    }

    template <class CharT, class EncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline std::wstring swstr(std::basic_string_view<CharT> str)
    {
        return wstr(u8(str));
    }

#endif

}
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
    /*
        template <class EncodingT, class AllocatorT, class StringT, class SourceEncodingT = typename StringT::EncodingT>
    inline ulib::EncodedString<EncodingT> recode(const StringT &str, typename AllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<typename SourceEncodingT::ParentEncodingT, EncodingT>)
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return ulib::EncodedString<EncodingT, AllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);
        }

        return Convert<SourceEncodingT, EncodingT>(str);
    }

    template <class EncodingT, class CharT, class SourceEncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<SourceEncodingT, void>, bool> = true>
    inline ulib::EncodedString<EncodingT> recode(const CharT *str)
    {
        if constexpr (std::is_same_v<typename SourceEncodingT::ParentEncodingT, EncodingT>)
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return ulib::EncodedString<EncodingT>((DestCharT *)str);
        }

        return Convert<SourceEncodingT, EncodingT>(str);
    }
    */


    // convert multibyte string to u8 string
    template <class StringT, class EncodingT = typename StringT::EncodingT,
              std::enable_if_t<std::is_same_v<EncodingT, MultibyteEncoding>, bool> = true>
    inline u8string u8(const StringT &str)
    {
        return Convert<Utf8>(str);
    }

    template <class StringT, class EncodingT = typename StringT::EncodingT,
              std::enable_if_t<!std::is_same_v<EncodingT, MultibyteEncoding>, bool> = true>
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

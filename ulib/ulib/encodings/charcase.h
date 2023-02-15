#pragma once

#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/typetraits/selecttype.h>
#include <ulib/allocators/tempallocator.h>

#include "convert.h"
#include "wide/encoding.h"
#include "literalencoding.h"

#include <ulib/config.h>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string>
#include <string_view>
#endif

namespace ulib
{
    namespace detail
    {
        template <class AllocatorT>
        inline void UppercaseChars(EncodedString<WideEncoding, AllocatorT> &str)
        {
            for (auto &ch : str)
                ch = towupper(ch);
        }

        template <class AllocatorT>
        inline void LowercaseChars(EncodedString<WideEncoding, AllocatorT> &str)
        {
            for (auto &ch : str)
                ch = towlower(ch);
        }
    }

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, class EncodingT, class AllocatorT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToUpper(const EncodedString<EncodingT, AllocatorT> &str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class EncodingT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToUpper(EncodedStringView<EncodingT> str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToUpper(const CharT *str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

/*

*/
#ifdef ULIB_USE_STD_STRING_VIEW

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline ulib::EncodedString<OutputEncodingT, OutputAllocatorT> ToUpper(const std::basic_string<CharT> &str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToUpper(std::basic_string_view<CharT> str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<OutputEncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::UppercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

#endif

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, class EncodingT, class AllocatorT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToLower(const EncodedString<EncodingT, AllocatorT> &str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class EncodingT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToLower(EncodedStringView<EncodingT> str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToLower(const CharT *str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
           detail::LowercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

/*

*/
#ifdef ULIB_USE_STD_STRING_VIEW

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline ulib::EncodedString<OutputEncodingT, OutputAllocatorT> ToLower(const std::basic_string<CharT> &str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<EncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> ToLower(std::basic_string_view<CharT> str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (std::is_same_v<OutputEncodingT, WideEncoding>)
        {
            auto wstr = Convert<WideEncoding, OutputAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return wstr;
        }
        else
        {
            auto wstr = Convert<WideEncoding, TempAllocatorT>(str);
            detail::LowercaseChars(wstr);
            return Convert<OutputEncodingT, OutputAllocatorT>(wstr);
        }
    }

#endif
}
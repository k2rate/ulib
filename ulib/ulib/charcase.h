#pragma once

#include <ulib/encodings/charcase.h>

namespace ulib
{
    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, class EncodingT, class AllocatorT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> upper(const EncodedString<EncodingT, AllocatorT> &str, typename OutputAllocatorT::Params al = {})
    {
        return ToUpper<OutputEncodingT, OutputAllocatorT>(str, al);
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class EncodingT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> upper(EncodedStringView<EncodingT> str, typename OutputAllocatorT::Params al = {})
    {
        return ToUpper<OutputEncodingT, OutputAllocatorT>(str, al);
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> upper(const CharT *str, typename OutputAllocatorT::Params al = {})
    {
        return ToUpper<OutputEncodingT, OutputAllocatorT>(str, al);
    }

/*

*/
#ifdef ULIB_STD_COMPATIBILITY

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline ulib::EncodedString<OutputEncodingT, OutputAllocatorT> upper(const std::basic_string<CharT> &str, typename OutputAllocatorT::Params al = {})
    {
        return ToUpper<OutputEncodingT, OutputAllocatorT>(str, al);
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> upper(std::basic_string_view<CharT> str, typename OutputAllocatorT::Params al = {})
    {
        return ToUpper<OutputEncodingT, OutputAllocatorT>(str, al);
    }

#endif

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, class EncodingT, class AllocatorT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> lower(const EncodedString<EncodingT, AllocatorT> &str, typename OutputAllocatorT::Params al = {})
    {
        return ToLower<OutputEncodingT, OutputAllocatorT>(str, al);
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class EncodingT,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> lower(EncodedStringView<EncodingT> str, typename OutputAllocatorT::Params al = {})
    {
        return ToLower<OutputEncodingT, OutputAllocatorT>(str, al);
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true, typename... T>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> lower(const CharT *str, typename OutputAllocatorT::Params al = {})
    {
        return ToLower<OutputEncodingT, OutputAllocatorT>(str, al);
    }

/*

*/
#ifdef ULIB_STD_COMPATIBILITY

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline ulib::EncodedString<OutputEncodingT, OutputAllocatorT> lower(const std::basic_string<CharT> &str, typename OutputAllocatorT::Params al = {})
    {
        return ToLower<OutputEncodingT, OutputAllocatorT>(str, al);
    }

    template <class UOutputEncodingT = void, class OutputAllocatorT = DefaultAllocator, class CharT, class EncodingT = LiteralEncodingT<CharT>,
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void>, bool> = true>
    inline EncodedString<OutputEncodingT, OutputAllocatorT> lower(std::basic_string_view<CharT> str, typename OutputAllocatorT::Params al = {})
    {
        return ToLower<OutputEncodingT, OutputAllocatorT>(str, al);
    }

#endif
}
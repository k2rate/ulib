#pragma once

#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/typetraits/selecttype.h>

#include "cconvert.h"
#include "type.h"
#include "literalencoding.h"


namespace ulib
{
    namespace detail
    {
        template <class AllocatorTy, class Allocator2T>
        struct SelectAllocator
        {
            using AllocatorT = AllocatorTy;
        };

        template <class Allocator2T>
        struct SelectAllocator<void, Allocator2T>
        {
            using AllocatorT = Allocator2T;
        };

        /*
        template<class EncodingT, class SourceEncodingT, class AllocatorT>
        inline ulib::EncodedString<EncodingT, AllocatorT> ParentCompatibleConvert(typename SourceEncodingT::CharT* str, size_t size, typename AllocatorT::Params al = {})
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return ulib::EncodedString<EncodingT, AllocatorT>((DestCharT *)str, (DestCharT *)str + size, al);
        }

        template<class EncodingT, class SourceEncodingT, class AllocatorT>
        inline ulib::EncodedString<EncodingT, AllocatorT> ParentCompatibleConvert(typename SourceEncodingT::CharT* str, typename AllocatorT::Params al = {})
        {
            return ParentCompatibleConvert(str, CStringLengthHack(str), al);
        }
        */

    }

    

    template <class FromEncodingT, class ToEncodingT, class AllocatorT = DefaultAllocator>
    inline EncodedString<ToEncodingT, AllocatorT> ConvertImpl(EncodedStringView<FromEncodingT> str, typename AllocatorT::Params al = {})
    {
        static_assert(FromEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");
        static_assert(ToEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");

        EncodedString<ToEncodingT, AllocatorT> result(str.size() * 4, al);

        auto end = ulib::ConvertChars<FromEncodingT, ToEncodingT>(str.data(), str.data() + str.size(), result.data());
        result.SetSize(end - result.data());

        return result;
    }

    /*
    
    template <class FromEncodingT, class ToEncodingT, class AllocatorT = DefaultAllocator, class InputAllocatorT>
    inline EncodedString<ToEncodingT, AllocatorT>&& ConvertImpl(EncodedString<FromEncodingT, InputAllocatorT>&& str, typename AllocatorT::Params al = {})
    {
        static_assert(FromEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");
        static_assert(ToEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");

        EncodedString<ToEncodingT, AllocatorT> result(str.size() * 4, al);

        auto end = ulib::ConvertChars<FromEncodingT, ToEncodingT>(str.data(), str.data() + str.size(), result.data());
        result.SetSize(end - result.data());

        return std::move(result);
    }

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, // user defined types
              class EncodingT, class AllocatorT, // auto defined types
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>>      
    inline EncodedString<OutputEncodingT, OutputAllocatorT> Convert(EncodedString<EncodingT, AllocatorT> &&str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (IsParentCompatible<EncodingT, OutputEncodingT>)
        {
            using ParentEncodingT = typename EncodingT::ParentEncodingT;
            using DestCharT = typename OutputEncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return EncodedString<OutputEncodingT, OutputAllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);


            if constexpr (std::is_same_v<AllocatorT, OutputAllocatorT>)
            {
                return std::move(str);
            }
            else
            {
                return EncodedString<OutputEncodingT, OutputAllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);
            }
            
            
        }
        else
        {
            return ConvertImpl<EncodingT, OutputEncodingT, OutputAllocatorT>(str, al);
        }
    }
*/
    

    template <class UOutputEncodingT = void, class UOutputAllocatorT = void, // user defined types
              class EncodingT, class AllocatorT, // auto defined types
              class OutputEncodingT = SelectTypeT<UOutputEncodingT, EncodingT>,
              class OutputAllocatorT = SelectTypeT<UOutputAllocatorT, AllocatorT>>      
    inline EncodedString<OutputEncodingT, OutputAllocatorT> Convert(const EncodedString<EncodingT, AllocatorT> &str, typename OutputAllocatorT::Params al = {})
    {
        if constexpr (IsParentCompatible<EncodingT, OutputEncodingT>)
        {
            using ParentEncodingT = typename EncodingT::ParentEncodingT;
            using DestCharT = typename OutputEncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return EncodedString<OutputEncodingT, OutputAllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);
        }
        else
        {
            return ConvertImpl<EncodingT, OutputEncodingT, OutputAllocatorT>(str, al);
        }
    }

    template <class EncodingT, class AllocatorT = DefaultAllocator, class SourceEncodingT>
    inline EncodedString<EncodingT, AllocatorT> Convert(EncodedStringView<SourceEncodingT> str, typename AllocatorT::Params al = {})
    {
        if constexpr (IsParentCompatible<SourceEncodingT, EncodingT>)
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return EncodedString<EncodingT, AllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);
        }
        else
        {
            return ConvertImpl<SourceEncodingT, EncodingT, AllocatorT>(str, al);
        }
    }

    template <class EncodingT, class AllocatorT = DefaultAllocator, class CharT, class SourceEncodingT = LiteralEncodingT<CharT>, std::enable_if_t<!std::is_same_v<SourceEncodingT, void>, bool> = true>
    inline EncodedString<EncodingT, AllocatorT> Convert(const CharT *str, typename AllocatorT::Params al = {})
    {
        if constexpr (IsParentCompatible<SourceEncodingT, EncodingT>)
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return EncodedString<EncodingT, AllocatorT>((DestCharT *)str, al);
        }
        else
        {
            return ConvertImpl<SourceEncodingT, EncodingT, AllocatorT>(str, al);
        }
    }

#ifdef ULIB_USE_STD_STRING_VIEW

    template <class EncodingT, class AllocatorT = DefaultAllocator, class CharT, class SourceEncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<SourceEncodingT, void>, bool> = true>
    inline EncodedString<EncodingT, AllocatorT> Convert(const std::basic_string<CharT> &str, typename AllocatorT::Params al = {})
    {
        if constexpr (IsParentCompatible<SourceEncodingT, EncodingT>)
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return EncodedString<EncodingT, AllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);
        }
        else
        {
            return ConvertImpl<SourceEncodingT, EncodingT, AllocatorT>(str, al);
        }
    }

    template <class EncodingT, class AllocatorT = DefaultAllocator, class CharT, class SourceEncodingT = LiteralEncodingT<CharT>,
              std::enable_if_t<!std::is_same_v<SourceEncodingT, void>, bool> = true>
    inline EncodedString<EncodingT, AllocatorT> Convert(std::basic_string_view<CharT> str, typename AllocatorT::Params al = {})
    {
        if constexpr (IsParentCompatible<SourceEncodingT, EncodingT>)
        {
            using ParentEncodingT = typename SourceEncodingT::ParentEncodingT;
            using DestCharT = typename EncodingT::CharT;
            static_assert(sizeof(typename ParentEncodingT::CharT) == sizeof(DestCharT),
                          "Parent encoding character size must be equal with base encoding to compatibile convertation");

            return EncodedString<EncodingT, AllocatorT>((DestCharT *)str.data(), (DestCharT *)str.data() + str.size(), al);
        }
        else
        {
            return ConvertImpl<SourceEncodingT, EncodingT, AllocatorT>(str, al);
        }
    }

#endif

}
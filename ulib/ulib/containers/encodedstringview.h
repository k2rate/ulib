#pragma once

#include <ulib/config.h>
#include "stringview.h"

namespace ulib
{
    template <class EncodingTy>
    class EncodedStringView : public BasicStringView<typename EncodingTy::CharT>
    {
    public:
        using BaseT = BasicStringView<typename EncodingTy::CharT>;
        using EncodingT = EncodingTy;
        using CharT = typename EncodingT::CharT;
        using value_type = CharT;

        EncodedStringView()
            : BasicStringView<CharT>()
        {
        }

        EncodedStringView(const CharT *str)
            : BasicStringView<CharT>(str)
        {
        }

        EncodedStringView(const CharT *b, const CharT *e)
            : BasicStringView<CharT>(b, e)
        {
        }

        EncodedStringView(const CharT *str, size_t size)
            : BasicStringView<CharT>(str, size)
        {
        }

        EncodedStringView(const EncodedStringView<EncodingT> &str)
            : BasicStringView<CharT>(str)
        {
        }

        template <class StringT, class SCharT = typename StringT::value_type,
                  std::enable_if_t<std::is_same_v<SCharT, CharT>, bool> = true>
        EncodedStringView(const StringT &str)
            : BasicStringView<SCharT>(str)
        {
        }

        ~EncodedStringView()
        {
        }

#ifdef ULIB_USE_STD_STRING_VIEW

        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(this->mBegin, this->mLast);
        }

        operator std::basic_string<CharT>() const
        {
            return std::basic_string<CharT>(this->mBegin, this->mLast);
        }

#ifdef __cpp_char8_t
        // template <class CurrentCharT = typename EncodingT::CharStd, std::enable_if_t<std::is_same_v<ParentEncodingT, MultibyteEncoding>, bool> = true>
        operator std::basic_string_view<typename EncodingT::CharStd>() const
        {
            return std::basic_string_view<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin, (typename EncodingT::CharStd *)this->mLast);
        }

        operator std::basic_string<typename EncodingT::CharStd>() const
        {
            return std::basic_string<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin, (typename EncodingT::CharStd *)this->mLast);
        }

#endif
#endif

        /*
        inline bool operator==(Range<CharT> right) const
        {
            return BaseT::Equal(right);
        }
        */
        };
}
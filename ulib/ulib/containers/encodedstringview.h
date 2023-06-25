#pragma once

#include <ulib/config.h>

#include "clength.h"
#include "range.h"

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string>
#include <string_view>
#endif

#include <ulib/typetraits/string.h>

namespace ulib
{
    template <class EncodingTy>
    class EncodedStringView : public Range<const typename EncodingTy::CharT>
    {
    public:
        using EncodingT = EncodingTy;
        using CharT = typename EncodingT::CharT;
        using BaseT = Range<const CharT>;
        using SelfT = EncodedStringView<EncodingTy>;

        using Iterator = typename BaseT::Iterator;
        using ConstIterator = typename BaseT::ConstIterator;

        using value_type = CharT;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        using ContainerTagT = string_container_tag;

        EncodedStringView() : BaseT() {}

        EncodedStringView(const CharT *str) : BaseT(str, CStringLengthHack(str)) {}

        EncodedStringView(const CharT *b, const CharT *e) : BaseT(b, e) {}

        EncodedStringView(ConstIterator b, ConstIterator e) : BaseT(b.ptr, e.ptr) {}

        EncodedStringView(const CharT *str, size_t size) : BaseT(str, size) {}

        EncodedStringView(const EncodedStringView<EncodingT> &source) : BaseT(source) {}

        template <class StringT, class SCharT = typename StringT::value_type,
                  std::enable_if_t<std::is_same_v<SCharT, CharT>, bool> = true>
        EncodedStringView(const StringT &str) : BaseT(str)
        {
        }

        ~EncodedStringView() {}

#ifdef ULIB_USE_STD_STRING_VIEW

        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(this->mBegin, this->Size());
        }

        operator std::basic_string<CharT>() const { return std::basic_string<CharT>(this->mBegin, this->mLast); }

#ifdef __cpp_char8_t
        // template <class CurrentCharT = typename EncodingT::CharStd, std::enable_if_t<std::is_same_v<ParentEncodingT,
        // MultibyteEncoding>, bool> = true>
        operator std::basic_string_view<typename EncodingT::CharStd>() const
        {
            return std::basic_string_view<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin,
                                                                       this->Size());
        }

        operator std::basic_string<typename EncodingT::CharStd>() const
        {
            return std::basic_string<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin,
                                                                  (typename EncodingT::CharStd *)this->mLast);
        }

#endif
#endif

        inline bool operator==(const CharT *right) const
        {
            return BaseT::Equal(BaseT(right, CStringLengthHack(right)));
        }

        inline bool operator==(SelfT right) const { return BaseT::Equal(right); }

        inline bool operator!=(const CharT *right) const
        {
            return !BaseT::Equal(BaseT(right, CStringLengthHack(right)));
        }

        inline bool operator!=(SelfT right) const { return !BaseT::Equal(right); }
    };

    template <class EncodingT, class CharT>
    inline bool operator==(const CharT *left, ulib::EncodedStringView<EncodingT> str)
    {
        return str == left;
    }

    template <class EncodingT, class CharT>
    inline bool operator!=(const CharT *left, ulib::EncodedStringView<EncodingT> str)
    {
        return str != left;
    }
} // namespace ulib

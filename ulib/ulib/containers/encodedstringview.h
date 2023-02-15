#pragma once

#include "stringview.h"

namespace ulib
{
    template <class EncodingTy>
    class EncodedStringView : public BasicStringView<typename EncodingTy::CharT>
    {
    public:
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
    };
}
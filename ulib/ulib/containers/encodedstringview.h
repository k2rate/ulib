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

        EncodedStringView(const EncodedStringView<EncodingT> &str)
            : BasicStringView<CharT>(str)
        {
        }

        template <class StringT, class CharT = typename StringT::value_type>
        EncodedStringView(const StringT &str)
            : BasicStringView<CharT>(str)
        {
        }

        ~EncodedStringView()
        {
        }
    };
}
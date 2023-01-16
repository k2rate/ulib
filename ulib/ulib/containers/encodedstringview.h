#pragma once

#include "stringview.h"

namespace ulib
{
    template <class EncodingT>
    class EncodedStringView : public BasicStringView<typename EncodingT::CharT>
    {
    public:
        using CharT = typename EncodingT::CharT;
        using CharAliasT = typename EncodingT::CharAliasT;
        using value_type = CharT;

        EncodedStringView()
            : BasicStringView<CharT>()
        {
        }

        EncodedStringView(const CharAliasT *str)
            : BasicStringView<CharT>((const CharT *)str)
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
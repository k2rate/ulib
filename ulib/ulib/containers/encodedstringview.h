#pragma once

#include "stringview.h"

namespace ulib
{
    template<class EncodingT>
    struct EncodedStringView : public BasicStringView<typename EncodingT::CharT>
    {
        using CharT = typename EncodingT::CharT;
        using CharAliasT = typename EncodingT::CharAliasT;

        EncodedStringView()
            : BasicStringView<CharT>()
        {
        }

        EncodedStringView(const CharAliasT *str)
            : BasicStringView<CharT>((const CharT*)str)
        {
        }

        
        template <class StringT>
        EncodedStringView(const StringT &str)
            : BasicStringView<CharT>(str)
        {
        }
        
        

        ~EncodedStringView()
        {
        }
    };
}
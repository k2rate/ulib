#pragma once

#include <ulib/types.h>

namespace ulib
{
    template <class FromEncodingT, class ToEncodingT>
    typename ToEncodingT::CharT *Convert(const typename FromEncodingT::CharT *begin, const typename FromEncodingT::CharT *end,
                                         typename ToEncodingT::CharT *output)
    {
        auto it = begin;
        while (it != end)
        {
            uint codepoint;

            it = FromEncodingT::Decode(it, end, codepoint);
            output = ToEncodingT::Encode(codepoint, output);
        }

        return output;
    }
}
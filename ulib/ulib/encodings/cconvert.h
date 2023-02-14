#pragma once

#include <ulib/types.h>

#include "type.h"

namespace ulib
{
    template <class FromEncodingT, class ToEncodingT>
    typename ToEncodingT::CharT *Convert(const typename FromEncodingT::CharT *begin, const typename FromEncodingT::CharT *end,
                                         typename ToEncodingT::CharT *output)
    {
        static_assert(FromEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");
        static_assert(ToEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");

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
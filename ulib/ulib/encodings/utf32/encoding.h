#pragma once

#include <ulib/types.h>
#include "../uchars.h"

namespace ulib
{
    struct Utf32
    {
        using Type = uint;

        // BasicString main char
        using CharT = u32char;
        
        // EncodingString constructor type 
        using CharAliasT = char32_t;

        inline static CharT *Encode(uint codepoint, CharT *out)
        {
            *out = codepoint;
            return out + 1;
        }

        inline static const CharT *Decode(const CharT *begin, const CharT *end, uint &out)
        {
            out = *begin;
            return begin + 1;
        }
    };
}

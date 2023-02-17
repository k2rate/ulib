#pragma once

#include "../type.h"
#include "../nullencoding.h"

namespace ulib
{
    struct MultibyteEncoding
    {
        using CharT = char;
        using ParentEncodingT = NullEncoding<CharT>;
        constexpr static EncodingType kType = EncodingType::Raw;
        constexpr static EncodingCharType kCharType = EncodingCharType::SingleByte;
        
#ifdef __cpp_char8_t
        using CharStd = int;
#endif
    };
}
#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstring.h>

namespace ulib
{
    using Utf32String = EncodedString<Utf32, DefaultAllocator>;
    using u32string = Utf32String;
}
#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstring.h>

namespace ulib
{
    using Utf16String = EncodedString<Utf16, DefaultAllocator>;
    using u16string = Utf16String;
}
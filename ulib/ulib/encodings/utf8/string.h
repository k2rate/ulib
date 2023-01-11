#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstring.h>

namespace ulib
{
    using Utf8String = EncodedString<Utf8, DefaultAllocator>;
    using u8string = Utf8String;
}
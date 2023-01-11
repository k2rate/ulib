#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstring.h>

namespace ulib
{
    using PlatformString = EncodedString<PlatformEncoding, DefaultAllocator>;
    using plstring = PlatformString;
}
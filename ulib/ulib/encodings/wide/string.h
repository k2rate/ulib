#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstring.h>

namespace ulib
{
    using WideString = EncodedString<WideEncoding, DefaultAllocator>;
    using wstring = WideString;
}
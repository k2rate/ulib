#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstring.h>

namespace ulib
{
    using String = EncodedString<MultibyteEncoding, DefaultAllocator>;
    using string = String;
}
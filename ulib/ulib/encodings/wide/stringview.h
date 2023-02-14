#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstringview.h>

namespace ulib
{
    using WideStringView = EncodedStringView<WideEncoding>;
    using wstring_view = WideStringView;
}
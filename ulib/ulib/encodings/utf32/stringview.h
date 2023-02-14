#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstringview.h>

namespace ulib
{
    using Utf32StringView = EncodedStringView<Utf32>;
    using u32string_view = Utf32StringView;
}
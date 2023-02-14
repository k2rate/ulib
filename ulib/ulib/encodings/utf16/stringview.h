#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstringview.h>

namespace ulib
{
    using Utf16StringView = EncodedStringView<Utf16>;
    using u16string_view = Utf16StringView;
}
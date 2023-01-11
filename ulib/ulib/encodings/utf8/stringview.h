#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstringview.h>

namespace ulib
{
    using Utf8StringView = EncodedStringView<Utf8>;
    using u8string_view = Utf8StringView;
}
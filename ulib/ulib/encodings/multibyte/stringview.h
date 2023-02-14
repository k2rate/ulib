#pragma once

#include "encoding.h"
#include <ulib/containers/encodedstringview.h>

namespace ulib
{
    using StringView = EncodedStringView<MultibyteEncoding>;
    using string_view = StringView;
}
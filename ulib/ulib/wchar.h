#pragma once

#include <ulib/encodings/wide/string.h>
#include <ulib/encodings/wide/stringview.h>
#include <ulib/encodings/convert.h>

namespace ulib
{
    // convert any string to wstring
    template <class StringT, class EncodingT = typename StringT::EncodingT>
    inline wstring wstr(const StringT &str)
    {
        return Convert<EncodingT, WideEncoding>(str);
    }
}
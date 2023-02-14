#pragma once

#include <type_traits>

#include <ulib/encodings/multibyte/string.h>
#include <ulib/encodings/multibyte/stringview.h>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#include <string>
#endif

namespace ulib
{
    template <class StringT, class EncodingT = typename StringT::EncodingT,
              std::enable_if_t<sizeof(typename String::CharT) == sizeof(typename EncodingT::CharT), bool> = true>
    inline string str(const StringT &str)
    {
        using CharT = typename String::CharT;
        return string((CharT *)str.data(), (CharT *)str.data() + str.size());
    }
}
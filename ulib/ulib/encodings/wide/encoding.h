#pragma once

#include "../type.h"

#ifdef _WIN32
#include "../utf16/encoding.h"
namespace ulib
{
    using SystemWideEncoding = Utf16;
}
#else
#include "../utf32/encoding.h"
namespace ulib
{
    using SystemWideEncoding = Utf32;
}
#endif

namespace ulib
{
    namespace detail
    {
        using WideChar = typename ulib::SystemWideEncoding::CharT;
    }

    static_assert(sizeof(detail::WideChar) == sizeof(wchar_t), "[ulib error] wchar_t incompatible with ulib::SystemWideEncoding");

    struct WideEncoding
    {
        using CharT = wchar_t;
        using ParentEncodingT = ulib::NullEncoding<CharT>;
        constexpr static EncodingType kType = EncodingType::Concrete;
#ifdef __cpp_char8_t
        using CharStd = int;
#endif
        inline static CharT *Encode(uint codepoint, CharT *out)
        {
            return (CharT *)ulib::SystemWideEncoding::Encode(codepoint, (detail::WideChar *)out);
        }

        inline static const CharT *Decode(const CharT *begin, const CharT *end, uint &out)
        {
            return (CharT *)ulib::SystemWideEncoding::Decode((detail::WideChar *)begin, (detail::WideChar *)end, out);
        }
    };
}
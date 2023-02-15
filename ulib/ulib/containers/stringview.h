#pragma once

#include "range.h"
#include "clength.h"

namespace ulib
{
    template <class CharT>
    class BasicStringView : public Range<const CharT>
    {
    public:
        using value_type = CharT;

        BasicStringView()
            : Range<const CharT>()
        {
        }

        BasicStringView(const CharT *str)
            : Range<const CharT>(str, str + CStringLengthHack(str))
        {
        }

        BasicStringView(const CharT *b, const CharT *e)
            : Range<const CharT>(b, e)
        {
        }

        BasicStringView(const CharT *str, size_t size)
            : Range<const CharT>(str, str + size)
        {
        }

        template <class StringT, class CharT = typename StringT::value_type>
        BasicStringView(const StringT &str)
            : Range<const CharT>(str)
        {
        }

        ~BasicStringView()
        {
        }
    };

    /*
        using StringView = BasicStringView<char>;
    using WideStringView = BasicStringView<wchar_t>;

    using string_view = StringView;
    using wstring_view = WideStringView;
    */

}
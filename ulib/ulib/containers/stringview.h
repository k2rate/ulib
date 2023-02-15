#pragma once

#include "range.h"
#include "clength.h"

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#endif

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

        template <class StringT, class SCharT = typename StringT::value_type,
                  std::enable_if_t<std::is_same_v<SCharT, CharT>, bool> = true>
        BasicStringView(const StringT &str)
            : Range<const SCharT>(str)
        {
        }

        ~BasicStringView()
        {
        }

#ifdef ULIB_USE_STD_STRING_VIEW
        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(this->mBegin, this->Size());
        }
#endif
        /*
        inline bool operator==(Range<const CharT> right) const
        {
            return BaseT::Equal(right);
        }
        */
        
    };

    /*
        using StringView = BasicStringView<char>;
    using WideStringView = BasicStringView<wchar_t>;

    using string_view = StringView;
    using wstring_view = WideStringView;
    */

}
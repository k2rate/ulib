#pragma once

#include <ulib/containers/string.h>

namespace ulib
{
    template <class EncodingT, class AllocatorT = DefaultAllocator>
    class EncodedString : public BasicString<typename EncodingT::CharT, AllocatorT>
    {
        using AllocatorParams = typename AllocatorT::Params;
        using CharT = typename EncodingT::CharT;
        using CharAliasT = typename EncodingT::CharAliasT;
        using BaseT = BasicString<typename EncodingT::CharT, AllocatorT>;

    public:
        inline EncodedString(AllocatorParams al = {})
            : BasicString<CharT, AllocatorT>(al)
        {
        }

        inline EncodedString(size_t capacity, AllocatorParams al = {})
            : BasicString<CharT, AllocatorT>(capacity, al)
        {
        }

        inline EncodedString(const CharAliasT *str, AllocatorParams al = {})
            : BasicString<CharT, AllocatorT>((CharT *)str, al)
        {
        }

        inline EncodedString(const BasicString<CharT, AllocatorT> &str)
            : BasicString<CharT, AllocatorT>(str)
        {
        }

        inline bool Equal(const CharAliasT *cstr) const { return BaseT::Equal((const CharT *)cstr); }

        inline bool operator==(const CharAliasT *right) const { return Equal(right); }
        inline bool operator!=(const CharAliasT *right) const { return !Equal(right); }
    };
}
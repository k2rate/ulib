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

        inline EncodedString(const BasicString<CharT, AllocatorT>& str)
            : BasicString<CharT, AllocatorT>(str)
        {
        }

    };
}
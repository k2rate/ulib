#pragma once

namespace ulib
{
    struct DefaultTag
    {
    };

    template <class TagT>
    struct Tag
    {
    };

    template <size_t Index>
    struct IndexedTag
    {
    };

    template <class TagT, size_t Index>
    struct MultiTag
    {
    };
}
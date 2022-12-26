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

    template <class T, class TagT = DefaultTag>
    struct Singleton
    {
        template <class... Args>
        static T &Instance(Args &&...args)
        {
            static T o(args...);
            return o;
        }
    };
}
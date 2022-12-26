#pragma once

namespace ulib
{
    class DefaultTag
    {
    };

    template <class TagT>
    class Tag
    {
    };

    template <size_t Index>
    class IndexedTag
    {
    };

    template <class TagT, size_t Index>
    class MultiTag
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
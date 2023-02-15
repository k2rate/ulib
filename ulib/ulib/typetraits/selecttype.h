#pragma once

namespace ulib
{
    template <class T, class DefaultT>
    struct SelectType
    {
        using Type = T;
    };

    template <class DefaultT>
    struct SelectType<void, DefaultT>
    {
        using Type = DefaultT;
    };

    template <class T, class DefaultT>
    using SelectTypeT = typename SelectType<T, DefaultT>::Type;
}
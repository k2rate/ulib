#pragma once

#include <ulib/typetraits/missingtype.h>

namespace ulib
{
    template <class T, class DefaultT>
    struct select_type
    {
        using type = T;
    };

    template <class DefaultT>
    struct select_type<ulib::missing_type, DefaultT>
    {
        using type = DefaultT;
    };

    template <class T, class DefaultT>
    using select_type_t = typename select_type<T, DefaultT>::type;
}
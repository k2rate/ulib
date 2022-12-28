#pragma once

#include "tags.h"

namespace ulib
{
    template <class T, class TagT = DefaultTag>
    struct Singleton
    {
        template <class... Args>
        inline static T &Instance(Args &&...args)
        {
            static T o(args...);
            return o;
        }
    };
}
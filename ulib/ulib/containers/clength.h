#pragma once

#include <ulib/types.h>

namespace ulib
{
    template <class CharT>
    static constexpr size_t CStringLengthHack(const CharT *ptr) noexcept /* strengthened */
    {
        // find length of null-terminated sequence
        size_t len = 0;
        while (*ptr != 0)
        {
            ++len;
            ++ptr;
        }

        return len;
    }
}
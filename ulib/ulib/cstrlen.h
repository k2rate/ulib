#pragma once

#include <cstddef>

namespace ulib
{
    template <class CharT>
    static constexpr size_t cstrlen(const CharT *ptr) noexcept
    {
        size_t len = 0;
        while (*ptr != 0)
            ++len, ++ptr;
        return len;
    }
} // namespace ulib
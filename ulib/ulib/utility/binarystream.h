#pragma once

#include <ulib/types.h>

namespace ulib
{
    class BinaryReadStream
    {
    public:

        BinaryReadStream()
        {

        }

        ~BinaryReadStream()
        {

        }

        

    private:
        rawptr_t mIt;
        rawptr_t mEnd;
    };
}
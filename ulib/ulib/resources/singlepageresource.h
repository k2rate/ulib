#pragma once

#include "resource.h"

#ifdef _DEBUG
#include <assert.h>
#endif

namespace ulib
{
    template <class AllocatorT>
    class SinglePageResource : public Resource<AllocatorT>
    {
    public:
#ifdef _DEBUG
        inline void Free(void *ptr)
        {
            
        }
#endif
    };
}
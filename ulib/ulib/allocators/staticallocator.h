#pragma once

namespace ulib
{
    class DefaultAllocatorPlaceholder {};

    template <class AllocatorT, int iIndex = 0, class Placeholder = DefaultAllocatorPlaceholder>
    class StaticAllocator
    {
    public:
        using Params = typename AllocatorT::Params;

        static AllocatorT &Instance(typename AllocatorT::Params *params = nullptr)
        {
            static AllocatorT *ptr = nullptr;
            if (ptr)
                return *ptr;

            if (params)
            {
                static AllocatorT al(*params);
                ptr = &al;
            }
            else
            {
                static AllocatorT al;
                ptr = &al;
            }

            return *ptr;
        }

        StaticAllocator()
        {
            Instance();
        }

        StaticAllocator(typename AllocatorT::Params params)
        {
            Instance(&params);
        }

        StaticAllocator(StaticAllocator&& source)
        {

        }

        StaticAllocator& operator=(StaticAllocator&& source)
        {
            return *this;
        }

        ~StaticAllocator()
        {
        }

        static void *Alloc(size_t size)
        {
            return Instance().Alloc(size);
        }

        static void Free(void *ptr)
        {
            Instance().Free(ptr);
        }
    };


}
#pragma once

#include <ulib/typetraits/singleton.h>

namespace ulib
{
    template <class TagT>
    struct AllocatorTag
    {
    };

    template <class AllocatorT, class TagT = AllocatorTag<DefaultTag>>
    class StaticAllocator
    {
    public:
        using Current = Singleton<AllocatorT, TagT>;
        using Params = typename AllocatorT::Params;

        template <class... Args>
        static AllocatorT &Instance(Args &&...args) { return Current::Instance(args...); }

        StaticAllocator() { Instance(); }
        StaticAllocator(Params params) { Instance(params); }
        StaticAllocator(StaticAllocator &&source) {}
        ~StaticAllocator() {}

        StaticAllocator &operator=(StaticAllocator &&source) { return *this; }

        static void *Alloc(size_t size) { return Instance().Alloc(size); }
        static void Free(void *ptr) { Instance().Free(ptr); }
    };

}
#pragma once

#include <assert.h>
#include <typeinfo>

namespace ulib
{
    template <class T, class AllocatorT>
    class StandardAllocator : public AllocatorT
    {
    public:
        using value_type = T;

        template <class T>
        struct rebind
        {
            using other = StandardAllocator<T, AllocatorT>;
        };

        StandardAllocator(AllocatorT::Params params = {})
            : AllocatorT(params)
        {
        }

        template <class U>
        StandardAllocator(const StandardAllocator<U, AllocatorT> &other)
        {
            /*
            if (!std::is_same<T, U>::value)
                throw std::exception(typeid(T).name());
            */
            
        }

        void deallocate(T *_Ptr, size_t _Count)
        {
            assert(_Ptr);
            assert(_Count);

            AllocatorT::Free(_Ptr);
        }

        T *allocate(size_t _Count)
        {
            assert(_Count);

            return (T *)AllocatorT::Alloc(_Count * sizeof(T));
        }
    };
}
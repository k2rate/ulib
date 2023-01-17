#pragma once

#include <type_traits>
#include <cstring> // memcpy

#include <ulib/types.h>
#include <ulib/allocators/defaultallocator.h>
#include <ulib/resources/resource.h>

#include "iterators/randomaccessiterator.h"
#include "args.h"
#include "range.h"

namespace ulib
{
    template <class T, class AllocatorT = DefaultAllocator>
    class BasicBuffer : public Resource<AllocatorT>
    {
    public:
        constexpr static size_t C_STEP = 16;
        constexpr static size_t M_STEP = sizeof(T) * C_STEP;

        using Iterator = RandomAccessIterator<T>;
        using ConstIterator = RandomAccessIterator<const T>;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using AllocatorParams = typename AllocatorT::Params;

        inline BasicBuffer(AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(M_STEP))) + M_STEP;
        }

        template <class ContainerT, class vt = typename ContainerT::value_type, class RangeT = Range<const typename ContainerT::value_type>, std::enable_if_t<std::is_same_v<T, std::remove_cv_t<vt>>, bool> = true>
        inline BasicBuffer(const ContainerT &cont, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            RangeT str = cont;

            size_t allocSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str.Data(), allocSize);
        }

        inline BasicBuffer(size_t size, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(T) * size;
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;
        }

        inline BasicBuffer(args::Capacity capacity, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(T) * capacity.cc;
            mEndB = (mLastB = mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;
        }

        inline BasicBuffer(const T *str, size_t size, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = size * sizeof(T);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        inline BasicBuffer(const T *b, const T *e, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = (e - b) * sizeof(T);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, b, allocSize);
        }

        inline BasicBuffer(const BasicBuffer<T, AllocatorT> &str)
            : Resource<AllocatorT>(str)
        {
            size_t dataSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(dataSize)) + dataSize;
            memcpy(mBeginB, str.Data(), dataSize);
        }

        inline BasicBuffer(BasicBuffer<T, AllocatorT> &&source)
            : Resource<AllocatorT>(std::move(source))
        {
            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

        inline ~BasicBuffer()
        {
            if (mBegin)
            {
                AllocatorT::Free(mBegin);
            }
        }

        inline T *Data() { return mBegin; }
        inline T *Data() const { return mBegin; }
        inline iterator Begin() { return mBegin; }
        inline iterator End() { return mLast; }
        inline const_iterator Begin() const { return mBegin; }
        inline const_iterator End() const { return mLast; }
        inline iterator ReverseBegin() { return mLast - 1; }
        inline iterator ReverseEnd() { return mBegin - 1; }
        inline const_iterator ReverseBegin() const { return mLast - 1; }
        inline const_iterator ReverseEnd() const { return mBegin - 1; }
        inline bool Empty() const { return mBegin == mLast; }

        inline size_t Size() const { return mLast - mBegin; }
        inline size_t Capacity() const { return mEnd - mBegin; }
        inline size_t Available() const { return mEnd - mLast; }
        inline size_t SizeInBytes() const { return mLastB - mBeginB; }
        inline size_t CapacityInBytes() const { return mEndB - mBeginB; }
        inline size_t AvailableInBytes() const { return mEnd - mLast; }
        inline void SetSize(size_t newSize) { mLast = mBegin + newSize; }

        inline iterator begin() { return mBegin; }
        inline iterator end() { return mLast; }
        inline const_iterator begin() const { return mBegin; }
        inline const_iterator end() const { return mLast; }
        inline size_t size() const { return Size(); }
        inline size_t capacity() const { return Capacity(); }
        inline T *data() { return Data(); }
        inline const T *data() const { return Data(); }
        inline void erase(iterator it) { Erase(it); }
        inline void push_back(const T &o) { PushBack(o); }
        inline void push_back(T &&o) { PushBack(std::move(o)); }
        inline void pop_back() { PopBack(); }
        inline bool empty() const { return Empty(); }
        inline size_t reserve(size_t s) { Reserve(s); }

        inline void Reserve(size_t s)
        {
            if (Capacity() >= s)
                return;

            uchar *newMem = (uchar *)AllocatorT::Alloc(s);

            T *oldBegin = mBegin;
            size_t oldSizeInBytes = SizeInBytes();

            mEndB = (mLastB = mBeginB = newMem) + s;
            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            AllocatorT::Free(oldBegin);
        }

        inline void Erase(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::Erase");
            assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::Erase");

            T *from = it.ptr + 1;
            ::memcpy(it.ptr, from, mLastB - (uchar *)from);
            mLast--;
        }

        inline void Erase(size_t i) { Erase(mBegin + i); }
        inline int Index(const_iterator it) const { return int(it.ptr - mBegin); }
        inline int Index(iterator it) const { return int(it.ptr - mBegin); }

        inline void FastErase(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::FastErase");
            assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::FastErase");

            T *pBack = mLast - 1;
            if (it.ptr != pBack)
            {
                // c++ fan version
                // new (it.ptr) T(std::move(*pBack));

                // right version
                memcpy(it.ptr, pBack, sizeof(T));
            }

            mLast--;
        }

        inline void PopBack()
        {
            assert(Size() && "Attempt pop element in empty list in List<T>::Pop");
            mLast--;
        }

        inline void Pop() { PopBack(); }

        template <class LAllocatorT>
        inline void Assign(const BasicBuffer<T, LAllocatorT> &source)
        {
            size_t allocSize = source.CapacityInBytes();
            size_t requiredSize = source.SizeInBytes();

            if (CapacityInBytes() < requiredSize)
            {
                AllocatorT::Free(mBegin);
                mEndB = (mBeginB = rawptr_t(AllocatorT::Alloc(allocSize))) + allocSize;
            }

            mLastB = mBeginB + requiredSize;
            memcpy(mBegin, mLast, requiredSize);
        }

        inline void Assign(BasicBuffer<T, AllocatorT> &&source)
        {
            AllocatorT::Free(mBeginB);

            Resource<AllocatorT> &rs = *this;
            rs = std::move(source);

            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

        inline bool Equal(ulib::Range<const T> right) const
        {
            size_t sizeInBytes = right.SizeInBytes();
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, right.Data(), sizeInBytes) == 0;
        }

        template <class LAllocatorT>
        inline bool Equal(const BasicBuffer<T, LAllocatorT> &right) const
        {
            size_t sizeInBytes = right.SizeInBytes();
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, right.mBeginB, sizeInBytes) == 0;
        }

        template <class ContainerT, class vt = typename ContainerT::value_type, class RangeT = Range<const typename ContainerT::value_type>, std::enable_if_t<std::is_same_v<T, std::remove_cv_t<vt>>, bool> = true>
        inline bool operator==(const ContainerT &right) const
        {
            RangeT str = right;
            return Equal(str);
        }

        template <class LAllocatorT>
        inline bool operator==(const BasicBuffer<T, LAllocatorT> &right) const { return Equal(right); }
        template <class LAllocatorT>
        inline bool operator!=(const BasicBuffer<T, LAllocatorT> &right) const { return !Equal(right); }

        inline BasicBuffer<T, AllocatorT> &operator=(const BasicBuffer<T, AllocatorT> &source)
        {
            Assign(source);
            return *this;
        }

        template <class LAllocatorT>
        inline BasicBuffer<T, AllocatorT> &operator=(const BasicBuffer<T, LAllocatorT> &source)
        {
            Assign(source);
            return *this;
        }

        inline BasicBuffer<T, AllocatorT> &operator=(BasicBuffer<T, AllocatorT> &&source)
        {
            Assign(std::move(source));
            return *this;
        }

        template <class LAllocatorT>
        inline void Append(const BasicBuffer<T, LAllocatorT> &right)
        {
            size_t sizeInBytes = SizeInBytes();
            size_t rightSizeInBytes = right.SizeInBytes();

            size_t reqSize = sizeInBytes + rightSizeInBytes;
            if (Capacity() < reqSize)
            {
                ReallocateMemory(sizeInBytes, reqSize);
            }

            memcpy(mLast, right.Data(), right.SizeInBytes());
            mLast = mBegin + reqSize;
        }

        inline void PushBack(T ch)
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, sizeInBytes);
            }

            *mLast = ch;
            mLast++;
        }

    protected:
        inline void ReallocateMemory(size_t currentSizeInBytes, size_t additionalSizeInBytes)
        {
            size_t oldSizeInBytes = currentSizeInBytes;
            size_t allocSizeInBytes = oldSizeInBytes + additionalSizeInBytes;
            T *oldBegin = mBegin;

            mEndB = (mLastB = mBeginB = (rawptr_t)AllocatorT::Alloc(allocSizeInBytes)) + allocSizeInBytes;

            assert(mBegin && "Out of memory in List<T>::ReallocateMemory");

            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            AllocatorT::Free(oldBegin);
        }

        union
        {
            struct
            {
                T *mBegin;
                T *mLast;
                T *mEnd;
            };

            struct
            {
                uchar *mBeginB;
                uchar *mLastB;
                uchar *mEndB;
            };
        };
    };

	using Buffer = BasicBuffer<uchar>;
}
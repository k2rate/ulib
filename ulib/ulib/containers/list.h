#pragma once

#include <initializer_list>
#include <new>      // overloaded new / delete
#include <string.h> // memcpy

#include <ulib/allocators/defaultallocator.h>
#include <ulib/resources/resource.h>
#include <ulib/types.h>

#include "args.h"
#include "iterators/randomaccessiterator.h"
#include "range.h"

#include <assert.h>

namespace ulib
{
    enum class ExpandMemoryPolicy
    {
        Flex = 0,
        Strict = 1,
        Default = Flex
    };

    template <class T, class AllocatorT = DefaultAllocator>
    class List : public Resource<AllocatorT>
    {
        constexpr static size_t C_STEP = 16;
        constexpr static size_t M_STEP = sizeof(T) * C_STEP;

    public:
        using Iterator = RandomAccessIterator<T>;
        using ConstIterator = RandomAccessIterator<const T>;

        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        using AllocatorParams = typename AllocatorT::Params;
        constexpr static bool kTrivally = std::is_trivially_copyable_v<T>;

        inline List(AllocatorParams al = {}) : Resource<AllocatorT>(al)
        {
            mEndB = (mLastB = mBeginB = (uchar *)AllocatorT::Alloc(M_STEP)) + M_STEP;

            assert(mBegin && "Out of memory in List<T>::List()");
        }

        template <class ContainerT, class vt = typename ContainerT::value_type,
                  class RangeT = Range<const typename ContainerT::value_type>,
                  std::enable_if_t<std::is_same_v<T, std::remove_cv_t<vt>>, bool> = true>
        inline List(const ContainerT &cont, AllocatorParams al = {}) : Resource<AllocatorT>(al)
        {
            RangeT rn = cont;

            size_t allocSize = rn.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, rn.Data(), allocSize);
        }

        inline explicit List(size_t size, AllocatorParams al = {}) : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(T) * size;
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            assert(mBegin && "Out of memory in List<T>::List(size_t)");

            for (auto it = mBegin; it != mEnd; it++)
                new (it) T();
        }

        inline explicit List(args::Capacity capacity, AllocatorParams al = {}) : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(T) * capacity.cc;
            mEndB = (mLastB = mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            assert(mBegin && "Out of memory in List<T>::List(size_t)");
        }

        inline List(std::initializer_list<T> init, AllocatorParams al = {}) : Resource<AllocatorT>(al)
        {
            CopyInit(init.begin(), init.size() * sizeof(T));
        }

        template <class LAllocatorT>
        inline List(const List<T, LAllocatorT> &source, AllocatorParams al = {}) : Resource<AllocatorT>(al)
        {
            CopyInit(source);
        }

        inline List(const List<T, AllocatorT> &source) : Resource<AllocatorT>(source) { CopyInit(source); }

        inline List(List<T, AllocatorT> &&source) : Resource<AllocatorT>(std::move(source))
        {
            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

        inline ~List()
        {
            if (mBeginB)
            {
                for (T *ptr = mBegin; ptr != mLast; ptr++)
                    ptr->~T();

                AllocatorT::Free(mBeginB);
            }
        }

        template <class LAllocatorT>
        inline void Assign(const List<T, LAllocatorT> &source)
        {
            size_t allocSize = source.CapacityInBytes();
            size_t requiredSize = source.SizeInBytes();

            for (T *ptr = mBegin; ptr != mLast; ptr++)
                ptr->~T();

            if (CapacityInBytes() < requiredSize)
            {
                AllocatorT::Free(mBeginB);
                mEndB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;
            }

            mLastB = mBeginB + requiredSize;

            if (kTrivally)
            {
                memcpy(mBeginB, mLastB, requiredSize);
            }
            else
            {
                T *ptr = mBegin;
                const T *ptr2 = source.mBegin;

                for (; ptr != mLast; ptr++, ptr2++)
                    new (ptr) T(*ptr2);
            }
        }

        inline void Assign(List<T, AllocatorT> &&source)
        {
            for (T *ptr = mBegin; ptr != mLast; ptr++)
                ptr->~T();

            AllocatorT::Free(mBeginB);

            Resource<AllocatorT> &rs = *this;
            rs = std::move(source);

            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

        inline List<T, AllocatorT> &operator=(const List<T, AllocatorT> &source)
        {
            Assign(source);
            return *this;
        }

        template <class LAllocatorT>
        inline List<T, AllocatorT> &operator=(const List<T, LAllocatorT> &source)
        {
            Assign(source);
            return *this;
        }

        inline List<T, AllocatorT> &operator=(List<T, AllocatorT> &&source)
        {
            Assign(std::move(source));
            return *this;
        }

        inline void Resize(size_t newSize)
        {
            auto point = mBegin + newSize;
            if (point <= mLast)
            {
                if constexpr (!kTrivally)
                {
                    for (auto it = point; it != mLast; it++)
                        it->~T();
                }

                mLast = point;

                return;
            }
            else
            {
                if (point > mEnd)
                    ReallocateMemory(SizeInBytes(), point - mEnd);

                for (auto it = mLast; it != point; it++)
                    new (it) T();

                mLast = point;
            }
        }

        inline size_t Size() const { return mLast - mBegin; }
        inline size_t Capacity() const { return mEnd - mBegin; }
        inline size_t SizeInBytes() const { return mLastB - mBeginB; }
        inline size_t CapacityInBytes() const { return mEndB - mBeginB; }
        inline bool Empty() const { return mLast == mBegin; }

        inline iterator Begin() { return mBegin; }
        inline iterator End() { return mLast; }
        inline const_iterator Begin() const { return mBegin; }
        inline const_iterator End() const { return mLast; }
        inline iterator ReverseBegin() { return mLast - 1; }
        inline iterator ReverseEnd() { return mBegin - 1; }
        inline const_iterator ReverseBegin() const { return mLast - 1; }
        inline const_iterator ReverseEnd() const { return mBegin - 1; }

        inline void InsertBack(const T *pElems, size_t count)
        {
            size_t sizeInBytes = SizeInBytes();
            size_t reqSize = sizeInBytes + count * sizeof(T);
            if (Capacity() < reqSize)
            {
                ReallocateMemory(sizeInBytes, reqSize);
            }

            auto newLast = mLast + count;
            for (; mLast != newLast; mLast++, pElems++)
                new (mLast) T(*pElems);
        }

        inline void Remove(const T &elem)
        {
            for (auto it = mBegin; it != mLast; it++)
            {
                if (*it == elem)
                {
                    Erase(it);
                    return;
                }
            }
        }

        inline void FastRemove(const T &elem)
        {
            for (auto it = mBegin; it != mLast; it++)
            {
                if (*it == elem)
                {
                    FastErase(it);
                    return;
                }
            }
        }

        inline T &Front()
        {
            assert(Size() && "Attempt get out of range element in List<T>::Front");
            return *Begin();
        }
        inline const T &Front() const
        {
            assert(Size() && "Attempt get out of range element in List<T>::Front");
            return *Begin();
        }
        inline T &Back()
        {
            assert(Size() && "Attempt get out of range element in List<T>::Back");
            return *ReverseBegin();
        }
        inline const T &Back() const
        {
            assert(Size() && "Attempt get out of range element in List<T>::Back");
            return *ReverseBegin();
        }

        inline void PushBack(const T &o)
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, sizeInBytes);
            }

            new (mLast) T(o);
            mLast++;
        }

        inline void PushBack(T &&o)
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, sizeInBytes);
            }

            new (mLast) T(std::move(o));
            mLast++;
        }

        inline void Add(const T &o) { PushBack(o); }
        inline void Add(T &&o) { PushBack(std::move(o)); }

        inline T &At(size_t i)
        {
            assert(Size() > i && "Attempt get out of range element in List<T>::At");
            return mBegin[i];
        }
        inline const T &At(size_t i) const
        {
            assert(Size() > i && "Attempt get out of range element in List<T>::At");
            return mBegin[i];
        }

        inline void Erase(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::Erase");
            assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::Erase");

            it.ptr->~T();
            T *from = it.ptr + 1;
            ::memcpy(it.ptr, from, mLastB - (uchar *)from);
            mLast--;
        }

        inline void Erase(size_t i) { Erase(mBegin + i); }
        inline size_t Index(const_iterator it) const { return size_t(it.ptr - mBegin); }
        inline size_t Index(iterator it) const { return size_t(it.ptr - mBegin); }

        inline void FastErase(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::FastErase");
            assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::FastErase");

            T *pBack = mLast - 1;
            it.ptr->~T();

            if (it.ptr != pBack)
            {
                // c++ fan version
                // new (it.ptr) T(std::move(*pBack));

                // right version
                memcpy(it.ptr, pBack, sizeof(T));
            }

            mLast--;
        }

        inline void FastErase(size_t i) { FastErase(mBegin + i); }

        inline void Clear()
        {
            for (T *ptr = mBegin; ptr != mLast; ptr++)
                ptr->~T();

            mLast = mBegin;
        }

        inline T &operator[](size_t i) { return At(i); }
        inline const T &operator[](size_t i) const { return At(i); }

        inline void PopBack()
        {
            assert(Size() && "Attempt pop element in empty list in List<T>::Pop");

            mLast--;
            mLast->~T();
        }

        inline void Pop() { PopBack(); }

        inline void Reserve(size_t s)
        {
            if (Capacity() >= s)
                return;

            uchar *newMem = (uchar *)AllocatorT::Alloc(s);
            assert(newMem && "Out of memory in List<T>::Reserve");

            T *oldBegin = mBegin;
            size_t oldSizeInBytes = SizeInBytes();

            mEndB = (mLastB = mBeginB = newMem) + s;
            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            AllocatorT::Free(oldBegin);
        }

        inline T *Data() { return mBegin; }
        inline const T *Data() const { return mBegin; }
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
        inline bool empty() { return Empty(); }
        inline T &front() { return Front(); }
        inline const T &front() const { return Front(); }
        inline T &back() { return Back(); }
        inline const T &back() const { return Back(); }
        inline T &at(size_t i) { return At(i); }
        inline const T &at(size_t i) const { return At(i); }

    private:
        inline void ReallocateMemory(size_t currentSizeInBytes, size_t additionalSizeInBytes)
        {
            size_t oldSizeInBytes = currentSizeInBytes;
            size_t allocSizeInBytes = oldSizeInBytes + additionalSizeInBytes;
            T *oldBegin = mBegin;

            mEndB = (mLastB = mBeginB = (uchar *)AllocatorT::Alloc(allocSizeInBytes)) + allocSizeInBytes;

            assert(mBegin && "Out of memory in List<T>::ReallocateMemory");

            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            AllocatorT::Free(oldBegin);
        }

        inline void CopyInit(const T *source, size_t dataSize)
        {
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(dataSize)) + dataSize;
            assert(mBegin && "Out of memory in List<T>::List(const List&)");

            if constexpr (kTrivally)
            {
                memcpy(mBeginB, source, dataSize);
            }
            else
            {
                T *ptr = mBegin;
                const T *ptr2 = source;

                for (; ptr != mLast; ptr++, ptr2++)
                    new (ptr) T(*ptr2);
            }
        }

        template <class ListT>
        inline void CopyInit(const ListT &source)
        {
            CopyInit(source.mBegin, source.SizeInBytes());
        }

        union {
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

    // use for Cont<T*> where T* allocated as new T
    template <class ContT>
    void LoopDelete(const ContT &cont)
    {
        for (auto ptr : cont)
            delete ptr;
    }

    // use for Cont<T*> where T* allocated as new (buf) T
    template <class ContT>
    void LoopDestroy(const ContT &cont)
    {
        for (auto ptr : cont)
            ptr->~T();
    }
} // namespace ulib
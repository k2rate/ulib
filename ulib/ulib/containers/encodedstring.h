#pragma once

#include <ulib/types.h>
#include <ulib/config.h>

#include <ulib/allocators/defaultallocator.h>
#include <ulib/resources/resource.h>

#include "encodedstringview.h"
#include "clength.h"
#include "args.h"
#include "range.h"

#include "iterators/randomaccessiterator.h"

#include <type_traits>
#include <cstring>

namespace ulib
{
#ifdef ULIB_USE_STD_STRING_VIEW
    template <class T>
    struct CheckStdStringView
    {
        constexpr static bool kTrue = false;
    };

    template <class CharT>
    struct CheckStdStringView<std::basic_string_view<CharT>>
    {
        constexpr static bool kTrue = true;
    };

    template <class T>
    inline constexpr bool IsStdStringView = CheckStdStringView<T>::kTrue;
#endif

    template <class EncodingTy, class AllocatorTy = DefaultAllocator>
    class EncodedString : public Resource<AllocatorTy>
    {
    public:
        // type definitions

        using EncodingT = EncodingTy;
        using AllocatorT = AllocatorTy;
        using SelfT = EncodedString<EncodingT, AllocatorT>;
        using SelfRefT = SelfT &;
        using BaseT = Resource<AllocatorT>;

        using CharT = typename EncodingT::CharT;
        using Iterator = RandomAccessIterator<CharT>;
        using ConstIterator = RandomAccessIterator<const CharT>;

        using AllocatorParams = typename AllocatorT::Params;

        using ParentEncodingT = typename EncodingT::ParentEncodingT;
        using ParentEncodingCharT = typename ParentEncodingT::CharT;
        using ParentEncodedStringT = EncodedString<ParentEncodingT, AllocatorT>;

        using value_type = CharT;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        using RangeT = Range<const CharT>;

        constexpr static size_t C_STEP = 16;
        constexpr static size_t M_STEP = sizeof(CharT) * C_STEP;

        // constructors

        inline EncodedString(AllocatorParams al = {})
            : BaseT(al)
        {
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(M_STEP))) + M_STEP;
        }

        inline EncodedString(const CharT *str, AllocatorParams al = {})
            : BaseT(al)
        {
            size_t allocSize = CStringLengthHack(str) * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        inline EncodedString(const CharT *b, const CharT *e, AllocatorParams al = {})
            : BaseT(al)
        {
            size_t allocSize = (e - b) * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, b, allocSize);
        }

        inline EncodedString(const CharT *str, size_t size, AllocatorParams al = {})
            : BaseT(al)
        {
            size_t allocSize = size * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        template <class ContainerT, class vt = typename ContainerT::value_type, class RangeT = Range<const typename ContainerT::value_type>,
                  std::enable_if_t<std::is_same_v<CharT, std::remove_cv_t<vt>>, bool> = true>
        inline EncodedString(const ContainerT &cont, AllocatorParams al = {})
            : BaseT(al)
        {
            RangeT str = cont;

            size_t allocSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str.Data(), allocSize);
        }

        inline EncodedString(size_t size, AllocatorParams al = {})
            : BaseT(al)
        {
            size_t allocSize = sizeof(CharT) * size;
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            for (auto it = mBegin; it != mEnd; it++)
                new (it) CharT();
        }

        inline EncodedString(args::Capacity capacity, AllocatorParams al = {})
            : BaseT(al)
        {
            size_t allocSize = sizeof(CharT) * capacity.cc;
            mEndB = (mLastB = mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;
        }

        // operators overloads

        template <class LAllocatorT>
        inline SelfT operator+(const EncodedString<EncodingT, LAllocatorT> &right) const { return AdditionImpl(right.mBegin, right.SizeInBytes()); }
        inline SelfT operator+(const CharT *right) const { return AdditionImpl(right); }

        template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!IsStdStringView<StringT>, bool> = true>
#endif
        inline SelfT operator+(const StringT &right) const
        {
            return AdditionImpl(right.data(), right.size());
        }

        template <class LAllocatorT>
        inline SelfRefT operator+=(const EncodedString<EncodingT, LAllocatorT> &right)
        {
            Append(right);
            return *this;
        }

        inline SelfRefT operator+=(const EncodedStringView<EncodingT> &right)
        {
            Append(right.data(), right.size());
            return *this;
        }

        inline SelfRefT operator+=(const CharT *right)
        {
            Append(right);
            return *this;
        }

        template <class LAllocatorT>
        inline bool operator<(const EncodedString<EncodingT, LAllocatorT> &right) const { return LowerThanImpl(right.mBegin, right.mLast); }
        inline bool operator<(const CharT *right) const { return LowerThanImpl(right); }

        inline bool operator==(const CharT *right) const { return Equal(right); }

        template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!IsStdStringView<StringT>, bool> = true>
#endif
        inline bool operator==(const StringT &right) const
        {
            return Equal(right);
        }

        inline bool operator!=(const CharT *right) const { return !Equal(right); }

        operator ParentEncodedStringT() const { return ParentEncodedStringT((ParentEncodingCharT *)this->mBegin, (ParentEncodingCharT *)this->mLast); }

#ifdef ULIB_USE_STD_STRING_VIEW

        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(this->mBegin, this->Size());
        }

        operator std::basic_string<CharT>() const
        {
            return std::basic_string<CharT>(this->mBegin, this->mLast);
        }

#ifdef __cpp_char8_t
        // template <class CurrentCharT = typename EncodingT::CharStd, std::enable_if_t<std::is_same_v<ParentEncodingT, MultibyteEncoding>, bool> = true>
        operator std::basic_string_view<typename EncodingT::CharStd>() const
        {
            return std::basic_string_view<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin, this->Size());
        }

        operator std::basic_string<typename EncodingT::CharStd>() const
        {
            return std::basic_string<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin, (typename EncodingT::CharStd *)this->mLast);
        }
#endif
#endif

        // lowercase aliases

        inline iterator begin() { return mBegin; }
        inline iterator end() { return mLast; }
        inline const_iterator begin() const { return mBegin; }
        inline const_iterator end() const { return mLast; }
        inline size_t size() const { return Size(); }
        inline size_t capacity() const { return Capacity(); }
        inline CharT *data() { return Data(); }
        inline const CharT *data() const { return Data(); }
        inline void erase(iterator it) { Erase(it); }
        inline void push_back(const CharT &o) { PushBack(o); }
        inline void push_back(CharT &&o) { PushBack(std::move(o)); }
        inline void pop_back() { PopBack(); }
        inline bool empty() const { return Empty(); }
        inline void reserve(size_t s) { Reserve(s); }
        inline CharT *c_str()
        {
            MarkZeroEnd();
            return Data();
        }

        // function definitions

        inline CharT *Data() { return mBegin; }
        inline CharT *Data() const { return mBegin; }
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

        inline void PushBack(CharT ch) { PushBackImpl(ch); }
        inline void Reserve(size_t s) { ReserveImpl(s); }
        inline void Erase(iterator it) { EraseImpl(it); }
        inline void Erase(size_t i) { EraseImpl(mBegin + i); }
        inline int Index(const_iterator it) const { return int(it.ptr - mBegin); }
        inline int Index(iterator it) const { return int(it.ptr - mBegin); }
        inline void FastErase(iterator it) { FastEraseImpl(it); }
        inline void PopBack() { PopBackImpl(); }
        inline void Pop() { PopBack(); }
        inline void MarkZeroEnd() { MarkZeroEndImpl(); }

        template <class LAllocatorT>
        inline void Assign(const EncodedString<EncodingT, LAllocatorT> &source) { Assign(source.mBegin, source.SizeInBytes()); }
        inline void Assign(SelfT &&source) { return AssignImpl(std::move(source)); }

        template <class LAllocatorT>
        inline bool Equal(const EncodedString<EncodingT, LAllocatorT> &right) const { return EqualImpl(RangeT(right.data(), right.size())); }
        inline bool Equal(const CharT *cstr) const { return EqualImpl(RangeT(cstr, CStringLengthHack(cstr))); }
        inline bool Equal(RangeT right) const { return EqualImpl(right); }

        template <class LAllocatorT>
        inline void Append(const EncodedString<EncodingT, LAllocatorT> &right) { AppendImpl(right.data(), right.size()); }
        inline void Append(const CharT *right) { AppendImpl(right, CStringLengthHack(right)); }
        inline void Append(const CharT *right, size_t rightSizeInBytes) { return AppendImpl(right, rightSizeInBytes); }

        // inline bool operator!=(ulib::Range<const CharT> right) const { return !Equal(right); }
        // inline bool operator==(ulib::Range<CharT> right) const { return Equal(right); }

        /*
        #ifdef ULIB_USE_STD_STRING_VIEW
        inline bool operator==(const std::basic_string<CharT> &right) const
        {
            return Equal(Range<const CharT>(right.data(), right.data() + right.size()));
        }

        inline bool operator!=(std::basic_string_view<CharT> right) const
        {
            return !Equal(Range<const CharT>(right.data(), right.data() + right.size()));
        }

#endif
        */

    protected:
        inline bool EqualImpl(RangeT right) const
        {
            size_t sizeInBytes = right.SizeInBytes();
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, right.Data(), sizeInBytes) == 0;
        }

        inline void AssignImpl(SelfT &&source)
        {
            AllocatorT::Free(mBeginB);

            Resource<AllocatorT> &rs = *this;
            rs = std::move(source);

            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }
        inline void AppendImpl(const CharT *right, size_t rightSizeInBytes)
        {
            size_t sizeInBytes = SizeInBytes();
            size_t reqSize = sizeInBytes + rightSizeInBytes;
            if (Capacity() < reqSize)
            {
                ReallocateMemoryWithRight(sizeInBytes, reqSize, right, rightSizeInBytes);
            }
            else
            {
                memcpy(mLast, right, rightSizeInBytes);
                mLast = mBegin + reqSize;
            }
        }

        inline void AppendImpl(const CharT *right)
        {
            AppendImpl(right, CStringLengthHack(right));
        }

        inline void AssignImpl(const CharT *str, size_t sizeInBytes)
        {
            size_t allocSize = sizeInBytes;
            size_t requiredSize = sizeInBytes;

            if (CapacityInBytes() < requiredSize)
            {
                AllocatorT::Free(mBegin);
                mEndB = (mBeginB = rawptr_t(AllocatorT::Alloc(allocSize))) + allocSize;
            }

            mLastB = mBeginB + requiredSize;
            memcpy(mBegin, mLast, requiredSize);
        }

        inline SelfT AdditionImpl(const CharT *right, size_t rightSizeInBytes) const
        {
            size_t sizeInBytes = SizeInBytes();
            size_t reqSize = sizeInBytes + rightSizeInBytes;

            SelfT result{args::Capacity(reqSize)};
            result.InitSummary(this->mBegin, sizeInBytes, right, rightSizeInBytes, reqSize);

            return result;
        }

        inline SelfT AdditionImpl(const CharT *right) const
        {
            return AdditionImpl(right, CStringLengthHack(right));
        }

        void MarkZeroEndImpl()
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, 1);
            }

            *mLast = 0;
        }

        inline void PopBackImpl()
        {
            assert(Size() && "Attempt pop element in empty list in List<T>::Pop");
            mLast--;
        }

        inline void FastEraseImpl(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::FastErase");
            assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::FastErase");

            CharT *pBack = mLast - 1;
            if (it.ptr != pBack)
            {
                // c++ fan version
                // new (it.ptr) T(std::move(*pBack));

                // right version
                memcpy(it.ptr, pBack, sizeof(CharT));
            }

            mLast--;
        }

        inline void EraseImpl(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in Container<T>::Erase");
            assert(it.ptr < mLast && "Attempt erase out of range element in Container<T>::Erase");

            CharT *from = it.ptr + 1;
            ::memcpy(it.ptr, from, mLastB - (uchar *)from);
            mLast--;
        }

        inline void ReserveImpl(size_t s)
        {
            if (Capacity() >= s)
                return;

            uchar *newMem = (uchar *)AllocatorT::Alloc(s);
            assert(newMem && "Out of memory in List<T>::Reserve");

            CharT *oldBegin = mBegin;
            size_t oldSizeInBytes = SizeInBytes();

            mEndB = (mLastB = mBeginB = newMem) + s;
            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            AllocatorT::Free(oldBegin);
        }

        inline void PushBackImpl(CharT ch)
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, sizeInBytes);
            }

            *mLast = ch;
            mLast++;
        }

        inline bool LowerThanImpl(const CharT *right, const CharT *rightEnd) const
        {
            auto it = mBegin;
            auto rit = right;
            for (; *it == *rit; it++, rit++)
            {
                if (it == mLast || rit == rightEnd)
                    return false;
            }

            using UnsignedT = std::make_unsigned_t<CharT>;

            return *(UnsignedT *)it < *(UnsignedT *)rit;
        }

        inline bool LowerThanImpl(const CharT *right) const
        {
            return LowerThanImpl(right, right + CStringLengthHack(right));
        }

        inline void InitSummary(const CharT *first, size_t firstSizeInBytes, const CharT *second, size_t secondSizeInBytes, size_t summarySizeInBytes)
        {
            memcpy(mBeginB, first, firstSizeInBytes);
            memcpy(mBeginB + firstSizeInBytes, second, secondSizeInBytes);

            mLastB = mBeginB + summarySizeInBytes;
        }

        inline void ReallocateMemory(size_t currentSizeInBytes, size_t additionalSizeInBytes)
        {
            size_t oldSizeInBytes = currentSizeInBytes;
            size_t allocSizeInBytes = oldSizeInBytes + additionalSizeInBytes;
            CharT *oldBegin = mBegin;

            mEndB = (mLastB = mBeginB = (rawptr_t)AllocatorT::Alloc(allocSizeInBytes)) + allocSizeInBytes;

            assert(mBegin && "Out of memory in List<T>::ReallocateMemory");

            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            AllocatorT::Free(oldBegin);
        }

        inline void ReallocateMemoryWithRight(size_t currentSizeInBytes, size_t additionalSizeInBytes, const CharT *right, size_t rightSizeInBytes)
        {
            size_t oldSizeInBytes = currentSizeInBytes;
            size_t allocSizeInBytes = oldSizeInBytes + additionalSizeInBytes;
            CharT *oldBegin = mBegin;

            mEndB = (mLastB = mBeginB = (rawptr_t)AllocatorT::Alloc(allocSizeInBytes)) + allocSizeInBytes;

            assert(mBegin && "Out of memory in List<T>::ReallocateMemory");

            mLastB += oldSizeInBytes;

            ::memcpy(mBeginB, oldBegin, oldSizeInBytes);
            ::memcpy(mLastB, right, rightSizeInBytes);

            mLastB += rightSizeInBytes;

            AllocatorT::Free(oldBegin);
        }

        union
        {
            struct
            {
                CharT *mBegin;
                CharT *mLast;
                CharT *mEnd;
            };

            struct
            {
                uchar *mBeginB;
                uchar *mLastB;
                uchar *mEndB;
            };
        };
    };
}

template <class EncodingT, class AllocatorT>
inline ulib::EncodedString<EncodingT, AllocatorT> operator+(ulib::EncodedStringView<EncodingT> left, const ulib::EncodedString<EncodingT, AllocatorT> &right)
{
    ulib::EncodedString<EncodingT, AllocatorT> result(left);
    result += right;
    return result;
}

template <class EncodingT>
inline ulib::EncodedString<EncodingT> operator+(ulib::EncodedStringView<EncodingT> left, ulib::EncodedStringView<EncodingT> right)
{
    ulib::EncodedString<EncodingT> result(left);
    result += right;
    return result;
}

/*
#ifdef ULIB_USE_STD_STRING_VIEW
template<class EncodingT, class AllocatorT>
inline bool operator==(const std::basic_string<typename EncodingT::CharT>& str, const ulib::EncodedString<EncodingT, AllocatorT>& ustr)
{
    return ustr == str;
}
#endif
*/

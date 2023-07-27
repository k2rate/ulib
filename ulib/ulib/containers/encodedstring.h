#pragma once

#include <ulib/config.h>
#include <ulib/types.h>

#include <ulib/allocators/defaultallocator.h>
#include <ulib/encodings/length.h>
#include <ulib/resources/resource.h>

#include "args.h"
#include "clength.h"
#include "encodedstringview.h"
#include "range.h"

#include "iterators/randomaccessiterator.h"
#include "ulib/containers/clength.h"
#include "ulib/encodings/literalencoding.h"

#include "tags.h"

#include <cstring>
#include <stdio.h>
#include <type_traits>

#include <stdexcept>
#include <ulib/typetraits/string.h>

#ifdef min
#undef min
#endif

namespace ulib
{
    template <class EncodingTy, class AllocatorTy = DefaultAllocator>
    class EncodedString : public Resource<AllocatorTy>
    {
    public:
        // type definitions

        using EncodingT = EncodingTy;
        using AllocatorT = AllocatorTy;
        using ResourceT = Resource<AllocatorTy>;
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
        using const_reference = const value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        using RangeT = Range<const CharT>;

        using ContainerTagT = string_container_tag;

        constexpr static size_t C_STEP = 16;
        constexpr static size_t M_STEP = sizeof(CharT) * C_STEP;

        // constructors

        inline EncodedString(AllocatorParams al = {}) : BaseT(al)
        {
            mEndB = (mLastB = mBeginB = rawptr_t(ResourceT::Alloc(M_STEP))) + M_STEP;
        }

        inline EncodedString(const CharT *str, AllocatorParams al = {}) : BaseT(al)
        {
            size_t allocSize = CStringLengthHack(str) * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        inline EncodedString(const CharT *b, const CharT *e, AllocatorParams al = {}) : BaseT(al)
        {
            size_t allocSize = (e - b) * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, b, allocSize);
        }

        inline EncodedString(const CharT *str, size_t size, AllocatorParams al = {}) : BaseT(al)
        {
            size_t allocSize = size * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        template <class ContainerT, class vt = typename ContainerT::value_type,
                  class RangeT = Range<const typename ContainerT::value_type>,
                  std::enable_if_t<std::is_same_v<CharT, std::remove_cv_t<vt>>, bool> = true>
        inline EncodedString(const ContainerT &cont, AllocatorParams al = {}) : BaseT(al)
        {
            RangeT str = cont;

            size_t allocSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str.Data(), allocSize);
        }

        inline EncodedString(size_t size, AllocatorParams al = {}) : BaseT(al)
        {
            size_t allocSize = sizeof(CharT) * size;
            mEndB = mLastB = (mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;

            for (auto it = mBegin; it != mEnd; it++)
                new (it) CharT();
        }

        inline EncodedString(args::Capacity capacity, AllocatorParams al = {}) : BaseT(al)
        {
            size_t allocSize = sizeof(CharT) * capacity.cc;
            mEndB = (mLastB = mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;
        }

        inline EncodedString(const SelfT &other) : BaseT(std::move(other))
        {
            size_t allocSize = other.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)ResourceT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, other.mBeginB, allocSize);
        }

        inline EncodedString(SelfT &&other) : BaseT(std::move(other))
        {
            mBeginB = other.mBeginB;
            mLastB = other.mLastB;
            mEndB = other.mEndB;

            other.mBeginB = nullptr;
        }

        inline ~EncodedString()
        {
            if (mBeginB)
            {
                ResourceT::Free(mBeginB);
                mBeginB = nullptr;
            }
        }

        // operators overloads

        inline reference operator[](size_t i) { return mBegin[i]; }
        inline const_reference operator[](size_t i) const { return mBegin[i]; }

        template <class LAllocatorT>
        inline SelfT operator+(const EncodedString<EncodingT, LAllocatorT> &right) const
        {
            return AdditionImpl(right.mBegin, right.SizeInBytes());
        }
        inline SelfT operator+(const CharT *right) const { return AdditionImpl(right); }

        template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!is_std_basic_string_view_v<StringT>, bool> = true>
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

        inline void operator=(const SelfT &right) { Assign(right); }
        inline void operator=(SelfT &&right) { Assign(std::move(right)); }

        template <class LAllocatorT>
        inline bool operator<(const EncodedString<EncodingT, LAllocatorT> &right) const
        {
            return LowerThanImpl(right.mBegin, right.mLast);
        }
        inline bool operator<(const CharT *right) const { return LowerThanImpl(right); }

        inline bool operator==(const CharT *right) const { return Equal(right); }
        inline bool operator!=(const CharT *right) const { return !Equal(right); }

        template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!is_std_basic_string_view_v<StringT>, bool> = true>
#endif
        inline bool operator==(const StringT &right) const
        {
            return Equal(right);
        }

        template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!is_std_basic_string_view_v<StringT>, bool> = true>
#endif
        inline bool operator!=(const StringT &right) const
        {
            return !Equal(right);
        }

        operator ParentEncodedStringT() const
        {
            return ParentEncodedStringT((ParentEncodingCharT *)this->mBegin, (ParentEncodingCharT *)this->mLast);
        }

#ifdef ULIB_USE_STD_STRING_VIEW

        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(this->mBegin, this->Size());
        }

        operator std::basic_string<CharT>() const { return std::basic_string<CharT>(this->mBegin, this->mLast); }

#ifdef __cpp_char8_t
        // template <class CurrentCharT = typename EncodingT::CharStd, std::enable_if_t<std::is_same_v<ParentEncodingT,
        // MultibyteEncoding>, bool> = true>
        operator std::basic_string_view<typename EncodingT::CharStd>() const
        {
            return std::basic_string_view<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin,
                                                                       this->Size());
        }

        operator std::basic_string<typename EncodingT::CharStd>() const
        {
            return std::basic_string<typename EncodingT::CharStd>((typename EncodingT::CharStd *)this->mBegin,
                                                                  (typename EncodingT::CharStd *)this->mLast);
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
        inline CharT *raw_data() { return RawData(); }
        inline const CharT *raw_data() const { return RawData(); }
        inline CharT *data() { return Data(); }
        inline const CharT *data() const { return Data(); }
        inline void erase(iterator it) { Erase(it); }
        inline void push_back(const CharT &o) { PushBack(o); }
        inline void push_back(CharT &&o) { PushBack(std::move(o)); }
        inline void pop_back() { PopBack(); }
        inline bool empty() const { return Empty(); }
        inline void reserve(size_t s) { Reserve(s); }
        inline void resize(size_t s) { Resize(s); }
        inline void resize(size_t s, CharT ch) { Resize(s, ch); }
        inline CharT *c_str() const { return Data(); }
        inline size_t length() const { return Length(); }
        inline void clear() { Clear(); }
        inline reference at(size_t i) { return At(i); };
        inline const_reference at(size_t i) const { return At(i); };
        inline reference front() { return *mBegin; }
        inline const_reference front() const { return *mBegin; }
        inline reference back() { return *(mLast - 1); }
        inline const_reference back() const { return *(mLast - 1); }

        template <class LAllocatorT>
        inline void assign(const EncodedString<EncodingT, LAllocatorT> &source)
        {
            Assign(source);
        }
        inline void assign(SelfT &&source) { return Assign(std::move(source)); };
        inline void assign(const CharT *source) { Assign(source); }

        template <class LAllocatorT>
        inline void append(const EncodedString<EncodingT, LAllocatorT> &right)
        {
            Append<LAllocatorT>(right);
        }
        inline void append(const CharT *right) { Append(right); }
        inline void append(const CharT *right, size_t rightSizeInBytes) { return Append(right, rightSizeInBytes); }

        // function definitions

        inline CharT *RawData() { return mBegin; }
        inline CharT *RawData() const { return mBegin; }
        inline CharT *Data()
        {
            MarkZeroEndConst();
            return mBegin;
        }
        inline CharT *Data() const
        {
            MarkZeroEndConst();
            return mBegin;
        }
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
        inline void Reserve(size_t s) { ReserveImpl(s * sizeof(CharT)); }
        inline void Resize(size_t s) { ResizeImpl(s, 0); }
        inline void Resize(size_t s, CharT ch) { ResizeImpl(s, ch); }
        inline void Erase(iterator it) { EraseImpl(it); }
        inline void Erase(size_t i) { EraseImpl(mBegin + i); }
        inline int Index(const_iterator it) const { return int(it.ptr - mBegin); }
        inline int Index(iterator it) const { return int(it.ptr - mBegin); }
        inline void FastErase(iterator it) { FastEraseImpl(it); }
        inline void PopBack() { PopBackImpl(); }
        inline void Pop() { PopBack(); }
        inline void MarkZeroEnd() { MarkZeroEndImpl(); }
        inline void Clear() { mLast = mBegin; }
        inline reference At(size_t i)
        {
            if (i >= Size())
                throw std::out_of_range("string out of range");
            return mBegin[i];
        }

        inline const_reference At(size_t i) const
        {
            if (i >= Size())
                throw std::out_of_range("string out of range");
            return mBegin[i];
        }

        inline reference Front() { return *mBegin; }
        inline const_reference Front() const { return *mBegin; }
        inline reference Back() { return *(mLast - 1); }
        inline const_reference Back() const { return *(mLast - 1); }

        template <class LAllocatorT>
        inline void Assign(const EncodedString<EncodingT, LAllocatorT> &source)
        {
            AssignImpl(source.mBegin, source.SizeInBytes());
        }
        inline void Assign(SelfT &&source) { AssignImpl(std::move(source)); }
        inline void Assign(const CharT *source) { AssignImpl(source, CStringLengthHack(source)); }

        template <class LAllocatorT>
        inline bool Equal(const EncodedString<EncodingT, LAllocatorT> &right) const
        {
            return EqualImpl(RangeT(right.data(), right.size()));
        }
        inline bool Equal(const CharT *cstr) const { return EqualImpl(RangeT(cstr, CStringLengthHack(cstr))); }
        inline bool Equal(RangeT right) const { return EqualImpl(right); }

        template <class LAllocatorT>
        inline void Append(const EncodedString<EncodingT, LAllocatorT> &right)
        {
            AppendImpl(right.data(), right.size());
        }
        inline void Append(const CharT *right) { AppendImpl(right, CStringLengthHack(right)); }
        inline void Append(const CharT *right, size_t rightSizeInBytes) { return AppendImpl(right, rightSizeInBytes); }

        inline size_t Length() const { return CalcStringLength<EncodingT>(mBegin, mLast); }

        /*
        inline void Detach()
        {
            mBeginB = nullptr;
        }
        */

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
            ResourceT::Free(mBeginB);

            ResourceT &rs = *this;
            rs = std::move(source);

            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

        inline void AssignImpl(const CharT *str, size_t sizeInBytes)
        {
            size_t allocSize = sizeInBytes;
            size_t requiredSize = sizeInBytes;

            if (CapacityInBytes() < requiredSize)
            {
                ResourceT::Free(mBegin);
                mEndB = (mBeginB = rawptr_t(ResourceT::Alloc(allocSize))) + allocSize;
            }

            mLastB = mBeginB + requiredSize;
            memcpy(mBegin, str, requiredSize);
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

        inline void AppendImpl(const CharT *right) { AppendImpl(right, CStringLengthHack(right)); }

        inline SelfT AdditionImpl(const CharT *right, size_t rightSizeInBytes) const
        {
            size_t sizeInBytes = SizeInBytes();
            size_t reqSize = sizeInBytes + rightSizeInBytes;

            SelfT result{args::Capacity(reqSize)};
            result.InitSummary(this->mBegin, sizeInBytes, right, rightSizeInBytes, reqSize);

            return result;
        }

        inline SelfT AdditionImpl(const CharT *right) const { return AdditionImpl(right, CStringLengthHack(right)); }

        void MarkZeroEndImpl()
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, sizeof(CharT));
            }

            *mLast = 0;
        }

        inline void MarkZeroEndConst() const { ((SelfT *)this)->MarkZeroEndImpl(); }

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

            uchar *newMem = (uchar *)ResourceT::Alloc(s);

            CharT *oldBegin = mBegin;
            size_t oldSizeInBytes = SizeInBytes();

            mEndB = (mLastB = mBeginB = newMem) + s;
            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            ResourceT::Free(oldBegin);
        }

        inline void ResizeImpl(size_t s, CharT ch)
        {
            size_t argSizeInBytes = s * sizeof(CharT);
            size_t currentSizeInBytes = SizeInBytes();

            if (currentSizeInBytes > argSizeInBytes)
            {
                mLastB = mBeginB + argSizeInBytes;
            }
            else if (currentSizeInBytes < argSizeInBytes)
            {
                ReserveImpl(argSizeInBytes);

                auto it = mLast;
                for (; it != (CharT *)(mBeginB + argSizeInBytes); it++)
                {
                    *it = ch;
                }

                mLast = it;
            }
            else
            {
                // nothing to do
                return;
            }
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
            assert(right);
            assert(right < rightEnd);

            size_t rightSizeInBytes = size_t((uchar *)rightEnd - (uchar *)right);
            size_t minSize = std::min(SizeInBytes(), rightSizeInBytes);

            int result = memcmp(mBegin, right, minSize);
            if (result == 0)
                return minSize != rightSizeInBytes;

            return result < 0;
        }

        inline bool LowerThanImpl(const CharT *right) const
        {
            assert(right);

            return LowerThanImpl(right, right + CStringLengthHack(right));
        }

        inline void InitSummary(const CharT *first, size_t firstSizeInBytes, const CharT *second,
                                size_t secondSizeInBytes, size_t summarySizeInBytes)
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

            mEndB = (mLastB = mBeginB = (rawptr_t)ResourceT::Alloc(allocSizeInBytes)) + allocSizeInBytes;

            assert(mBegin && "Out of memory in List<T>::ReallocateMemory");

            mLastB += oldSizeInBytes;

            ::memcpy(mBegin, oldBegin, oldSizeInBytes);
            ResourceT::Free(oldBegin);
        }

        inline void ReallocateMemoryWithRight(size_t currentSizeInBytes, size_t additionalSizeInBytes,
                                              const CharT *right, size_t rightSizeInBytes)
        {
            size_t oldSizeInBytes = currentSizeInBytes;
            size_t allocSizeInBytes = oldSizeInBytes + additionalSizeInBytes;
            CharT *oldBegin = mBegin;

            mEndB = (mLastB = mBeginB = (rawptr_t)ResourceT::Alloc(allocSizeInBytes)) + allocSizeInBytes;

            assert(mBegin && "Out of memory in List<T>::ReallocateMemory");

            mLastB += oldSizeInBytes;

            ::memcpy(mBeginB, oldBegin, oldSizeInBytes);
            ::memcpy(mLastB, right, rightSizeInBytes);

            mLastB += rightSizeInBytes;

            ResourceT::Free(oldBegin);
        }

        union {
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

    template <class CharT, class EncodingT = ulib::LiteralEncodingT<CharT>, class AllocatorT>
    inline bool operator==(const CharT *const left, const ulib::EncodedString<EncodingT, AllocatorT> &right)
    {
        return right == left;
    }

    template <class CharT, class EncodingT = ulib::LiteralEncodingT<CharT>, class AllocatorT>
    inline bool operator!=(const CharT *const left, const ulib::EncodedString<EncodingT, AllocatorT> &right)
    {
        return right != left;
    }

} // namespace ulib

template <class EncodingT, class AllocatorT>
inline ulib::EncodedString<EncodingT, AllocatorT> operator+(ulib::EncodedStringView<EncodingT> left,
                                                            const ulib::EncodedString<EncodingT, AllocatorT> &right)
{
    ulib::EncodedString<EncodingT, AllocatorT> result(left);
    result += right;
    return result;
}

template <class EncodingT>
inline ulib::EncodedString<EncodingT> operator+(ulib::EncodedStringView<EncodingT> left,
                                                ulib::EncodedStringView<EncodingT> right)
{
    ulib::EncodedString<EncodingT> result(left);
    result += right;
    return result;
}

namespace std
{
    template <class EncodingT, class AllocatorT, class AllocT>
    inline bool operator==(
        const std::basic_string<typename EncodingT::CharT, std::char_traits<typename EncodingT::CharT>, AllocT> &str,
        const ulib::EncodedString<EncodingT, AllocatorT> &str1)
    {
        return str1.Equal(str);
    }

    template <class EncodingT, class AllocatorT, class AllocT>
    inline bool operator!=(
        const std::basic_string<typename EncodingT::CharT, std::char_traits<typename EncodingT::CharT>, AllocT> &str,
        const ulib::EncodedString<EncodingT, AllocatorT> &str1)
    {
        return !str1.Equal(str);
    }
} // namespace std

/*
#ifdef ULIB_USE_STD_STRING_VIEW
template<class EncodingT, class AllocatorT>
inline bool operator==(const std::basic_string<typename EncodingT::CharT>& str, const ulib::EncodedString<EncodingT,
AllocatorT>& ustr)
{
    return ustr == str;
}
#endif
*/

#pragma once

#include <ulib/types.h>
#include <ulib/config.h>

#include <ulib/allocators/defaultallocator.h>
#include <ulib/resources/resource.h>

#include "clength.h"
#include "stringview.h"
#include "iterators/randomaccessiterator.h"
#include "args.h"
#include "range.h"

#include <type_traits>
#include <cstring>

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#endif

namespace ulib
{
    template <class CharT, class AllocatorT = DefaultAllocator>
    class BasicString : public Resource<AllocatorT>
    {
    public:
        constexpr static size_t C_STEP = 16;
        constexpr static size_t M_STEP = sizeof(CharT) * C_STEP;

        using Iterator = RandomAccessIterator<CharT>;
        using ConstIterator = RandomAccessIterator<const CharT>;
        using AllocatorParams = typename AllocatorT::Params;

        using value_type = CharT;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        inline BasicString(AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(M_STEP))) + M_STEP;
        }

        template <class ContainerT, class vt = typename ContainerT::value_type, class RangeT = Range<const typename ContainerT::value_type>, std::enable_if_t<std::is_same_v<CharT, std::remove_cv_t<vt>>, bool> = true>
        inline BasicString(const ContainerT &cont, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            RangeT str = cont;

            size_t allocSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str.Data(), allocSize);
        }

        inline BasicString(size_t size, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(CharT) * size;
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            assert(mBegin && "Out of memory in List<T>::List(size_t)");

            for (auto it = mBegin; it != mEnd; it++)
                new (it) CharT();
        }

        inline BasicString(args::Capacity capacity, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(CharT) * capacity.cc;
            mEndB = (mLastB = mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            assert(mBegin && "Out of memory in List<T>::List(size_t)");
        }

        inline BasicString(const CharT *str, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = CStringLengthHack(str) * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        inline BasicString(const CharT *str, size_t size, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = size * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str, allocSize);
        }

        inline BasicString(const CharT *b, const CharT *e, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = (e - b) * sizeof(CharT);
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, b, allocSize);
        }

        inline BasicString(const BasicString<CharT, AllocatorT> &str)
            : Resource<AllocatorT>(str)
        {
            size_t dataSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(dataSize)) + dataSize;
            memcpy(mBeginB, str.Data(), dataSize);
        }

        inline BasicString(BasicString<CharT, AllocatorT> &&source)
            : Resource<AllocatorT>(std::move(source))
        {
            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

#ifdef ULIB_USE_STD_STRING_VIEW
        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(mBegin, Size());
        }

        operator std::basic_string<CharT>() const
        {
            return std::basic_string<CharT>(mBegin, mLast);
        }
#endif

        inline ~BasicString()
        {
            if (mBegin)
            {
                AllocatorT::Free(mBegin);
            }
        }

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

        inline void Reserve(size_t s)
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

        inline void Erase(iterator it)
        {
            assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::Erase");
            assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::Erase");

            CharT *from = it.ptr + 1;
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

        inline void PopBack()
        {
            assert(Size() && "Attempt pop element in empty list in List<T>::Pop");
            mLast--;
        }

        inline void Pop() { PopBack(); }

        void MarkZeroEnd()
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, 1);
            }

            *mLast = 0;
        }

        inline CharT *c_str()
        {
            MarkZeroEnd();
            return Data();
        }

        template <class LAllocatorT>
        inline void Assign(const BasicString<CharT, LAllocatorT> &source)
        {
            Assign(source.mBegin, source.SizeInBytes());
        }

        inline void Assign(BasicString<CharT, AllocatorT> &&source)
        {
            AllocatorT::Free(mBeginB);

            Resource<AllocatorT> &rs = *this;
            rs = std::move(source);

            mBegin = source.mBegin;
            mLast = source.mLast;
            mEnd = source.mEnd;

            source.mBegin = nullptr;
        }

        inline bool Equal(const CharT *cstr) const
        {
            size_t sizeInBytes = CStringLengthHack(cstr) * sizeof(CharT);
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, cstr, sizeInBytes) == 0;
        }

        inline bool Equal(ulib::Range<const CharT> right) const
        {
            size_t sizeInBytes = right.SizeInBytes();
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, right.Data(), sizeInBytes) == 0;
        }

        template <class LAllocatorT>
        inline bool Equal(const BasicString<CharT, LAllocatorT> &right) const
        {
            size_t sizeInBytes = right.SizeInBytes();
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, right.mBeginB, sizeInBytes) == 0;
        }

        template <class ContainerT, class vt = typename ContainerT::value_type,
                  class RangeT = Range<const typename ContainerT::value_type>,
                  std::enable_if_t<std::is_same_v<CharT, std::remove_cv_t<vt>>, bool> = true>
        inline bool operator==(const ContainerT &right) const
        {
            RangeT str = right;
            return Equal(str);
        }

        template <class LAllocatorT>
        inline bool operator==(const BasicString<CharT, LAllocatorT> &right) const { return Equal(right); }
        template <class LAllocatorT>
        inline bool operator!=(const BasicString<CharT, LAllocatorT> &right) const { return !Equal(right); }

        inline bool operator==(const CharT *right) const { return Equal(right); }
        inline bool operator!=(const CharT *right) const { return !Equal(right); }

        inline bool operator<(const CharT *right) const
        {
            return LowerThan(right);
        }

        template<class LAllocatorT>
        inline bool operator<(const BasicString<CharT, LAllocatorT> &right) const
        {
            return LowerThan(right.mBegin, right.mLast);
        }

        inline BasicString<CharT, AllocatorT> &operator=(const BasicString<CharT, AllocatorT> &source)
        {
            Assign(source);
            return *this;
        }

        template <class LAllocatorT>
        inline BasicString<CharT, AllocatorT> &operator=(const BasicString<CharT, LAllocatorT> &source)
        {
            Assign(source);
            return *this;
        }

        inline BasicString<CharT, AllocatorT> &operator=(BasicString<CharT, AllocatorT> &&source)
        {
            Assign(std::move(source));
            return *this;
        }

        template <class LAllocatorT>
        inline void Append(const BasicString<CharT, LAllocatorT> &right)
        {
            Append(right.mBegin, right.SizeInBytes());
        }

        inline void Append(const CharT *right)
        {
            Append(right, CStringLengthHack(right) * sizeof(CharT));
        }

        template <class LAllocatorT>
        inline BasicString<CharT, AllocatorT> operator+(const BasicString<CharT, LAllocatorT> &right) const
        {
            return Sum(right.mBegin, right.SizeInBytes());
        }

        inline BasicString<CharT, AllocatorT> operator+(const CharT *right) const
        {
            return Sum(right, CStringLengthHack(right) * sizeof(CharT));
        }

        template <class LAllocatorT>
        inline BasicString<CharT, AllocatorT> &operator+=(const BasicString<CharT, AllocatorT> &right)
        {
            Append(right);
            return *this;
        }

        inline BasicString<CharT, AllocatorT> &operator+=(const CharT *right)
        {
            Append(right);
            return *this;
        }

        inline void PushBack(CharT ch)
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
        inline bool LowerThan(const CharT *right, const CharT *rightEnd) const
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

        inline bool LowerThan(const CharT *right) const
        {
            return LowerThan(right, right + CStringLengthHack(right));
        }

        inline void Assign(const CharT *str, size_t sizeInBytes)
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

        inline void Append(const CharT *right, size_t rightSizeInBytes)
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

        inline BasicString<CharT, AllocatorT> Sum(const CharT *right, size_t rightSizeInBytes) const
        {
            size_t sizeInBytes = SizeInBytes();
            size_t reqSize = sizeInBytes + rightSizeInBytes;

            BasicString<CharT, AllocatorT> result{args::Capacity(reqSize)};
            result.InitSummary(mBegin, sizeInBytes, right, rightSizeInBytes, reqSize);

            return result;
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

    /*
    using String = BasicString<char, DefaultAllocator>;
    using WideString = BasicString<wchar_t, DefaultAllocator>;

    // timokha
    using string = String;
    using wstring = WideString;
    */

    /*
    using Utf8String = EncodedString<Utf8, DefaultAllocator>;
    using Utf16String = EncodedString<Utf16, DefaultAllocator>;
    using Utf32String = EncodedString<Utf32, DefaultAllocator>;

    using PathString = EncodedString<PlatformEncoding, DefaultAllocator>;
    using path = PathString;

    // timokha
    using string = String;
    using wstring = WideString;
    using u8string = Utf8String;
    using u16string = Utf16String;
    using u32string = Utf32String;
    */

    /*

 namespace ulib
 {
     template <class StringViewT>
     struct SplitIterator
     {
         SplitIterator()
         {
         }

         SplitIterator(StringViewT str)
         {

         }

         inline StringViewT &operator*() const { return *ptr; }
         inline StringViewT *operator->() const { return ptr; }

         inline SplitIterator &operator++()
         {
             assert(mCurrent.end() != mSource.end());
             auto it = mCurrent.end();

             return *this;
         } // Prefix increment operator.

         inline SplitIterator &operator++(int)
         {
             ptr++;
             return *this;
         } // Postfix increment operator.

         StringViewT mCurrent;
         StringViewT mSource;
         StringViewT mSeparator;
     };

     template <class StringViewT>
     struct SplitView
     {
     public:
         SplitView()
         {
         }

         ~SplitView()
         {
         }

         StringViewT Next()
         {
             auto it = mCurrent.Begin();
         }

     private:
         StringViewT mCurrent;
         StringViewT mSeparator;
     };

     template <class CharT>
     void StrSplit(ulib::BasicStringView<CharT> str, ulib::BasicStringView<CharT> separator)
     {
     }

     template <class EncodingT, std::enable_if_t<sizeof(typename EncodingT::CharAliasT), bool> = true>
     void StrSplit(ulib::EncodedStringView<EncodingT> str, ulib::EncodedStringView<EncodingT> separator)
     {
     }

     template <class CharT>
     void StrSplit(const CharT *str, const CharT *sep)
     {
         auto strLen = CStringLengthHack(str);
         auto sepLen = CStringLengthHack(sep);

         return StrSplit(ulib::BasicStringView<CharT>(str, str + strLen), ulib::BasicStringView<CharT>(sep, sep + sepLen))
     }

     void test()
     {
         for (auto str : ulib::StrSplit(u8"ky ky ky", u8" "))
         {
             str; // string_view
         }
     }
 }

 */
}

/*
#ifdef ULIB_USE_STD_STRING_VIEW
template <class CharT>
inline bool operator==(const std::basic_string<CharT> &str, const ulib::BasicString<CharT> &ustr)
{
    return ustr == str;
}
#endif
*/

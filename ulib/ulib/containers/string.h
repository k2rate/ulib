#pragma once

#include <ulib/types.h>
#include <ulib/allocators/defaultallocator.h>
#include <ulib/resources/resource.h>

#include "clength.h"
#include "stringview.h"

#define ULIB_USE_STD_STRING_VIEW

#ifdef ULIB_USE_STD_STRING_VIEW
#include <string_view>
#endif

#include <type_traits>

#include <cstring>

namespace ulib
{
    template <class CharT, class AllocatorT>
    class BasicString : public Resource<AllocatorT>
    {
    public:
        constexpr static size_t C_STEP = 16;
        constexpr static size_t M_STEP = sizeof(CharT) * C_STEP;

        using AllocatorParams = typename AllocatorT::Params;
        using value_type = CharT;

        inline BasicString(AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(M_STEP))) + M_STEP;
        }

        inline BasicString(size_t capacity, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(CharT) * capacity;
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(allocSize))) + allocSize;
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

        inline BasicString(BasicStringView<CharT> str, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = str.SizeInBytes();
            mEndB = mLastB = (mBeginB = (uchar *)AllocatorT::Alloc(allocSize)) + allocSize;

            memcpy(mBeginB, str.Data(), allocSize);
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
        operator std::basic_string_view<CharT>()
        {
            return std::basic_string_view<CharT>(mBegin, mEnd);
        }
#endif

        /*
               template <size_t N>
        inline BasicString(std::initializer_list<CharT> init, AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = init.size() * sizeof(CharT);
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(allocSize))) + allocSize;

            memcpy(mBeginB, init.begin(), init.size() * sizeof(CharT));
        }

        */

        /*
               template <size_t N>
        inline BasicString(const CharT (&arr)[N], AllocatorParams al = {})
            : Resource<AllocatorT>(al)
        {
            size_t allocSize = sizeof(CharT) * N;
            mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(allocSize))) + allocSize;
            memcpy(mBeginB, arr, N);
        }
        */

        /*
                template <size_t N>
         inline BasicString(const CharT (&arr)[N], AllocatorParams al = {})
             : Resource<AllocatorT>(al)
         {
             size_t allocSize = sizeof(CharT) * N;
             mEndB = (mLastB = mBeginB = rawptr_t(AllocatorT::Alloc(allocSize))) + allocSize;
             memcpy(mBeginB, arr, N);
         }
        */

        inline ~BasicString()
        {
            if (mBegin)
            {
                AllocatorT::Free(mBegin);
            }
        }

        inline CharT *Data() { return mBegin; }
        inline CharT *Data() const { return mBegin; }
        inline CharT *data() { return mBegin; }
        inline CharT *data() const { return mBegin; }

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

        template <class LAllocatorT>
        inline bool Equal(const BasicString<CharT, LAllocatorT> &right) const
        {
            size_t sizeInBytes = right.SizeInBytes();
            if (sizeInBytes != SizeInBytes())
                return false;

            return memcmp(mBeginB, right.mBeginB, sizeInBytes) == 0;
        }

        template <class LAllocatorT>
        inline bool operator==(const BasicString<CharT, LAllocatorT> &right) const { return Equal(right); }
        template <class LAllocatorT>
        inline bool operator!=(const BasicString<CharT, LAllocatorT> &right) const { return !Equal(right); }

        inline bool operator==(const CharT *right) const { return Equal(right); }
        inline bool operator!=(const CharT *right) const { return !Equal(right); }

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

        template <class LAllocatorT>
        inline BasicString<CharT, AllocatorT> operator+(const BasicString<CharT, LAllocatorT> &right)
        {
            size_t sizeInBytes = SizeInBytes();
            size_t rightSizeInBytes = right.SizeInBytes();

            size_t reqSize = sizeInBytes + rightSizeInBytes;
            BasicString<CharT, AllocatorT> result(reqSize);

            memcpy(result.mBeginB, mBeginB, sizeInBytes);
            memcpy(result.mBeginB + sizeInBytes, right.mBeginB, rightSizeInBytes);

            result.mLastB = result.mBeginB + reqSize;

            return result;
        }

        inline void PushBack(CharT ch)
        {
            if (mLast == mEnd)
            {
                size_t sizeInBytes = SizeInBytes();
                ReallocateMemory(sizeInBytes, sizeInBytes);
            }

            mLast = ch;
            mLast++;
        }

        size_t Size() const
        {
            return mLast - mBegin;
        }

        size_t size() const
        {
            return mLast - mBegin;
        }

        size_t Capacity() const
        {
            return mEnd - mBegin;
        }

        size_t SizeInBytes() const
        {
            return mLastB - mBeginB;
        }

        size_t CapacityInBytes() const
        {
            return mEndB - mBeginB;
        }

        void SetSize(size_t newSize)
        {
            mLast = mBegin + newSize;
        }

    protected:
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

    using String = BasicString<char, DefaultAllocator>;
    using WideString = BasicString<wchar_t, DefaultAllocator>;

    // timokha
    using string = String;
    using wstring = WideString;

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

}
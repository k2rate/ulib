#pragma once

#include <ulib/containers/string.h>
#include <ulib/containers/encodedstringview.h>

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
    class EncodedString : protected BasicString<typename EncodingTy::CharT, AllocatorTy>
    {
    public:
        using EncodingT = EncodingTy;
        using AllocatorT = AllocatorTy;

        using CharT = typename EncodingT::CharT;
        using Iterator = RandomAccessIterator<CharT>;
        using ConstIterator = RandomAccessIterator<const CharT>;

        using AllocatorParams = typename AllocatorT::Params;

        using BaseT = BasicString<typename EncodingT::CharT, AllocatorT>;

        using ParentEncodingT = typename EncodingT::ParentEncodingT;
        using ParentEncodingCharT = typename ParentEncodingT::CharT;
        using ParentEncodedStringT = EncodedString<ParentEncodingT, AllocatorT>;

        using value_type = CharT;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        inline EncodedString(AllocatorParams al = {})
            : BaseT(al)
        {
        }

        inline EncodedString(const CharT *str, AllocatorParams al = {})
            : BaseT(str, al)
        {
        }

        inline EncodedString(const CharT *b, const CharT *e, AllocatorParams al = {})
            : BaseT(b, e, al)
        {
        }

        inline EncodedString(const CharT *str, size_t size, AllocatorParams al = {})
            : BaseT(str, size, al)
        {
        }

        template <class ContainerT, class vt = typename ContainerT::value_type, class RangeT = Range<const typename ContainerT::value_type>,
                  std::enable_if_t<std::is_same_v<CharT, std::remove_cv_t<vt>>, bool> = true>
        inline EncodedString(const ContainerT &cont, AllocatorParams al = {})
            : BaseT(cont, al)
        {
        }

        inline EncodedString(size_t size, AllocatorParams al = {})
            : BaseT(size, al)
        {
        }

        inline EncodedString(args::Capacity capacity, AllocatorParams al = {})
            : BaseT(capacity, al)
        {
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

        inline CharT *Data()
        {
            return this->mBegin;
        }
        inline CharT *Data() const { return this->mBegin; }
        inline iterator Begin() { return this->mBegin; }
        inline iterator End() { return this->mLast; }
        inline const_iterator Begin() const { return this->mBegin; }
        inline const_iterator End() const { return this->mLast; }
        inline iterator ReverseBegin() { return this->mLast - 1; }
        inline iterator ReverseEnd() { return this->mBegin - 1; }
        inline const_iterator ReverseBegin() const { return this->mLast - 1; }
        inline const_iterator ReverseEnd() const { return this->mBegin - 1; }
        inline bool Empty() const { return this->mBegin == this->mLast; }
        inline size_t Size() const { return this->mLast - this->mBegin; }
        inline size_t Capacity() const { return this->mEnd - this->mBegin; }
        inline size_t Available() const { return this->mEnd - this->mLast; }
        inline size_t SizeInBytes() const { return this->mLastB - this->mBeginB; }
        inline size_t CapacityInBytes() const { return this->mEndB - this->mBeginB; }
        inline size_t AvailableInBytes() const { return this->mEnd - this->mLast; }
        inline void SetSize(size_t newSize) { this->mLast = this->mBegin + newSize; }
        inline void PushBack(const CharT &o) { BaseT::PushBack(o); }
        inline void PushBack(CharT &&o) { BaseT::PushBack(std::move(o)); }
        inline iterator begin() { return this->mBegin; }
        inline iterator end() { return this->mLast; }
        inline const_iterator begin() const { return this->mBegin; }
        inline const_iterator end() const { return this->mLast; }
        inline size_t size() const { return Size(); }
        inline size_t capacity() const { return Capacity(); }
        inline CharT *data() { return Data(); }
        inline const CharT *data() const { return Data(); }
        inline void erase(iterator it) { Erase(it); }
        inline void push_back(const CharT &o) { BaseT::PushBack(o); }
        inline void push_back(CharT &&o) { BaseT::PushBack(std::move(o)); }
        inline void pop_back() { PopBack(); }
        inline bool empty() const { return Empty(); }
        inline void reserve(size_t s) { Reserve(s); }
        inline void Reserve(size_t s) { BaseT::Reserve(s); }
        inline void Erase(iterator it) { return BaseT::Erase(it); }
        inline void Erase(size_t i) { Erase(this->mBegin + i); }
        inline void FastErase(iterator it) { return BaseT::FastErase(it); }
        inline int Index(const_iterator it) const { return int(it.ptr - this->mBegin); }
        inline int Index(iterator it) const { return int(it.ptr - this->mBegin); }
        inline void PopBack() { return BaseT::PopBack(); }
        inline void Pop() { PopBack(); }
        inline void MarkZeroEnd() { return BaseT::MarkZeroEnd(); }
        inline CharT *c_str() { return BaseT::c_str(); }

        template <class LAllocatorT>
        inline void Assign(const EncodedString<EncodingT, LAllocatorT> &source) { BaseT::Assign(source.mBegin, source.SizeInBytes()); }
        inline void Assign(const EncodedString<EncodingT> &&source) { BaseT::Assign(std::move(source)); }

        template <class LAllocatorT>
        inline void Append(const EncodedString<EncodingT, LAllocatorT> &right) { BaseT::Append(right); }
        inline void Append(const CharT *right) { BaseT::Append(right); }
        inline void Append(const CharT *right, size_t rightSizeInBytes) { BaseT::Append(right, rightSizeInBytes); }

        template <class LAllocatorT>
        inline EncodedString<EncodingT, AllocatorT> operator+(const EncodedString<EncodingT, LAllocatorT> &right) const
        {
            return Sum(right.mBegin, right.SizeInBytes());
        }

        inline EncodedString<EncodingT, AllocatorT> operator+(const CharT *right) const
        {
            return Sum(right, CStringLengthHack(right) * sizeof(CharT));
        }

        template <class LAllocatorT>
        inline EncodedString<EncodingT, AllocatorT> &operator+=(const EncodedString<EncodingT, LAllocatorT> &right)
        {
            Append(right);
            return *this;
        }

        inline EncodedString<EncodingT, AllocatorT> &operator+=(const EncodedStringView<EncodingT> &right)
        {
            Append(right.data(), right.size());
            return *this;
        }

        inline EncodedString<EncodingT, AllocatorT> &operator+=(const CharT *right)
        {
            Append(right);
            return *this;
        }

        inline bool operator<(const CharT *right) const
        {
            return BaseT::LowerThan(right);
        }

        template <class LAllocatorT>
        inline bool operator<(const EncodedString<EncodingT, LAllocatorT> &right) const
        {
            return BaseT::LowerThan(right.mBegin, right.mLast);
        }

        /*
                template <class LAllocatorT>
        inline bool operator==(const EncodedString<EncodingT, LAllocatorT> &right) const { return BaseT::Equal(right); }
        template <class LAllocatorT>
        inline bool operator!=(const EncodedString<EncodingT, LAllocatorT> &right) const { return !BaseT::Equal(right); }

        */

        inline bool operator==(const CharT *right) const { return BaseT::Equal(right); }
        inline bool operator!=(const CharT *right) const { return !BaseT::Equal(right); }

        template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!IsStdStringView<StringT>, bool> = true>
#endif
        inline bool operator==(const StringT &right) const
        {
            return BaseT::Equal(right);
        }

         template <class StringT, class SCharT = typename StringT::value_type,
#ifdef ULIB_USE_STD_STRING_VIEW
                  std::enable_if_t<!IsStdStringView<StringT>, bool> = true>
#endif
        inline EncodedString<EncodingT, AllocatorT> operator+(const StringT &right) const
        {
            return Sum(right.data(), right.size());
        }

        // inline bool operator!=(ulib::Range<const CharT> right) const { return !BaseT::Equal(right); }
        // inline bool operator==(ulib::Range<CharT> right) const { return BaseT::Equal(right); }

        /*
        #ifdef ULIB_USE_STD_STRING_VIEW
        inline bool operator==(const std::basic_string<CharT> &right) const
        {
            return BaseT::Equal(Range<const CharT>(right.data(), right.data() + right.size()));
        }

        inline bool operator!=(std::basic_string_view<CharT> right) const
        {
            return !BaseT::Equal(Range<const CharT>(right.data(), right.data() + right.size()));
        }

#endif
        */

    protected:
        inline EncodedString<EncodingT, AllocatorT> Sum(const CharT *right, size_t rightSizeInBytes) const
        {
            size_t sizeInBytes = SizeInBytes();
            size_t reqSize = sizeInBytes + rightSizeInBytes;

            EncodedString<EncodingT, AllocatorT> result{args::Capacity(reqSize)};
            result.InitSummary(this->mBegin, sizeInBytes, right, rightSizeInBytes, reqSize);

            return result;
        }
        /*
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
        */

        /*
                inline EncodedString(AllocatorParams al = {})
            : BasicString<CharT, AllocatorT>(al)
        {
        }

        inline EncodedString(size_t capacity, AllocatorParams al = {})
            : BasicString<CharT, AllocatorT>(capacity, al)
        {
        }

        inline EncodedString(const BasicString<CharT, AllocatorT> &str)
            : BasicString<CharT, AllocatorT>(str)
        {
        }

        inline EncodedString(EncodedStringView<EncodingT> str, AllocatorParams al = {})
            : BasicString<CharT, AllocatorT>((CharT *)str.Begin().Raw(), (CharT *)str.End().Raw(), al)
        {
        }
        */
    };
}

template<class EncodingT, class AllocatorT>
inline ulib::EncodedString<EncodingT, AllocatorT> operator+(ulib::EncodedStringView<EncodingT> left, const ulib::EncodedString<EncodingT, AllocatorT>& right)
{
    ulib::EncodedString<EncodingT, AllocatorT> result(left);
    result += right;
    return result;
}

template<class EncodingT>
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

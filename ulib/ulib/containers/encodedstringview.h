#pragma once

#include <ulib/config.h>
#include <ulib/cstrlen.h>
#include <ulib/encodings/length.h>
#include <ulib/span.h>


#include <ulib/typetraits/string.h>

#ifdef ULIB_STD_COMPATIBILITY
#include <string>
#include <string_view>
#endif

namespace ulib
{
    template <class EncodingTy>
    class EncodedStringView
    {
    public:
        using EncodingT = EncodingTy;
        using CharT = typename EncodingT::CharT;
        using SpanT = Span<CharT>;
        using SelfT = EncodedStringView<EncodingTy>;

        using Iterator = RandomAccessIterator<CharT>;
        using ConstIterator = RandomAccessIterator<const CharT>;
        using ReverseIterator = ReverseRandomAccessIterator<CharT>;
        using ConstReverseIterator = ReverseRandomAccessIterator<const CharT>;
        using ReverseT = ReversedSpan<const CharT>;
        using SplitViewT = SplitView<SelfT>;

        using value_type = CharT;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using reverse_iterator = ReverseIterator;
        using const_reverse_iterator = ConstReverseIterator;
        using size_type = size_t;

                // ulib fields
        using ContainerTypeT = string_type_tag;
        using ContainerOwnershipT = view_ownership_tag;

        EncodedStringView() {}
        EncodedStringView(const CharT *str) noexcept : mSpan(str, cstrlen(str)) {}
        EncodedStringView(const CharT *b, const CharT *e) noexcept : mSpan(b, e) {}
        EncodedStringView(ConstIterator b, ConstIterator e) noexcept : mSpan(b.ptr, e.ptr) {}
        EncodedStringView(const CharT *str, size_t size) noexcept : mSpan(str, size) {}
        EncodedStringView(const EncodedStringView<EncodingT> &source) : mSpan(source) {}

        template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        EncodedStringView(const K &str) : mSpan(str)
        {
        }
        ~EncodedStringView() noexcept = default;

        // functions

        inline const_pointer Data() { return mSpan.Data(); }
        inline const_pointer Data() const { return mSpan.Data(); }
        inline const_iterator Begin() const { return mSpan.Begin(); }
        inline const_iterator End() const { return mSpan.End(); }
        inline const_reverse_iterator ReverseBegin() const { return mSpan.ReverseBegin(); }
        inline const_reverse_iterator ReverseEnd() const { return mSpan.ReverseEnd(); }
        inline ReverseT Reverse() const { return ReverseT{ReverseBegin(), ReverseEnd()}; }
        inline bool Empty() const { return mSpan.Empty(); }
        inline size_type Size() const { return mSpan.Size(); }
        inline size_type Length() const { return CalcStringLength<EncodingT>(mSpan.Begin().Raw(), mSpan.End().Raw()); }
        inline size_type SizeInBytes() const { return mSpan.SizeInBytes(); }
        inline void RemovePrefix(size_type c) { mSpan.RemovePrefix(c); }
        inline void RemoveSuffix(size_type c) { mSpan.RemoveSuffix(c); }
        inline const_reference At(size_type idx) const { return mSpan.At(idx); }
        inline const_reference Front() const { return mSpan.Front(); }
        inline const_reference Back() const { return mSpan.Back(); }
        inline bool Compare(SelfT right) const { return mSpan.Compare(right); }
        inline size_type Find(const_reference v, size_type pos = 0) const { return mSpan.Find(v, pos); }
        inline size_type Find(SelfT v, size_type pos = 0) const { return mSpan.Find(v, pos); }
        inline size_type ReverseFind(const_reference v, size_type pos = 0) const { return mSpan.ReverseFind(v, pos); }
        inline size_type ReverseFind(SelfT v, size_type pos = 0) const { return mSpan.ReverseFind(v, pos); }
        inline bool StartsWith(const_reference v) const { return mSpan.StartsWith(v); }
        inline bool EndsWith(const_reference v) const { return mSpan.EndsWith(v); }
        inline bool Contains(const_reference v) const { return mSpan.Contains(v); }
        inline bool StartsWith(SelfT v) const { return mSpan.StartsWith(v); }
        inline bool EndsWith(SelfT v) const { return mSpan.EndsWith(v); }
        inline bool Contains(SelfT v) const { return mSpan.Contains(v); }
        inline SelfT SubString(size_type pos, size_type n = npos) const { return SelfT(mSpan.SubSpan(pos, n)); }
        inline SplitViewT Split(SelfT sep) const { return SplitViewT{*this, sep}; }
        inline BufferView Raw() const { return mSpan.Raw(); }

        // operators

        template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        inline bool operator==(const K &right) const
        {
            return mSpan.Compare(SpanT{right});
        }
        inline bool operator==(const CharT *right) const { return mSpan.Compare(SpanT{right, cstrlen(right)}); }
        inline bool operator==(SelfT right) const { return mSpan.Compare(right); }

        template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        inline bool operator!=(const K &right) const
        {
            return !mSpan.Compare(SpanT{right});
        }
        inline bool operator!=(const CharT *right) const { return !mSpan.Compare(SpanT{right, cstrlen(right)}); }
        inline bool operator!=(SelfT right) const { return !mSpan.Compare(right); }

        inline const_reference operator[](size_type idx) const { return mSpan[idx]; }

#ifdef ULIB_STD_COMPATIBILITY

        operator std::basic_string_view<CharT>() const
        {
            return std::basic_string_view<CharT>(mSpan.Data(), mSpan.Size());
        }

        operator std::basic_string<CharT>() const { return std::basic_string<CharT>(mSpan.Data(), mSpan.Size()); }

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

        // aliases

        inline const_pointer data() const { return Data(); }
        inline const_iterator begin() const { return Begin(); }
        inline const_iterator end() const { return End(); }
        inline const_reverse_iterator rbegin() const { return ReverseBegin(); }
        inline const_reverse_iterator rend() const { return ReverseEnd(); }
        inline ReverseT reverse() const { return Reverse(); }
        inline bool empty() const { return Empty(); }
        inline size_type size() const { return Size(); }
        inline size_type size_in_bytes() const { return SizeInBytes(); }
        inline size_type length() const { return Length(); }
        inline void remove_prefix(size_type c) { RemovePrefix(c); }
        inline void remove_suffix(size_type c) { RemoveSuffix(c); }
        inline const_reference at(size_type idx) const { return At(idx); }
        inline const_reference front() const { return Front(); }
        inline const_reference back() const { return Back(); }
        inline bool compare(SelfT right) const { return Compare(right); }
        inline size_type find(const_reference v, size_type pos = 0) const { return Find(v, pos); }
        inline size_type find(SelfT v, size_type pos = 0) const { return Find(v, pos); }
        inline size_type rfind(const_reference v, size_type pos = 0) const { return ReverseFind(v, pos); }
        inline size_type rfind(SelfT v, size_type pos = 0) const { return ReverseFind(v, pos); }
        inline bool starts_with(const_reference v) const { return StartsWith(v); }
        inline bool ends_with(const_reference v) const { return EndsWith(v); }
        inline bool contains(const_reference v) const { return Contains(v); }
        inline bool starts_with(SelfT v) const { return StartsWith(v); }
        inline bool ends_with(SelfT v) const { return EndsWith(v); }
        inline bool contains(SelfT v) const { return Contains(v); }
        inline SelfT substr(size_type pos, size_type n = npos) const { return SubString(pos, n); }
        inline SplitViewT split(SelfT sep) const { return Split(sep); }
        inline BufferView raw() const { return Raw(); }
    private:
        SpanT mSpan;
    };

    template <class EncodingT, class CharT>
    inline bool operator==(const CharT *left, ulib::EncodedStringView<EncodingT> str)
    {
        return str == left;
    }

    template <class EncodingT, class CharT>
    inline bool operator!=(const CharT *left, ulib::EncodedStringView<EncodingT> str)
    {
        return str != left;
    }

} // namespace ulib

#pragma once

#include <ulib/containers/iterators/randomaccessiterator.h>
#include <ulib/containers/splitview.h>
#include <ulib/containers/tags.h>


#include <ulib/types.h>
#include <ulib/typetraits/range.h>


#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace ulib
{
    static constexpr size_t npos = size_t(-1);

    template <class T>
    class ReversedSpan
    {
    public:
        using ReverseIterator = ReverseRandomAccessIterator<T>;
        using reverse_iterator = ReverseIterator;
        using ContainerTagT = list_container_tag;

        ReversedSpan(ReverseIterator rb, ReverseIterator re) noexcept : ms(rb), me(re) {}
        ~ReversedSpan() noexcept = default;

        inline ReverseIterator begin() const noexcept { return ms; }
        inline ReverseIterator end() const noexcept { return me; }

    private:
        ReverseIterator ms;
        ReverseIterator me;
    };

    template <class T>
    class Span
    {
    public:
        using SelfT = Span<T>;

        using value_type = T;
        using BufferViewT = Span<uchar>;

        using Iterator = RandomAccessIterator<value_type>;
        using ConstIterator = RandomAccessIterator<const value_type>;
        using ReverseIterator = ReverseRandomAccessIterator<value_type>;
        using ConstReverseIterator = ReverseRandomAccessIterator<const value_type>;
        using ReverseT = ReversedSpan<const value_type>;
        using ContainerTagT = list_container_tag;
        using SplitViewT = SplitView<SelfT>;

        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using reverse_iterator = ReverseIterator;
        using const_reverse_iterator = ConstReverseIterator;
        using size_type = size_t;

        Span() noexcept { mBegin = mLast = nullptr; }
        Span(const SelfT &other) noexcept { mBegin = other.mBegin, mLast = other.mLast; }
        Span(const_pointer b, const_pointer e) noexcept { mBegin = b, mLast = e; }
        Span(const_pointer v, size_type count) noexcept { mBegin = v, mLast = v + count; }
        Span(const_iterator first, const_iterator last) noexcept { mBegin = first.raw(), mLast = last.raw(); }

        template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        Span(const K &arr)
        {
            mBegin = std::data(arr);
            mLast = mBegin + std::size(arr);
        }

        template <size_type N>
        Span(value_type (&arr)[N]) noexcept
        {
            mBegin = &arr[0], mLast = &arr[N];
        }

        ~Span() noexcept = default;
        // Span(const_iterator first, const_iterator last) { mBegin = first.raw(), mLast = first.raw(); }
        // functions

        inline const_pointer Data() { return mBegin; }
        inline const_pointer Data() const { return mBegin; }
        inline const_iterator Begin() const { return mBegin; }
        inline const_iterator End() const { return mLast; }
        inline const_reverse_iterator ReverseBegin() const { return mLast - 1; }
        inline const_reverse_iterator ReverseEnd() const { return mBegin - 1; }
        inline ReverseT Reverse() const { return ReverseT{ReverseBegin(), ReverseEnd()}; }
        inline bool Empty() const { return mBegin == mLast; }
        inline size_type Size() const { return mLast - mBegin; }
        inline size_type Length() const { return mLast - mBegin; }
        inline size_type SizeInBytes() const { return size_type(mLast) - size_type(mBegin); }
        inline void RemovePrefix(size_type c) { mBegin += c; }
        inline void RemoveSuffix(size_type c) { mLast -= c; }
        inline const_reference At(size_type idx) const
        {
            if (idx >= Size())
                throw std::out_of_range{".at() out of range"};
            return mBegin[idx];
        }
        inline const_reference Front() const
        {
            assert(!Empty());
            return *Begin();
        }

        inline const_reference Back() const
        {
            assert(!Empty());
            return *(mLast - 1);
        }

        inline bool Compare(SelfT right) const { return std::equal(mBegin, mLast, right.mBegin, right.mLast); }

        inline size_type Find(const_reference v, size_type pos = 0) const
        {
            auto it = std::find(mBegin + pos, mLast, v);
            return it == mLast ? npos : it - mBegin;

            // for (auto it = mBegin + pos; it != mLast; it++)
            // {
            //     if (*it == v)
            //         return pos;
            //     ++pos;
            // }

            // return npos;
        }

        inline size_type Find(SelfT v, size_type pos = 0) const
        {
            auto it = std::search(mBegin + pos, mLast, v.mBegin, v.mLast);
            return it == mLast ? npos : it - mBegin;

            // size_type msize = SizeInBytes() - pos * sizeof(value_type), vsize = v.SizeInBytes();
            // if (msize < vsize)
            //     return npos;

            // auto it = mBegin + pos;
            // while (true)
            // {
            //     if (memcmp(it, v.mBegin, vsize) == 0)
            //         return pos;

            //     ++pos;
            //     ++it;

            //     if (it == (const_pointer)((uchar *)mLast - vsize))
            //     {
            //         if (memcmp(it, v.mBegin, vsize) == 0)
            //             return pos;

            //         return npos;
            //     }
            // }

            // return npos;
        }

        inline size_type ReverseFind(const_reference v, size_type pos = 0) const
        {
            // auto re = mBegin + pos - 1;

            auto it = std::find(ReverseBegin(), ConstReverseIterator{mBegin + pos - 1}, v);
            return it == ReverseEnd() ? npos : it.base() - ConstIterator(mBegin);

            // auto mend = mBegin + pos - 1;
            // auto mbegin = mLast - 1;

            // size_type rpos = Size() - 1;

            // for (auto it = mbegin; it != mend; it--)
            // {
            //     if (*it == v)
            //         return rpos;
            //     --rpos;
            // }

            // return npos;
        }

        inline size_type ReverseFind(SelfT v, size_type pos = 0) const
        {
            auto it = std::search(ReverseBegin(), ConstReverseIterator{mBegin + pos - 1}, v.ReverseBegin(), v.ReverseEnd());
            return it == ReverseEnd() ? npos : it.base() - ConstIterator(mBegin) - v.Size() + 1;

            // size_type msize = SizeInBytes() - pos * sizeof(value_type), vsize = v.SizeInBytes();
            // if (msize < vsize)
            //     return false;

            // auto mend = mBegin + pos - 1;
            // auto mbegin = (const_pointer)((uchar *)mLast - vsize);
            // size_type rpos = Size() - v.Size();

            // for (auto it = mbegin; it != mend; it--)
            // {
            //     if (memcmp(it, v.mBegin, vsize) == 0)
            //         return rpos;
            //     --rpos;
            // }

            // return npos;
        }

        inline bool StartsWith(const_reference v) const { return Front() == v; }
        inline bool EndsWith(const_reference v) const { return Back() == v; }
        inline bool Contains(const_reference v) const
        {
            for (auto &obj : *this)
                if (obj == v)
                    return true;
            return false;
        }

        inline bool StartsWith(SelfT v) const
        {
            size_type msize = SizeInBytes(), vsize = v.SizeInBytes();
            if (msize < vsize)
                return false;

            return std::equal(v.mBegin, v.mLast, mBegin);

            // return memcmp(mBegin, v.mBegin, vsize) == 0;
        }

        inline bool EndsWith(SelfT v) const
        {
            size_type msize = SizeInBytes(), vsize = v.SizeInBytes();
            if (msize < vsize)
                return false;

            return std::equal(v.mBegin, v.mLast, (const_pointer)((uchar *)mLast - vsize));
            // return memcmp((const_pointer)((uchar *)mLast - vsize), v.mBegin, vsize) == 0;
        }

        inline bool Contains(SelfT v) const { return Find(v) != npos; }
        inline SelfT SubSpan(size_type pos, size_type n = npos) const
        {
            auto first = mBegin + pos;
            if (n == npos)
            {
                if (first > mLast)
                    throw std::out_of_range{"subspan out of range"};

                return SelfT{first, mLast};
            }
            else
            {
                auto last = first + n;
                if (last > mLast)
                    throw std::out_of_range{"subspan out of range"};

                return SelfT{first, last};
            }
        }

        inline SplitViewT Split(SelfT sep) const { return SplitViewT{*this, sep}; }
        inline BufferViewT Raw() const { return BufferViewT{(uchar *)mBegin, (uchar *)mLast}; }

        // operators

        inline SelfT &operator=(SelfT right) { return mBegin = right.mBegin, mLast = right.mLast, *this; }
        // template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        // inline SelfT &operator=(const K &right)
        // {
        //     mBegin = right.data();
        //     mLast = mBegin + right.size();
        // }

        template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        inline bool operator==(const K &right) const
        {
            return Compare(SelfT{right});
        }
        inline bool operator==(SelfT right) const { return Compare(right); }

        template <class K, enable_if_range_compatible_t<SelfT, K> = true>
        inline bool operator!=(const K &right) const
        {
            return !Compare(SelfT{right});
        }
        inline bool operator!=(SelfT right) const { return !Compare(right); }

        inline const_reference operator[](size_type idx) const { return mBegin[idx]; }

        // aliases

        inline const_pointer data() const { return Data(); }
        inline const_iterator begin() const { return mBegin; }
        inline const_iterator end() const { return mLast; }
        inline const_reverse_iterator rbegin() const { return mLast - 1; }
        inline const_reverse_iterator rend() const { return mBegin - 1; }
        inline ReverseT reverse() const { return Reverse(); }
        inline bool empty() const { return Empty(); }
        inline size_type size() const { return Size(); }
        inline size_type size_in_bytes() const { return SizeInBytes(); }
        inline size_type length() const { return Length(); }
        inline void remove_prefix(size_type c) const { mBegin += c; }
        inline void remove_suffix(size_type c) const { mLast -= c; }
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
        inline SelfT subspan(size_type pos, size_type n = npos) const { return SubSpan(pos, n); }
        inline SplitViewT split(SelfT sep) const { return Split(sep); }
        inline BufferViewT raw() const { return Raw(); }

    private:
        const_pointer mBegin;
        const_pointer mLast;
    };

    template <class T>
    using span = Span<T>;

    using BufferView = Span<uchar>;
    using buffer_view = BufferView;

} // namespace ulib
#pragma once

#include <ulib/containers/iterators/randomaccessiterator.h>
#include <ulib/containers/tags.h>
#include <ulib/types.h>

#include <assert.h>
#include <span>
#include <stdexcept>
#include <string_view>

namespace ulib
{
    static constexpr size_t npos = size_t(-1);

    template <class T>
    class Span
    {
    public:
        using SelfT = Span<T>;

        using value_type = const T;
        using Iterator = RandomAccessIterator<value_type>;

        using pointer = value_type *;
        using const_pointer = value_type *;
        using reference = value_type &;
        using const_reference = reference;
        using iterator = Iterator;
        using const_iterator = Iterator;
        using ContainerTagT = list_container_tag;
        using size_type = size_t;

        Span() noexcept { mBegin = mLast = nullptr; }
        Span(const SelfT &other) noexcept { mBegin = other.mBegin, mLast = other.mLast; }
        Span(pointer b, pointer e) noexcept { mBegin = b, mLast = e; }
        Span(pointer v, size_t count) noexcept { mBegin = v, mLast = v + count; }
        Span(const_iterator first, const_iterator last) noexcept { mBegin = first.raw(), mLast = first.raw(); }

        template <
            class K, class ValueType = typename K::value_type,
            std::enable_if_t<std::is_same_v<typename K::iterator::iterator_category, std::random_access_iterator_tag> &&
                                 std::is_same_v<T, ValueType> && !std::is_same_v<K, SelfT>,
                             bool> = true>
        Span(const K &arr)
        {
            mBegin = arr.data();
            mLast = mBegin + arr.size();
        }

        template <size_type N>
        Span(std::type_identity_t<value_type> (&arr)[N]) noexcept
        {
            mBegin = &arr[0], mLast = &arr[N];
        }

        ~Span() noexcept = default;
        // Span(const_iterator first, const_iterator last) { mBegin = first.raw(), mLast = first.raw(); }
        // functions

        inline value_type *Data() { return mBegin; }
        inline value_type *Data() const { return mBegin; }
        inline const_iterator Begin() const { return mBegin; }
        inline const_iterator End() const { return mLast; }
        inline bool Empty() const { return mBegin == mLast; }
        inline size_type Size() const { return mLast - mBegin; }
        inline size_type Length() const { return mLast - mBegin; }
        inline size_type SizeInBytes() const { return size_type(mLast) - size_type(mBegin); }
        inline void RemovePrefix(size_type c) const { return mBegin += c; }
        inline void RemoveSuffix(size_type c) const { return mLast -= c; }
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

        inline bool Compare(SelfT right) const
        {
            size_t size = SizeInBytes();
            if (size != right.SizeInBytes())
                return false;

            return memcmp(mBegin, right.mBegin, size) == 0;
        }

        inline size_type Find(const_reference v, size_type pos = 0) const
        {
            for (auto it = mBegin + pos; it != mLast; it++)
            {
                if (*it == v)
                    return pos;
                ++pos;
            }

            return npos;
        }

        inline size_type Find(SelfT v, size_type pos = 0) const
        {
            size_type msize = SizeInBytes() - pos * sizeof(value_type), vsize = v.SizeInBytes();
            if (msize < vsize)
                return npos;

            auto it = mBegin + pos;
            while (true)
            {
                if (memcmp(it, v.mBegin, vsize) == 0)
                    return pos;

                ++pos;
                ++it;

                if (it == (const_pointer)((uchar *)mLast - vsize))
                {
                    if (memcmp(it, v.mBegin, vsize) == 0)
                        return pos;

                    return npos;
                }
            }

            // for (auto it = mBegin + pos; it != (const_pointer)((uchar *)mLast - vsize); it++)
            // {
            //     if (memcmp(it, v.mBegin, vsize) == 0)
            //         return pos;
            //     ++pos;
            // }

            return npos;
        }

        inline size_type ReverseFind(const_reference v, size_type pos = 0) const
        {
            auto mend = mBegin + pos - 1;
            auto mbegin = mLast - 1;

            size_type rpos = Size() - 1;

            for (auto it = mbegin; it != mend; it--)
            {
                if (*it == v)
                    return rpos;
                --rpos;
            }

            return npos;
        }

        inline size_type ReverseFind(SelfT v, size_type pos = 0) const
        {
            size_type msize = SizeInBytes() - pos * sizeof(value_type), vsize = v.SizeInBytes();
            if (msize < vsize)
                return false;

            auto mend = mBegin + pos - 1;
            auto mbegin = (const_pointer)((uchar *)mLast - vsize);
            size_type rpos = Size() - v.Size();

            for (auto it = mbegin; it != mend; it--)
            {
                if (memcmp(it, v.mBegin, vsize) == 0)
                    return rpos;
                --rpos;
            }

            return npos;
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

            return memcmp(mBegin, v.mBegin, vsize) == 0;
        }

        inline bool EndsWith(SelfT v) const
        {
            size_type msize = SizeInBytes(), vsize = v.SizeInBytes();
            if (msize < vsize)
                return false;

            return memcmp((const_pointer)((uchar *)mLast - vsize), v.mBegin, vsize) == 0;
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

        // operators

        inline bool operator==(SelfT right) const { return Compare(right); }
        inline bool operator!=(SelfT right) const { return !Compare(right); }
        inline const_reference operator[](size_type idx) const { return mBegin[idx]; }

        // aliases

        inline const_pointer data() const { return Data(); }
        inline const_iterator begin() const { return mBegin; }
        inline const_iterator end() const { return mLast; }
        inline bool empty() const { return Empty(); }
        inline size_type size() const { return Size(); }
        inline size_type size_in_bytes() const { return SizeInBytes(); }
        inline size_type length() const { return Length(); }
        inline void remove_prefix(size_type c) const { return mBegin += c; }
        inline void remove_suffix(size_type c) const { return mLast -= c; }
        inline const_reference at(size_type idx) const { return At(idx); }
        inline reference front() const { return Front(); }
        inline reference back() const { return Back(); }
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

    private:
        const_pointer mBegin;
        const_pointer mLast;
    };

    template <class T>
    using span = Span<T>;

} // namespace ulib
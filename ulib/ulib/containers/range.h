#pragma once

#include <type_traits>
#include <typeinfo>
#include <iterator>

#include <ulib/types.h>
#include "iterators/randomaccessiterator.h"

namespace ulib
{
    template <class T>
    class Range
    {
    public:
        using Iterator = RandomAccessIterator<T>;
        using ConstIterator = RandomAccessIterator<const T>;

        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        inline Range()
        {
            mBegin = nullptr;
            mLast = nullptr;
        }

        inline Range(T *begin, T *end)
        {
            mBegin = begin;
            mLast = end;
        }

        inline Range(Range<std::remove_cv_t<T>> &other)
        {
            mBegin = other.mBegin;
            mLast = other.mLast;
        }

        template <class ContainerT>
        inline Range(ContainerT &cont)
        {
            using std::data;
            using std::size;

            using ContT = std::remove_cv_t<std::remove_reference_t<decltype(*data(cont))>>;
            static_assert(std::is_same_v<ContT, std::remove_cv_t<T>>, "Range<T> type conversion failed");

            mBegin = data(cont);
            mLast = data(cont) + size(cont);
        }

        template <class ContainerT>
        inline Range<T> &operator=(ContainerT &cont)
        {
            using std::data;
            using std::size;

            using ContT = std::remove_cv_t<std::remove_reference_t<decltype(*data(cont))>>;
            static_assert(std::is_same_v<ContT, std::remove_cv_t<T>>, "RangeView type conversion failed");

            mBegin = data(cont);
            mLast = data(cont) + size(cont);

            return *this;
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
        inline size_t SizeInBytes() const { return size_t(mLast) - size_t(mBegin); }

        inline iterator begin() { return mBegin; }
        inline iterator end() { return mLast; }
        inline const_iterator begin() const { return mBegin; }
        inline const_iterator end() const { return mLast; }
        inline size_t size() const { return Size(); }
        inline T *data() { return Data(); }
        inline const T *data() const { return Data(); }
        inline bool empty() { return Empty(); }

        inline const T &Front() const
        {
            assert(!Empty());
            return *Begin();
        }

        inline const T &Back() const
        {
            assert(!Empty());
            return *ReverseBegin();
        }

		inline bool Equal(Range<T> right) const
		{
			size_t size = Size();
			if(size != right.Size())
				return false;

			return memcmp(mBegin, right.mBegin, size) == 0;
		}

		inline bool operator==(Range<T> right) const
		{
			return Equal(right);
		}

    protected:
        T *mBegin;
        T *mLast;
    };
}
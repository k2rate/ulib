#pragma once

#include "baseiterator.h"
#include <iterator>

namespace ulib
{
	template<class T>
	struct RandomAccessIterator : public BaseIterator<T, std::random_access_iterator_tag>
	{
		inline RandomAccessIterator() {}
		inline RandomAccessIterator(T* p) : ptr(p) {}
		inline RandomAccessIterator(const RandomAccessIterator& it) : ptr(it.ptr) {}

		inline bool operator!=(const RandomAccessIterator& it) const { return ptr != it.ptr; }
		inline bool operator==(const RandomAccessIterator& it) const { return ptr == it.ptr; }

		inline T& operator*() const { return *ptr; }
		inline RandomAccessIterator& operator++() { ++ptr; return *this; }       // Prefix increment operator.
		inline RandomAccessIterator& operator++(int) { ptr++; return *this; }     // Postfix increment operator.
		inline RandomAccessIterator& operator--() { --ptr; return *this; }
		inline RandomAccessIterator& operator--(int) { ptr--; return *this; }

		inline RandomAccessIterator operator+(int k) const { return ptr + k; }
		inline RandomAccessIterator operator-(int k) const { return ptr - k; }

		inline RandomAccessIterator operator+=(int k) { return ptr += k; }
		inline RandomAccessIterator operator-=(int k) { return ptr -= k; }

		inline bool operator>(RandomAccessIterator k) const { return ptr > k.ptr; }
		inline bool operator<(RandomAccessIterator k) const { return ptr < k.ptr; }
		inline bool operator<=(RandomAccessIterator k) const { return ptr <= k.ptr; }
		inline bool operator>=(RandomAccessIterator k) const { return ptr >= k.ptr; }
		inline bool operator==(RandomAccessIterator k) const { return ptr == k.ptr; }

		// BinaryIterator operator+(BinaryIterator<T> ky) const { return (T*)(ptrdiff_t(ptr) + ptrdiff_t(ky.ptr)); }
		inline size_t operator-(RandomAccessIterator k) const { return ptr - k.ptr; }

		inline T* operator->() const { return ptr; }

		inline T* Raw() { return ptr; }

		T* ptr;
	};
}
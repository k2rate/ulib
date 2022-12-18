#pragma once

#include "baseiterator.h"

namespace ulib
{
	template<class T>
	struct BinaryIterator : public BaseIterator<T, void>
	{
		inline BinaryIterator() {}
		inline BinaryIterator(T* p) : ptr(p) {}
		inline BinaryIterator(const BinaryIterator& it) : ptr(it.ptr) {}

		inline bool operator!=(const BinaryIterator& it) const { return ptr != it.ptr; }
		inline bool operator==(const BinaryIterator& it) const { return ptr == it.ptr; }

		inline T& operator*() const { return *ptr; }
		inline BinaryIterator& operator++() { ++ptr; return *this; }       // Prefix increment operator.
		inline BinaryIterator& operator++(int) { ptr++; return *this; }     // Postfix increment operator.
		inline BinaryIterator& operator--() { --ptr; return *this; }
		inline BinaryIterator& operator--(int) { ptr--; return *this; }

		inline BinaryIterator operator+(int k) const { return ptr + k; }
		inline BinaryIterator operator-(int k) const { return ptr - k; }

		inline BinaryIterator operator+=(int k) { return ptr += k; }
		inline BinaryIterator operator-=(int k) { return ptr -= k; }

		inline bool operator>(BinaryIterator k) const { return ptr > k.ptr; }
		inline bool operator<(BinaryIterator k) const { return ptr < k.ptr; }
		inline bool operator<=(BinaryIterator k) const { return ptr <= k.ptr; }
		inline bool operator>=(BinaryIterator k) const { return ptr >= k.ptr; }
		inline bool operator==(BinaryIterator k) const { return ptr == k.ptr; }

		// BinaryIterator operator+(BinaryIterator<T> ky) const { return (T*)(ptrdiff_t(ptr) + ptrdiff_t(ky.ptr)); }
		inline size_t operator-(BinaryIterator k) const { return ptr - k.ptr; }

		inline T* operator->() const { return ptr; }

		inline T* Raw() { return ptr; }

		T* ptr;
	};
}
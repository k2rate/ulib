#pragma once

#include "baseiterator.h"
#include <iterator>

namespace ulib
{
	template<class T>
	struct RandomAccessIterator : public BaseIterator<T, std::random_access_iterator_tag>
	{
        using SelfT = RandomAccessIterator<T>;

		inline RandomAccessIterator() {}
		inline RandomAccessIterator(T* p) : ptr(p) {}
		inline RandomAccessIterator(const SelfT& it) : ptr(it.ptr) {}

		inline T& operator*() const { return *ptr; }
		inline SelfT& operator++() { ++ptr; return *this; }       // Prefix increment operator.
		inline SelfT& operator++(int) { ptr++; return *this; }     // Postfix increment operator.
		inline SelfT& operator--() { --ptr; return *this; }
		inline SelfT& operator--(int) { ptr--; return *this; }

		inline SelfT operator+(size_t k) const { return ptr + k; }
		inline SelfT operator-(size_t k) const { return ptr - k; }

		inline SelfT operator+=(size_t k) { return ptr += k; }
		inline SelfT operator-=(size_t k) { return ptr -= k; }

		inline bool operator>(SelfT k) const { return ptr > k.ptr; }
		inline bool operator<(SelfT k) const { return ptr < k.ptr; }
		inline bool operator<=(SelfT k) const { return ptr <= k.ptr; }
		inline bool operator>=(SelfT k) const { return ptr >= k.ptr; }
		inline bool operator==(SelfT k) const { return ptr == k.ptr; }
        inline bool operator!=(SelfT k) const { return ptr != k.ptr; }

		inline size_t operator-(SelfT k) const { return ptr - k.ptr; }
		inline T* operator->() const { return ptr; }
		inline T* Raw() { return ptr; }

		T* ptr;
	};
}
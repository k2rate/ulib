#pragma once

#include <assert.h>

namespace ulib
{
	class DefaultAllocator
	{
	public:
		struct Params
		{
		};

		DefaultAllocator(Params params = {})
		{
		}

		inline void *Alloc(size_t size) { return new char[size]; }
		inline void Free(void *page)
		{
			assert(page);
			delete[] page;
		}

	private:
	};
}

inline void *operator new(size_t size, ulib::DefaultAllocator &al) { return al.Alloc(size); }
inline void operator delete(void *ptr, ulib::DefaultAllocator &al) { return al.Free(ptr); }
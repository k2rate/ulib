#pragma once

#include "baseresource.h"
#include <assert.h>

namespace ulib
{
	template<class AllocatorT>
	class DebugResource : public BaseResource<AllocatorT>
	{
	public:
		inline DebugResource(typename AllocatorT::Params params)
			: BaseResource<AllocatorT>(params)
		{
			mUsage = 0;
		}

		inline ~DebugResource()
		{
			assert(mUsage == 0);
		}

		inline void* Alloc(size_t size) 
		{ 
			size_t eq = size + sizeof(size_t);
			size_t* s = (size_t*)BaseResource<AllocatorT>::Alloc(eq);

			*s = eq;
			mUsage += eq;
			return s + 1;
		}

		inline void Free(void* ptr) 
		{
			size_t* page = (size_t*)ptr - 1;
			size_t size = *page;

			BaseResource<AllocatorT>::Free(page); 

			mUsage -= size;
		}

	protected:

		size_t mUsage;
	};
}
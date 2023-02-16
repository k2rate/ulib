#pragma once

#include "baseresource.h"
#include <assert.h>

namespace ulib
{
    struct DebugPageHeader
    {
        size_t pageSize;
        size_t hash;
    };

    struct DebugPageFooter
    {
        size_t hash;
    };

	template <class AllocatorT>
	class DebugResource : public BaseResource<AllocatorT>
	{
	public:
		inline DebugResource()
		{
			mUsage = 0;
		}

		inline DebugResource(typename AllocatorT::Params params)
			: BaseResource<AllocatorT>(params)
		{
			mUsage = 0;
		}

		inline DebugResource(const DebugResource<AllocatorT> &source)
			: BaseResource<AllocatorT>(source)
		{
			mUsage = 0;
		}

		inline DebugResource(DebugResource &&source)
			: BaseResource<AllocatorT>(std::move(source))
		{
			mUsage = source.mUsage;
			source.mUsage = 0;
		}

		inline ~DebugResource()
		{
			assert(mUsage == 0 && "resource memory usage must be == 0 before destroying resource");
		}

		inline DebugResource<AllocatorT> &operator=(DebugResource<AllocatorT> &&source)
		{
			BaseResource<AllocatorT> &rs = *this;
			rs = std::move(source);

            assert(mUsage == 0 && "resource memory usage must be == 0 before moving resource");

            mUsage = source.mUsage;
			source.mUsage = 0;

			return *this;
		}

		inline void *Alloc(size_t size)
		{
			size_t eq = size + sizeof(DebugPageHeader) + sizeof(DebugPageFooter);

			DebugPageHeader *header = (DebugPageHeader *)BaseResource<AllocatorT>::Alloc(eq);
            assert(header != nullptr && "nullptr returned in BaseResource<AllocatorT>::Alloc");

            DebugPageFooter* footer = (DebugPageFooter*)((char*)header + (size + sizeof(DebugPageHeader)));      

            header->pageSize = eq;
            header->hash = eq + rand() + rand();      
            footer->hash = header->hash;

			mUsage += eq;
			return header + 1;
		}

		inline void Free(void *ptr)
		{
            DebugPageHeader *header = (DebugPageHeader *)ptr - 1;
            DebugPageFooter* footer = (DebugPageFooter*)((char*)header + (header->pageSize - sizeof(DebugPageFooter)));    

            assert(mUsage >= header->pageSize && "current resource memory usage must be bigger or equal than size of page to free");

            size_t headerHash = header->hash;
            size_t footerHash = footer->hash;

            assert(headerHash == footerHash && "heap corruption detected by ulib::DebugResource");

            mUsage -= header->pageSize;

			BaseResource<AllocatorT>::Free(header);
		}

	protected:
		size_t mUsage;
	};
}
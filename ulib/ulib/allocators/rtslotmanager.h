#pragma once

#include <ulib/types.h>
#include <assert.h>

namespace ulib
{
	class RtSlotManager
	{
	public:
		struct Page
		{
			// next free
			Page *next;
			rawptr_t begin;
			rawptr_t end;
		};

		struct Block
		{
			// next free
			Block *next;
		};

		static constexpr inline size_t kPageHeaderSize = sizeof(Page);
		static constexpr inline size_t kBlockHeaderSize = sizeof(Block);

		RtSlotManager(size_t slotSize = 0)
		{
			mBegin = nullptr;
			mLast = nullptr;
			mEnd = nullptr;

			mPageFree = nullptr;
			mBlockFree = nullptr;

			mSlotSize = slotSize;
		}

		~RtSlotManager() 
		{
			
		}

		

		// using 24 bytes for header;
		inline void AddFreePage(void *begin, void *end)
		{
			rawptr_t b = rawptr_t(begin);
			rawptr_t e = rawptr_t(end);

			assert(e > b);
			assert((size_t(e) - size_t(b)) >= kPageHeaderSize);
			assert((size_t(e) - size_t(b)) >= mSlotSize);

			Page *page = (Page *)b;

			page->begin = b;
			page->end = e;

			page->next = mPageFree;
			mPageFree = page;

			
		}

		inline void AddFreePage(void* ptr, size_t size)
		{
			AddFreePage(ptr, (char*)ptr + size);
		}

		// alloc from raw memory
		void *AllocRaw()
		{
			rawptr_t block = mLast;

			// check out of raw memory
			if (block != mEnd)
			{
				mLast += mSlotSize;
				return block;
			}

			// attempt switch page and alloc from that
			if (Page *page = mPageFree)
			{
				mBegin = page->begin;
				mLast = page->begin;
				mEnd = page->end;

				// check what some idiot put empty page here
				assert(mLast != mEnd);

				mPageFree = page->next;

				block = mLast;
				mLast += mSlotSize;
				return block;
			}

			return nullptr;
		}

		inline void *Alloc()
		{
			// alloc from freed memory
			if (Block *block = mBlockFree)
			{
				mBlockFree = block->next;
				return block;
			}

			return AllocRaw();
		}

		inline void Free(void *data)
		{
			Block *block = (Block *)data;
			block->next = mBlockFree;
			mBlockFree = block;
		}

		inline size_t SlotSize()
		{
			return mSlotSize;
		}

	protected:
		rawptr_t mBegin;
		rawptr_t mLast;
		rawptr_t mEnd;

		Page *mPageFree;
		Block *mBlockFree;

		size_t mSlotSize;
	};

}
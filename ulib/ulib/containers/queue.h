#pragma once

#include "list.h"

namespace ulib
{
	template <class T, class AllocatorT = DefaultAllocator>
	class FastQueue : private List<T, AllocatorT>
	{
	public:
		using ListT = List<T, AllocatorT>;
		using AllocatorParams = typename AllocatorT::Params;

		FastQueue(AllocatorParams params = {})
			: ListT(params), mIdx(0)
		{
		}

		FastQueue(FastQueue &&source)
			: ListT(std::move(source)), mIdx(source.mIdx)
		{
		}

		FastQueue& operator=(FastQueue &&source)
		{
			ListT& lt = *this;
			lt = std::move(source);

			mIdx = source.mIdx;
		}

		inline void Push(const T &o) { ListT::PushBack(o); }
		inline void Push(T &&o) { ListT::PushBack(o); }

		inline size_t Size() { return ListT::Size(); }
		inline void Clear()
		{
			ListT::Clear();
			mIdx = 0;
		}

		inline bool Peek(T &out)
		{
			size_t size = ListT::Size();
			if (!size)
				return false;

			if (mIdx == size)
			{
				ListT::Clear();
				return false;
			}

			out = std::move(ListT::At(mIdx));
			mIdx++;

			return true;
		}

	private:
		size_t mIdx;
	};
}
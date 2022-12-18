#pragma once

#include <assert.h>
#include <ulib/containers/buffer.h>

namespace ulib
{
	template<class Container>
	class Walker
	{
	public:

		using iterator = typename Container::iterator;
		using Iterator = iterator;
		using value_type = typename Container::value_type;

		Walker(Container* cont)
			: mContainer(cont)
		{
			mIt = mContainer->Begin();
		}

		~Walker()
		{

		}

		Iterator GetPos() { return mIt; }
		void SetPos(Iterator it)
		{
			mIt = it;
		}

		inline Iterator Current()
		{
			return mIt;
		}

		inline void Reset()
		{
			mIt = mContainer->Begin();
		}

		inline Iterator Walk(size_t count)
		{
			Iterator it = mIt;
			mIt += count;
			assert(mIt <= mContainer->End());
			return it;
		}

		size_t Left() { return mContainer->End() - mIt; }
		size_t Index() { return mIt - mContainer->Begin(); }

	private:
		Container* mContainer;
		Iterator mIt;
	};

}
#pragma once

namespace ulib
{
	template <class AllocatorT>
	class BaseResource : public AllocatorT
	{
	public:
		BaseResource(AllocatorT::Params params)
			: AllocatorT(params)
		{}
	};
}
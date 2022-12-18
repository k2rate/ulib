#pragma once

namespace ulib
{
	template <class AllocatorT>
	class BaseResource : public AllocatorT
	{
	public:
		BaseResource(typename AllocatorT::Params params)
			: AllocatorT(params)
		{}
	};
}
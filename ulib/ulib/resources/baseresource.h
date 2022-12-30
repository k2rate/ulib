#pragma once

#include <type_traits>

namespace ulib
{
	template <class AllocatorT>
	class BaseResource : public AllocatorT
	{
	public:
		inline BaseResource(typename AllocatorT::Params params)
			: AllocatorT(params)
		{}

		inline BaseResource(BaseResource<AllocatorT>&& source)
			: AllocatorT(std::move(source))
		{}

		inline BaseResource(const BaseResource<AllocatorT>& source)
			: AllocatorT(source)
		{}

		inline BaseResource<AllocatorT>& operator=(BaseResource<AllocatorT>&& source)
		{
			AllocatorT& al = *this;
			al = std::move(source);
			return *this;
		}
	};
}
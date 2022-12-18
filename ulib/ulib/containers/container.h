#pragma once

#include <ulib/resources/resource.h>

namespace ulib
{
	template<class T, class IteratorT>
	class Container : public Resource
	{
	public:
		Container(ULIB_RESOURCE_PARAMS) 
			: Resource(ULIB_RESOURCE_ARGS)
		{}

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using Iterator = IteratorT;
		using iterator = Iterator;
	};
}
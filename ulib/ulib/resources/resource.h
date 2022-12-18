#pragma once

#ifdef _DEBUG
#include "debugresource.h"

namespace ulib
{
	template<class AllocatorT>
	using Resource = DebugResource<AllocatorT>;
}

#else
#include "rawresource.h"

namespace ulib
{
	template<class AllocatorT>
	using Resource = RawResource<AllocatorT>;
}

#endif
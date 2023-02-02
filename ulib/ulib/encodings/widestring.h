#include "converter.h"

#ifdef _WIN32
#include "utf16/encoding.h"
namespace ulib
{
	using WideEncoding = Utf16;
}
#else
#include "utf32/encoding.h"
namespace ulib
{
	using WideEncoding = Utf32;
}
#endif

namespace ulib
{
	template<class EncodingT, class AllocatorT = DefaultAllocator>
	inline BasicString<wchar_t, AllocatorT> ToWideString(EncodedStringView<EncodingT> str, typename AllocatorT::Params al = {})
	{
		BasicString<wchar_t, AllocatorT> result(str.size() * 4, al);

		auto end = ulib::Convert<EncodingT, WideEncoding>(str.data(), str.data() + str.size(), result.data());
        result.SetSize(end - result.data());

		return result;
	}
}
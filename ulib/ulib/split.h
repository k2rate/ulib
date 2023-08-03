#pragma once

#include <ulib/encodings/type.h>
#include <stdexcept>
#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/encodings/literalencoding.h>
#include <ulib/typetraits/encoding.h>
#include <ulib/containers/splitview.h>

namespace ulib
{
    template <class EncodingT>
    inline SplitView<EncodedStringView<EncodingT>> split_impl(EncodedStringView<EncodingT> str, EncodedStringView<EncodingT> sep)
    {
        return SplitView<EncodedStringView<EncodingT>>(str, sep);
    }

    template <class StringT, class SeparatorStringT, class EncodingT = argument_encoding_t<StringT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void> && std::is_same_v<EncodingT, argument_encoding_t<SeparatorStringT>>, bool> = true>
    inline SplitView<EncodedStringView<EncodingT>> split(const StringT &str, const SeparatorStringT &sep)
    {
        return split_impl(ulib::EncodedStringView<EncodingT>(str), ulib::EncodedStringView<EncodingT>(sep));
    }

} // namespace ulib
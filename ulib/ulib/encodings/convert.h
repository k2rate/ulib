#pragma once

#include "cconvert.h"

#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>

#include "type.h"

namespace ulib
{
    template <class AllocatorT = DefaultAllocator>
    struct Converter
    {
        template <class FromEncodingT, class ToEncodingT>
        static EncodedString<ToEncodingT, AllocatorT> Convert(EncodedStringView<FromEncodingT> str, typename AllocatorT::Params al = {})
        {
            static_assert(FromEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");
            static_assert(ToEncodingT::kType != EncodingType::Raw, "It is not possible to convert raw encoding");

            EncodedString<ToEncodingT, AllocatorT> result(str.size() * 4, al);

            auto end = ulib::Convert<FromEncodingT, ToEncodingT>(str.data(), str.data() + str.size(), result.data());
            result.SetSize(end - result.data());

            return result;
        }
    };

    template <class FromEncodingT, class ToEncodingT>
    static EncodedString<ToEncodingT> Convert(EncodedStringView<FromEncodingT> str)
    {
        return Converter<>::Convert<FromEncodingT, ToEncodingT>(str);
    }
}
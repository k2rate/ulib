#pragma once

#include <type_traits>

namespace ulib
{
    enum class EncodingType
    {
        Raw = 0,
        Concrete = 1,
    };

    template <class SourceEncodingT, class EncodingT>
    inline constexpr bool IsParentCompatible =
        std::is_same_v<SourceEncodingT, typename EncodingT::ParentEncodingT> ||
        std::is_same_v<typename SourceEncodingT::ParentEncodingT, EncodingT> ||
        std::is_same_v<SourceEncodingT, EncodingT>;
}
#pragma once

#include <ulib/typetraits/field.h>

namespace ulib
{
    ULIB_DEFINE_TYPE_FIELD_CHECK(ulib_container_tag, ContainerTagT);

    template <class T>
    inline constexpr bool is_ulib_container_v = has_ulib_container_tag_v<T>;
}
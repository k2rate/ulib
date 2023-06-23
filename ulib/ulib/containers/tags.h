#pragma once

namespace ulib
{
    enum class ContainerTag
    {
        None,
        String,
        List,
        Map
    };

    struct string_container_tag {};
    struct list_container_tag {};
    struct map_container_tag {};

} // namespace ulib
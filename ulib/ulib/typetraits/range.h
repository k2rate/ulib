#pragma once

#include <ulib/typetraits/container.h>

namespace ulib
{
    template <class T, class K>
    struct container_range_compatible
    {
        using value_type = container_value_type_t<T>;
        using k_value_type = container_value_type_t<K>;

        static constexpr bool is_tags_compatible =
            has_container_iterator_tag_v<T> && is_container_iterator_tag_v<T, container_iterator_tag_t<K>>;

        // static constexpr bool is_const_compatible = std::is_const_v<value_type> || (!std::is_const_v<value_type> && !std::is_const_v<k_value_type>) ;
        static constexpr bool is_type_compatible = std::is_same_v<std::remove_cv_t<value_type>, std::remove_cv_t<k_value_type>>;

        static constexpr bool value = is_tags_compatible && is_type_compatible;
    };

    template <class T, class K>
    inline constexpr bool is_container_range_compatible_v = container_range_compatible<T, K>::value;

    template <class T, class K>
    inline constexpr bool is_different_container_range_compatible_v =
        !std::is_same_v<T, K> && is_container_range_compatible_v<T, K>;

    template <class T, class K>
    using enable_if_range_compatible_t = std::enable_if_t<is_different_container_range_compatible_v<T, K>, bool>;

} // namespace ulib

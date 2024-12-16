#pragma once

#include <ulib/containers/list.h>

namespace ulib
{
    template <class ContT, class LambdaT, class ValueT = typename ContT::value_type, class GroupT = std::invoke_result_t<LambdaT, ValueT>,
              class AllocatorT = constainer_choose_ulib_allocator_t<ContT>>
    List<std::pair<GroupT, List<ValueT, AllocatorT>>, AllocatorT> group_by(const ContT &cont, LambdaT pred)
    {
        List<std::pair<GroupT, List<ValueT, AllocatorT>>, AllocatorT> result;

        auto find_package = [&](const GroupT &gr) -> std::pair<GroupT, List<ValueT, AllocatorT>> * {
            for (auto &package : result)
            {
                if (package.first == gr)
                    return &package;
            }

            return nullptr;
        };

        auto get_package = [&](const GroupT &gr) -> std::pair<GroupT, List<ValueT, AllocatorT>> & {
            if (std::pair<GroupT, List<ValueT, AllocatorT>> *pPackage = find_package(gr))
                return *pPackage;

            auto &package = result.emplace_back();
            package.first = gr;

            return package;
        };

        for (auto &info : cont)
        {
            auto &package = get_package(pred(info));
            package.second.push_back(info);
        }

        return result;
    }
} // namespace ulib
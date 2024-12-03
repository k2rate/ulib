
#include <iostream>
#include <ulib/chrono.h>
#include <ulib/encodings/w1251/string.h>
#include <ulib/string.h>
#include <ulib/strutility.h>

#include <ulib/containers/filterview.h>
#include <ulib/containers/mapview.h>

namespace ulib
{
    namespace detail
    {
        template <typename Sig, typename C>
        inline Sig C::*overload_resolve_v(std::false_type, Sig C::*mem_func_ptr)
        {
            return mem_func_ptr;
        }

        template <typename Sig, typename C>
        inline Sig C::*overload_resolve_v(std::true_type, Sig C::*mem_variable_ptr)
        {
            return mem_variable_ptr;
        }
    } // namespace detail

    template <typename Sig, typename C>
    inline Sig C::*overload(Sig C::*mem_ptr)
    {
        return detail::overload_resolve_v(std::is_member_object_pointer<Sig C::*>(), mem_ptr);
    }
} // namespace ulib

int main()
{
    // int (TMyClass::*pt2ConstMember)(float, char, char) const

    // ulib::string_view text0 = "111text111";
    // printf("%s\n", ulib::str(text0.strip("1")).c_str());

    // ulib::string str0 = "text";

    ulib::list<ulib::string> strs = {"one", "two", "thr ee", "things", "foo bar"};

    auto predicate = [](const ulib::string &str, const ulib::string &sep) {
        std::printf("[Invoking predicate on '%s']\n", str.c_str());
        return str + sep + str;
    };

    printf("Lazy-mapped:\n");

    auto strs_view = strs.ToSpan();

    auto view = strs_view.map(predicate, ".").map(predicate, "_"); // ulib::MapView{ulib::MapView{strs, predicate, "."}, predicate, "_"};

    for (auto s : view)
    {
        printf("%s\n", ulib::string{s}.data());
    }

    printf("\n");

    strs.transform(&ulib::string::push_back, '?');

    for (auto strs : strs.map(&ulib::string::split, " "))
    {
        for (auto v : strs)
            printf("v: %s\n", ulib::string{v}.c_str());
    }

    for (auto size : strs.map(&ulib::string::size))
    {
        printf("size: %llu\n", size);
    }

    auto pred = [](const ulib::string &str) { return str.starts_with("tw"); };

    strs.filter_lazy(pred).transform(&ulib::string::push_back, '!');

    for (auto b : strs.filter(pred).map(&ulib::string::c_str))
    {
        printf("starts_with: %s\n", b);
    }

    // ulib::string str1(str0.begin(), str0.end());

    // ulib::list<int> vals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    // constexpr int ky = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13;

    // auto filterVals = vals.filter([](auto v) { return v <= 8; });
    // auto reduceVals = vals.reduce([](auto left, auto right) { return left + right; });
    // auto mapVals = vals.map([](auto v) { return v + 10; });

    // auto printList = [](ulib::list<int> &list) {
    //     ulib::string result;
    //     for (auto obj : list)
    //     {
    //         result.append(std::to_string(obj) + "-");
    //     }

    //     return result;
    // };

    // printf("filterVals: %s\n", printList(filterVals).c_str());
    // printf("reduceVals: %d\n", reduceVals);
    // printf("mapVals: %s\n", printList(mapVals).c_str());

    return 0;
}
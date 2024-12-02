
#include <iostream>
#include <ulib/chrono.h>
#include <ulib/encodings/w1251/string.h>
#include <ulib/string.h>
#include <ulib/strutility.h>


int main()
{
    // int (TMyClass::*pt2ConstMember)(float, char, char) const

    // ulib::string_view text0 = "111text111";
    // printf("%s\n", ulib::str(text0.strip("1")).c_str());

    // ulib::string str0 = "text";

    ulib::list<ulib::string> strs = {"one", "two", "three"};

    strs.transform(&ulib::string::push_back, '?');

    for (const char *cstr : strs.map(&ulib::string::c_str))
    {
        printf("%s\n", cstr);
    }

    for (const char *cstr : strs.map(&ulib::string::c_str))
    {
        printf("%s\n", cstr);
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
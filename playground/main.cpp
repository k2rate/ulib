
#include <iostream>
#include <ulib/chrono.h>
#include <ulib/strutility.h>
#include <ulib/string.h>

int main()
{
    ulib::list<ulib::u8string> list{u8"one", u8"two", u8"three"};
    auto result = ulib::join(list, u8" | ");

    printf("%s\n", result.c_str());


    return 0;
}
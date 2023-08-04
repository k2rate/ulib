#include <string>
#include <ulib/string.h>

int main()
{
        

    auto result = ulib::u8(ulib::str(::ulib::u8(ulib::str(ulib::u8(
        ulib::str(::ulib::u8(ulib::str(u8"hello nigger"))))))));

    // void* mem = __builtin_operator_new(200);
    // memcpy(mem, "hello nigger", sizeof("hello nigger"));

    printf("str: %s\n", result);

    return 0;
}
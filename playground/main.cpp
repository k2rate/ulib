
#include <ulib/string.h>

int main()
{
    ulib::u8string str = u8"";
    ulib::string_view vstr = str;

    char16_t kUtf16Str[] = u"sfasdf";
    ulib::u16(kUtf16Str);

    return 0;
}
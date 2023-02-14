#include <ulib/string.h>
#include <ulib/wchar.h>
#include <ulib/u8.h>
#include <ulib/u16.h>
#include <ulib/u32.h>

#include <ulib/list.h>

#include <vector>

#include <windows.h>

int main()
{

    {
        ulib::u8string test = u8"пиздец нахуй";
        ulib::u16string test2 = u"ахуеть блять";
        ulib::u32string test3 = U"впизду крч";

        ulib::wstring w1 = ulib::wstr(test);
        ulib::wstring w2 = ulib::wstr(test2);
        ulib::wstring w3 = ulib::wstr(test3);

        ulib::u8string test0 = u8"пиздец нахуй";
        ulib::string str = ulib::str(test); // u8 to multibyte
        // ulib::str(test2); // error (u16 to multibyte)

        std::string sstr;

        test = ulib::u8(test);
        test = ulib::u8(test2);
        test = ulib::u8(test3);
        test = ulib::u8(w1);
        test = ulib::u8(str);

        test = ulib::u8("ky");
        test = ulib::u8(L"чайник");
        test = ulib::u8(u8"чайник");
        test = ulib::u8(u"чайник");
        test = ulib::u8(U"чайник");

        test = ulib::u8(std::string("ky"));
        test = ulib::u8(std::wstring(L"ky"));
        test = ulib::u8(std::u8string(u8"ky"));
        test = ulib::u8(std::u16string(u"ky"));
        test = ulib::u8(std::u32string(U"ky"));

        ulib::string_view view = "niggers";
        ulib::wstring_view wview = L"niggers";
        ulib::u8string_view u8view = u8"негры блять";
        ulib::u16string_view u16view = u"негры блять";
        ulib::u32string_view u32view = U"негры блять";

        test = ulib::u8(view);
        test = ulib::u8(wview);
        test = ulib::u8(u8view);
        test = ulib::u8(u16view);
        test = ulib::u8(u32view);

        std::string_view sview = "niggers";
        std::wstring_view swview = L"niggers";
        std::u8string_view su8view = u8"негры блять";
        std::u16string_view su16view = u"негры блять";
        std::u32string_view su32view = U"негры блять";

        test = ulib::u8(sview);
        test = ulib::u8(swview);
        test = ulib::u8(su8view);
        test = ulib::u8(su16view);
        test = ulib::u8(su32view);

        test = ulib::Convert<ulib::Utf8>(test);
        test = ulib::Convert<ulib::Utf8>(su8view);
        test = ulib::Convert<ulib::Utf8>(view);
        test = ulib::Convert<ulib::Utf8>("adfdsafdsafas");
        test = ulib::Convert<ulib::Utf8>(L"да");

        auto tttt = ulib::u8(ulib::u16(ulib::u32(ulib::u8(ulib::str(ulib::u8(ulib::wstr(u8"полный ку")))))));

        auto ww = ulib::wstr(tttt);
        MessageBoxW(0, (wchar_t*)ww.c_str(), L"Check", MB_OK);
        

    }

    {
        ulib::u8string u8str = u8"хуй";
        ulib::u16string u16str = u"хуй";
        ulib::u32string u32str = U"хуй";
        ulib::string str = "pizdec";
        ulib::wstring wstr = L"пиздец";

        char8 u8ch[] = u8"пиздец";
        char16 u16ch[] = u"пиздец";
        char32 u32ch[] = U"пиздец";
        char ch[] = "pizdec";
        wchar_t wch[] = L"пиздец";

        u8str = u8ch;
        u8str = ulib::u8string(u8ch);
        u16str = u16ch;
        u16str = ulib::u16string(u16ch);
        u32str = u32ch;
        u32str = ulib::u32string(u32ch);
        str = ch;
        str = ulib::string(ch);
        wstr = wch;
        wstr = ulib::wstring(wch);

        u8str = ulib::u8string(std::begin(u8ch), std::end(u8ch));
        u16str = ulib::u16string(std::begin(u16ch), std::end(u16ch));
        u32str = ulib::u32string(std::begin(u32ch), std::end(u32ch));
        str = ulib::string(std::begin(ch), std::end(ch));
        wstr = ulib::wstring(std::begin(wch), std::end(wch));

        u8str = ulib::u8string(std::begin(u8ch), std::size(u8ch));
        u16str = ulib::u16string(std::begin(u16ch), std::size(u16ch));
        u32str = ulib::u32string(std::begin(u32ch), std::size(u32ch));
        str = ulib::string(std::begin(ch), std::size(ch));
        wstr = ulib::wstring(std::begin(wch), std::size(wch));

        str = u8str;
        // u8str = str; error

        std::string sstr = str;
        std::u8string su8str = u8str;
        std::u16string su16str = u16str;
        std::u32string su32str = u32str;

        // std::u16string su16str = u32str; error

        std::string sstr2 = u8str;
        // std::string sstr3 = u16str; // error

        ulib::BasicString<char> bstr = sstr;
        ulib::string tstr = sstr;
        tstr = sstr;
    }

    // ulib::StrSplit("test", "s");

    {
        ulib::string_view ky = "pozor1";
        ulib::string_view ky2 = "pozor2";

        ulib::u8string str;

        ky == ky2;
    }

    try
    {
        {

            {
                {
                    ulib::u8string test = u8"пиздец нахуй";
                    ulib::u16string test2 = u"ахуеть блять";
                    ulib::u32string test3 = U"впизду крч";

                    ulib::wstring w1 = ulib::wstr(test);
                    ulib::wstring w2 = ulib::wstr(test2);
                    ulib::wstring w3 = ulib::wstr(test3);

                    // ulib::ToWideString<ulib::Utf8>(ulib::WideStringTo<ulib::Utf8>(L"Широкий пиздец"));

                    auto full = w1 + L"\n" + w2 + L"\n" + w3 + L"\n" + ulib::wstr(ulib::u8(L"Широкий пиздец"));
                    MessageBoxW(0, full.c_str(), L"ky", MB_OK);
                }

                ulib::List<char> list = {'p', 'o', 'z', 'o', 'r'};
                ulib::String test = list;
                printf("test: %s\n", test.c_str());

                std::vector<char> list0 = {'p', 'o', 'z', 'o', 'r'};
                ulib::String test0 = list0;
                printf("test0: %s\n", test0.c_str());

                ulib::String str = "afjlkdsajfdksa";
                ulib::List<char> tt = str;
                ulib::String ky{std::string("pizdec")};

                ulib::u8string u8str;
                // u8str = str;
            }

            {

                ulib::String test;
                std::string_view tests = test;

                ulib::Range<const char> rr = tests;

                ulib::String blyat = ulib::Range<const typename std::string_view::value_type>(tests);
                ulib::String test2 = rr;
                ulib::String pzr = tests;

                blyat.Append("pads");
            }

            std::string_view ky = ulib::u8string(u8"test");

            ulib::String tstr{"ky"};

            ulib::u8string str8 = u8"пиздец сука заебался нахуй я аниме смотреть блять";
            ulib::u16string str16 = u"пиздец сука заебался нахуй я аниме смотреть блять";
            ulib::u32string str32 = U"пиздец сука заебался нахуй я аниме смотреть блять";

            // auto out = ulib::format<ulib::Utf16>(u8"Utf8: {}\nUtf16: {}\nUtf32: {}\n", str8, str16, str32);
            ulib::u16string out;

            MessageBoxW(0, (wchar_t *)out.c_str(), L"хуй", MB_OK);

            ulib::u8string_view vv = u8"пиздец";
            ulib::u8string str = vv;

            ulib::string_view vs = "pizdec";
            ulib::string rstr = vs;

            /*
            std::string_view ky = "plakmp";
            ulib::StringView u = ky;

            ulib::u8string str = u8"пиздец сука заебался нахуй я аниме смотреть блять";
            ulib::u16string u16str = ulib::Convert<ulib::Utf8, ulib::Utf16>(str);

            */

            // printf("test: %s\n", str.data());

            /*
            ulib::u8string u8str = u8"u8str";
            ulib::u16string u16str = u"u16str";
            ulib::u32string u32str = U"u32str";

            // сомнительные сука вещи
            ulib::wstring wstr = L"";
            ulib::string str = "test string";

            wstr = u16str; // windows only
            wstr = u32str; // linux only

            ulib::path path;

            wstr = path; // windows
            str = path; // linux

            u8str = path; // linux
            u16str = path; // windows




            char buf[3] = "re";
            size_t test = std::end(buf) - std::begin(buf);
            printf("test: %d\n", int(test));
            */

            return 0;
        }
    }
    catch (const std::exception &e)
    {
        printf("exception: %s\n", e.what());
    }

    return 0;
}
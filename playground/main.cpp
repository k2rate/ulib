#include <ulib/string.h>
#include <ulib/wchar.h>
#include <ulib/u8.h>
#include <ulib/u16.h>
#include <ulib/u32.h>

#include <ulib/list.h>

#include <vector>

#include <windows.h>
#include <ulib/charcase.h>

#include <locale>
#include <map>

#include <ulib/sstr.h>

namespace ewin
{
    int messagebox(ulib::u8string_view message = u8"default text", ulib::u8string_view title = u8"message", int type = MB_OK, void *hwnd = nullptr)
    {
        return MessageBoxW(0, ulib::wstr(message).c_str(), ulib::wstr(title).c_str(), MB_OK);
    }
};

__declspec(noinline) void test()
{

    ulib::u8string str = ulib::Convert<ulib::Utf8>(ulib::u8string(u8"re"));
    // ulib::Convert<ulib::Utf8>(ulib::u16string(u"re"));
}

int main()
{
    std::setlocale(LC_ALL, ".utf8");

    u8"g";

    {
        ulib::u8string nig = u8"негры блять";
        nig += u8" ку";

        printf("[u8] string: %s size: %llu length: %llu\n",
               nig.c_str(), nig.size(), nig.length());

        ulib::u16string nigr = u"негры блять";
        printf("[u16] string: %s size: %llu length: %llu\n",
               ulib::u8(nigr).c_str(), nigr.size(), nigr.length());
    }

    {
        ulib::u8string test(u8"re");
    }

    {
        ulib::u8string test(u8"re");
        auto tt = std::move(test);
    }

    {
        ulib::u8string test(u8"re");
        ulib::u8string test2(u8"re");
        ulib::u8string test3(u8"re");

        auto tt = std::move(test);
        auto tt1 = std::move(test2);
        auto tt2 = std::move(test3);
    }

    {
        ulib::Convert<ulib::Utf8>(L"pizdec");
    }

    {
        ulib::Convert<ulib::Utf8>(ulib::string("re"));
        ulib::Convert<ulib::Utf8>(ulib::wstring(L"re"));
        ulib::Convert<ulib::Utf8>(ulib::u8string(u8"re"));
        ulib::Convert<ulib::Utf8>(ulib::u16string(u"re"));
        ulib::Convert<ulib::Utf8>(ulib::u32string(U"re"));
    }

    try
    {

        {
            ulib::string str = "pizdec ";
            std::string sstr = ulib::string_view("sdfafdafsda");

            str + sstr;
            str + ulib::string_view("str");
            ulib::string_view("str") + str;

            str += str;
            str += str;
            str += str;
            str += str;
            str += str;
            str += str;

            //
            {
                std::string s0 = ulib::sstr(ulib::u8string(u8"ky"));
                std::string s1 = ulib::sstr(ulib::u16string(u"ky"));
                std::string s2 = ulib::sstr(ulib::u32string(U"ky"));
                std::string s3 = ulib::sstr(ulib::string("ky"));
                std::string s4 = ulib::sstr(ulib::wstring(L"ky"));

                std::string s5 = ulib::sstr(ulib::u8string_view(u8"ky"));
                std::string s6 = ulib::sstr(ulib::u16string_view(u"ky"));
                std::string s7 = ulib::sstr(ulib::u32string_view(U"ky"));
                std::string s8 = ulib::sstr(ulib::string_view("ky"));
                std::string s9 = ulib::sstr(ulib::wstring_view(L"ky"));

                std::string s10 = ulib::sstr(u8"ky");
                std::string s11 = ulib::sstr(u"ky");
                std::string s12 = ulib::sstr(U"ky");
                std::string s13 = ulib::sstr("ky");
                std::string s14 = ulib::sstr(L"ky");

                std::string s15 = ulib::sstr(std::u8string(u8"ky"));
                std::string s16 = ulib::sstr(std::u16string(u"ky"));
                std::string s17 = ulib::sstr(std::u32string(U"ky"));
                std::string s18 = ulib::sstr(std::string("ky"));
                std::string s19 = ulib::sstr(std::wstring(L"ky"));

                std::string s20 = ulib::sstr(std::u8string_view(u8"ky"));
                std::string s21 = ulib::sstr(std::u16string_view(u"ky"));
                std::string s22 = ulib::sstr(std::u32string_view(U"ky"));
                std::string s23 = ulib::sstr(std::string_view("ky"));
                std::string s24 = ulib::sstr(std::wstring_view(L"ky"));
            }

            {
                std::wstring s0 = ulib::swstr(ulib::u8string(u8"ky"));
                std::wstring s1 = ulib::swstr(ulib::u16string(u"ky"));
                std::wstring s2 = ulib::swstr(ulib::u32string(U"ky"));
                std::wstring s3 = ulib::swstr(ulib::string("ky"));
                std::wstring s4 = ulib::swstr(ulib::wstring(L"ky"));

                std::wstring s5 = ulib::swstr(ulib::u8string_view(u8"ky"));
                std::wstring s6 = ulib::swstr(ulib::u16string_view(u"ky"));
                std::wstring s7 = ulib::swstr(ulib::u32string_view(U"ky"));
                std::wstring s8 = ulib::swstr(ulib::string_view("ky"));
                std::wstring s9 = ulib::swstr(ulib::wstring_view(L"ky"));

                std::wstring s10 = ulib::swstr(u8"ky");
                std::wstring s11 = ulib::swstr(u"ky");
                std::wstring s12 = ulib::swstr(U"ky");
                std::wstring s13 = ulib::swstr("ky");
                std::wstring s14 = ulib::swstr(L"ky");

                std::wstring s15 = ulib::swstr(std::u8string(u8"ky"));
                std::wstring s16 = ulib::swstr(std::u16string(u"ky"));
                std::wstring s17 = ulib::swstr(std::u32string(U"ky"));
                std::wstring s18 = ulib::swstr(std::string("ky"));
                std::wstring s19 = ulib::swstr(std::wstring(L"ky"));

                std::wstring s20 = ulib::swstr(std::u8string_view(u8"ky"));
                std::wstring s21 = ulib::swstr(std::u16string_view(u"ky"));
                std::wstring s22 = ulib::swstr(std::u32string_view(U"ky"));
                std::wstring s23 = ulib::swstr(std::string_view("ky"));
                std::wstring s24 = ulib::swstr(std::wstring_view(L"ky"));
            }
        }

        {
            std::map<ulib::u8string, ulib::u8string> map;
            map[u8"re"] = u8"meta";
            ewin::messagebox(u8"meta");
        }

        {
            ulib::wstring wstr = L"пиздец";

            // ulib::string_view view = "niggers";
            ulib::wstring_view wview = L"niggers ";
            ulib::u8string_view u8view = u8"негры блять ";
            ulib::u16string_view u16view = u"негры блять ";
            ulib::u32string_view u32view = U"негры блять ";

            // std::string_view sview = "niggers";
            std::wstring_view swview = L"niggers ";
            std::u8string_view su8view = u8"негры блять ";
            std::u16string_view su16view = u"негры блять ";
            std::u32string_view su32view = U"негры блять ";

            auto s0 = ulib::ToUpper(wview);
            auto s1 = ulib::ToUpper(u8view);
            auto s2 = ulib::ToUpper(u16view);
            auto s3 = ulib::ToUpper(u32view);

            auto s4 = ulib::ToUpper(swview);
            auto s5 = ulib::ToUpper(su8view);
            auto s6 = ulib::ToUpper(su16view);
            auto s7 = ulib::ToUpper(su32view);

            s0 = ulib::ToLower(wview);
            s1 = ulib::ToLower(u8view);
            s2 = ulib::ToLower(u16view);
            s3 = ulib::ToLower(u32view);

            s4 = ulib::ToLower(swview);
            s5 = ulib::ToLower(su8view);
            s6 = ulib::ToLower(su16view);
            s7 = ulib::ToLower(su32view);

            s0 = ulib::upper(wview);
            s1 = ulib::upper(u8view);
            s2 = ulib::upper(u16view);
            s3 = ulib::upper(u32view);

            s4 = ulib::upper(swview);
            s5 = ulib::upper(su8view);
            s6 = ulib::upper(su16view);
            s7 = ulib::upper(su32view);

            s0 = ulib::lower(wview);
            s1 = ulib::lower(u8view);
            s2 = ulib::lower(u16view);
            s3 = ulib::lower(u32view);

            s4 = ulib::lower(swview);
            s5 = ulib::lower(su8view);
            s6 = ulib::lower(su16view);
            s7 = ulib::lower(su32view);

            wstr = ulib::wstr(ulib::u8(s0) + ulib::u8(s1) + ulib::u8(s2) + ulib::u8(s3) +
                              ulib::u8(s4) + ulib::u8(s5) + ulib::u8(s6) + ulib::u8(s7));

            MessageBoxW(0, wstr.c_str(), L"re", MB_OK);
            MessageBoxW(0, ulib::ToLower(wstr).c_str(), L"re", MB_OK);

            std::string sstr;
            ulib::string u8str;

            sstr == u8str;
            u8str == sstr;
        }

        {
            ulib::string_view view = "test";
            ulib::wstring_view wview = L"test";
            ulib::u8string_view u8view = u8"test";
            ulib::u16string_view u16view = u"test";
            ulib::u32string_view u32view = U"test";

            ulib::string_view view1 = view;
            ulib::wstring_view wview1 = wview;
            ulib::u8string_view u8view1 = u8view;
            ulib::u16string_view u16view1 = u16view;
            ulib::u32string_view u32view1 = u32view;

            view1 = view;
            wview1 = wview;
            u8view1 = u8view;
            u16view1 = u16view;
            u32view1 = u32view;

            std::string sstr;
            std::u8string su8str;
            ulib::u8string u8str;
            ulib::u8string_view u8view2 = u8str;

            u8view = u8str;
            u8view = u8view;
            u8view = su8str;
            // u8view = sstr;
            // u8view = u16view;

            ulib::u8string u8str1(u8view);
            u8str = u8view;

            const ulib::u8string u8str22 = u8"asdokfdsaf";

            ulib::u8string uu = u8str + u8str1 + u8str22 + u8str22;
            uu += uu;
        }

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
            MessageBoxW(0, (wchar_t *)ww.c_str(), L"Check", MB_OK);
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
        }

        // ulib::StrSplit("test", "s");

        {
            ulib::string_view ky = "pozor1";
            ulib::string_view ky2 = "pozor2";

            ulib::u8string str;

            // ky == ky2;
        }
    }
    catch (const std::exception &e)
    {
        printf("exception: %s\n", e.what());
    }

    return 0;
}
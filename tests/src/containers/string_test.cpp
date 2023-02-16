#include <ulib/string.h>
#include <ulib/wchar.h>
#include <ulib/u8.h>
#include <ulib/u16.h>
#include <ulib/u32.h>

#include <gtest/gtest.h>

TEST(StringTest, Constructs)
{
    ASSERT_NO_THROW(ulib::String{"hi"});
}

TEST(StringTest, ConstructsWithPresetSize)
{
    ulib::String str(5);
    ASSERT_EQ(str.Size(), 5);
}

TEST(StringTest, ConstructsWithPresetCapacity)
{
    ulib::String str(ulib::args::Capacity{5});
    ASSERT_EQ(str.Capacity(), 5);
}

TEST(StringTest, ConstructsFromStdString)
{
    std::string source = "hello";
    ulib::String str = source;

    ASSERT_EQ(str, ulib::String(source.c_str()));
}

TEST(StringTest, ConstructsFromStringView)
{
    std::string_view source = "hello";
    ulib::String str = source;

    ASSERT_EQ(str, ulib::String(source.data(), source.size()));
}

TEST(StringTest, ConstructsFromUlibStringView)
{
    ulib::StringView source = "hello";
    ulib::String str = source;

    ASSERT_EQ(str, source);
}

TEST(StringTest, ConstructsFromPointerAndSize)
{
    char source[] = "hello world";
    ulib::String str{source, 5};

    ASSERT_EQ(str, "hello");
}

TEST(StringTest, ConstructsFromCharacterRange)
{
    char source[] = "hello world";
    ulib::String str{source, source + 5};

    ASSERT_EQ(str, "hello");
}

TEST(StringTest, EqualityComparison)
{
    ulib::String str = "foo";

    ASSERT_EQ(str, "foo");
    ASSERT_NE(str, "bar");
}

TEST(StringTest, CanAppend)
{
    ulib::String str = "hello";

    str.Append(" world");
    ASSERT_EQ(str, "hello world");
}

TEST(StringTest, CanAssign)
{
    ulib::String str = "hello";
    str = "goodbye";

    ASSERT_EQ(str, "goodbye");
}

TEST(StringTest, ConvertsToStringView)
{
    ulib::String str = "hello";
    std::string_view to = str;

    ASSERT_EQ(str, ulib::String(to.data(), to.size()));
}

TEST(StringTest, Pushing)
{
    ulib::String str = "hello ";
    str.PushBack('w');
    str.PushBack('o');
    str.PushBack('r');
    str.push_back('l');
    str.push_back('d');

    ASSERT_EQ(str, "hello world");
}

TEST(StringTest, Iteration)
{
    ulib::string str = "hello";
    ulib::string str2;
    for(auto ch : str)
    {
        str2.PushBack(ch);
    }

    ASSERT_EQ(str, str2);
}

TEST(StringTest, Addition)
{
    ulib::string str = "hello";
    ulib::string str2 = str + " world";

    ASSERT_EQ(str2, "hello world");

    str2 += " world";

    ASSERT_EQ(str2, "hello world world");
}

TEST(StringTest, Copy)
{
    ulib::string str = "hello";
    ulib::string str2 = str;
    ulib::string str3 = str;

    ASSERT_EQ(str2, str);
    ASSERT_EQ(str3, "hello");
}

TEST(StringTest, CopyFromStringView)
{
    ulib::string_view vstr = "hello";

    ulib::string str2 = vstr;
    ASSERT_EQ(str2, vstr);

    str2 = vstr;
    ASSERT_EQ(str2, vstr);
}

TEST(StringTest, AdditionWithStringView)
{
    ulib::string str2;
    ulib::string_view vstr = "hello";

    str2 += vstr;
    
    ASSERT_EQ(str2, vstr);

    str2 = vstr;
    ASSERT_EQ(str2, vstr);
}

TEST(StringTest, StringViewSum)
{
    ulib::string_view vstr = "hello";
    ulib::string_view vstr2 = " world";

    ulib::string str = vstr + vstr2;
    
    ASSERT_EQ(str, "hello world");
}

TEST(StringTest, Assignment)
{
    ulib::string_view vstr = "hello";
    ulib::wstring_view vwstr = L"hello";
    ulib::u8string_view vu8str = u8"hello";
    ulib::u16string_view vu16str = u"hello";
    ulib::u32string_view vu32str = U"hello";
    
    ulib::string str = vstr;
    ulib::wstring wstr = vwstr;
    ulib::u8string u8str = vu8str;
    ulib::u16string u16str = vu16str;
    ulib::u32string u32str = vu32str;

    ASSERT_EQ(str, "hello");
    ASSERT_EQ(wstr, L"hello");
    ASSERT_EQ(u8str, u8"hello");
    ASSERT_EQ(u16str, u"hello");
    ASSERT_EQ(u32str, U"hello");

    str = vstr;
    wstr = vwstr;
    u8str = vu8str;
    u16str = vu16str;
    u32str = vu32str;

    ASSERT_EQ(str, "hello");
    ASSERT_EQ(wstr, L"hello");
    ASSERT_EQ(u8str, u8"hello");
    ASSERT_EQ(u16str, u"hello");
    ASSERT_EQ(u32str, U"hello");

    vstr = str;
    vwstr = wstr;
    vu8str = u8str;
    vu16str = u16str;
    vu32str = u32str;

    ASSERT_EQ(str, "hello");
    ASSERT_EQ(wstr, L"hello");
    ASSERT_EQ(u8str, u8"hello");
    ASSERT_EQ(u16str, u"hello");
    ASSERT_EQ(u32str, U"hello");

    ulib::string_view vstr1 = str;
    ulib::wstring_view vwstr1 = wstr;
    ulib::u8string_view vu8str1 = u8str;
    ulib::u16string_view vu16str1 = u16str;
    ulib::u32string_view vu32str1 = u32str;

    ASSERT_EQ(vstr1, "hello");
    ASSERT_EQ(vwstr1, L"hello");
    ASSERT_EQ(vu8str1, u8"hello");
    ASSERT_EQ(vu16str1, u"hello");
    ASSERT_EQ(vu32str1, U"hello");

    vstr1 = vstr;
    vwstr1 = vwstr;
    vu8str1 = vu8str;
    vu16str1 = vu16str;
    vu32str1 = vu32str;

    ASSERT_EQ(vstr1, "hello");
    ASSERT_EQ(vwstr1, L"hello");
    ASSERT_EQ(vu8str1, u8"hello");
    ASSERT_EQ(vu16str1, u"hello");
    ASSERT_EQ(vu32str1, U"hello");

    ulib::string_view vstr2 = vstr;
    ulib::wstring_view vwstr2 = vwstr;
    ulib::u8string_view vu8str2 = vu8str;
    ulib::u16string_view vu16str2 = vu16str;
    ulib::u32string_view vu32str2 = vu32str;

    ASSERT_EQ(vstr2, "hello");
    ASSERT_EQ(vwstr2, L"hello");
    ASSERT_EQ(vu8str2, u8"hello");
    ASSERT_EQ(vu16str2, u"hello");
    ASSERT_EQ(vu32str2, U"hello");
}

TEST(StringTest, Compare)
{
    ulib::string_view vstr = "hello";
    ulib::wstring_view vwstr = L"hello";
    ulib::u8string_view vu8str = u8"hello";
    ulib::u16string_view vu16str = u"hello";
    ulib::u32string_view vu32str = U"hello";
    
    ulib::string str = vstr;
    ulib::wstring wstr = vwstr;
    ulib::u8string u8str = vu8str;
    ulib::u16string u16str = vu16str;
    ulib::u32string u32str = vu32str;

    ASSERT_EQ(str == "hello", true);
    ASSERT_EQ(wstr == L"hello", true);
    ASSERT_EQ(u8str == u8"hello", true);
    ASSERT_EQ(u16str == u"hello", true);
    ASSERT_EQ(u32str == U"hello", true);

    ASSERT_EQ(vstr == "hello", true);
    ASSERT_EQ(vwstr == L"hello", true);
    ASSERT_EQ(vu8str == u8"hello", true);
    ASSERT_EQ(vu16str == u"hello", true);
    ASSERT_EQ(vu32str == U"hello", true);

    ASSERT_EQ(str == vstr, true);
    ASSERT_EQ(wstr == vwstr, true);
    ASSERT_EQ(u8str == vu8str, true);
    ASSERT_EQ(u16str == vu16str, true);
    ASSERT_EQ(u32str == vu32str, true);

    ASSERT_EQ(vstr == str, true);
    ASSERT_EQ(vwstr == wstr, true);
    ASSERT_EQ(vu8str == u8str, true);
    ASSERT_EQ(vu16str == u16str, true);
    ASSERT_EQ(vu32str == u32str, true);

    ASSERT_EQ(vstr == vstr, true);
    ASSERT_EQ(vwstr == vwstr, true);
    ASSERT_EQ(vu8str == vu8str, true);
    ASSERT_EQ(vu16str == vu16str, true);
    ASSERT_EQ(vu32str == vu32str, true);

    ASSERT_EQ(str == str, true);
    ASSERT_EQ(wstr == wstr, true);
    ASSERT_EQ(u8str == u8str, true);
    ASSERT_EQ(u16str == u16str, true);
    ASSERT_EQ(u32str == u32str, true);
}
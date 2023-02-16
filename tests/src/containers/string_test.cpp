#include <ulib/string.h>
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
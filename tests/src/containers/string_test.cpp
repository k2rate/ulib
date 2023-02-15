#define ULIB_USE_STD_STRING_VIEW

#include <ulib/containers/string.h>
#include <ulib/containers/encodedstring.h>

#include <ulib/encodings/multibyte/string.h>
#include <ulib/encodings/multibyte/stringview.h>

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

    ASSERT_EQ(str, source);
}

TEST(StringTest, ConstructsFromStringView)
{
    std::string_view source = "hello";
    ulib::String str = source;

    ASSERT_EQ(str, source);
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

    ASSERT_EQ(str, to);
}

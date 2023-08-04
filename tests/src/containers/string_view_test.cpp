#include <gtest/gtest.h>

#include <ulib/string.h>

TEST(StringViewTest, Empty)
{
    ulib::string_view view;
    ASSERT_TRUE(view.empty());

    view = "hello";
    ASSERT_FALSE(view.empty());
}

TEST(StringViewTest, Size)
{
    ulib::string_view view;
    ASSERT_EQ(view.size(), 0);

    view = "hello";
    ASSERT_EQ(view.size(), 5);
}

TEST(StringViewTest, Contains)
{
    ulib::string_view view;
    ASSERT_FALSE(view.contains("vi"));

    view = "view";
    ASSERT_TRUE(view.contains("vi"));
    ASSERT_TRUE(view.starts_with("vi"));
    ASSERT_TRUE(!view.ends_with("vi"));
    ASSERT_TRUE(view.ends_with("ew"));
    ASSERT_TRUE(view.contains("ew"));
}

TEST(StringViewTest, Reverse)
{
    ulib::string_view view;

    view = "view";
    char rr[] = "weiv";

    size_t idx = 0;
    for (auto ch : view.reverse())
    {
        ASSERT_EQ(ch, rr[idx]);
        idx++;
    }
}

TEST(StringViewTest, Length)
{
    ulib::u8string_view view = u8"три";
    ASSERT_EQ(view.length(), 3);
    ASSERT_EQ(view.size(), 6);
}

TEST(StringViewTest, RemoveSuffixAndPrefix)
{
    ulib::string_view view = "hello";

    view.remove_suffix(1);
    ASSERT_EQ(view, "hell");

    view.remove_prefix(1);
    ASSERT_EQ(view, "ell");
}

TEST(StringViewTest, ConstructFromSpan)
{
    char buf[] = { 'h', 'e', 'l', 'l', 'o' };
    ulib::span<char> span = buf;
    ulib::string_view view = span;
    
    ASSERT_EQ(view, "hello");
    ASSERT_EQ(view, span);
}
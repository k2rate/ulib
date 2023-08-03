#include <gtest/gtest.h>

#include <ulib/split.h>
#include <ulib/list.h>
#include <ulib/string.h>

TEST(ListTest, FromSplit)
{
    auto spl = ulib::split("hello world", " ");
    ulib::list<ulib::string> list{spl.begin(), spl.end()};

    ASSERT_TRUE(list.size() == 2);
    ASSERT_TRUE(list[0] == "hello");
    ASSERT_TRUE(list[1] == "world");
}
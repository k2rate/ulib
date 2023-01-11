#include <ulib/containers/list.h>
#include <gtest/gtest.h>

TEST(UlibListTest, SupportsAddingValues)
{
    ulib::List<int> list;

    EXPECT_NO_THROW(list.Add(3));
    EXPECT_NO_THROW(list.Add(4));
    EXPECT_NO_THROW(list.Add(5));

    EXPECT_EQ(list.Size(), 3);
}

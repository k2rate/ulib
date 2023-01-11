#include <ulib/containers/list.h>
#include <gtest/gtest.h>

#include <numeric>

TEST(ListTest, SupportsAddingValues)
{
    ulib::List<int> list;

    ASSERT_NO_THROW(list.Add(3));
    ASSERT_NO_THROW(list.Add(4));
    ASSERT_NO_THROW(list.Add(5));

    EXPECT_EQ(list.Size(), 3);
}

TEST(ListTest, Resizes)
{
    ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

    EXPECT_EQ(list.Size(), 10);

    FAIL() << "TODO: ulib::List<T> does not have Resize() yet!";

/*
    list.Resize(5);
    EXPECT_EQ(list.Size(), 5);
    */
}

TEST(ListTest, Clears)
{
    ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

    EXPECT_EQ(list.Size(), 10);

    ASSERT_NO_FATAL_FAILURE(list.Clear());
    EXPECT_EQ(list.Size(), 0);
}

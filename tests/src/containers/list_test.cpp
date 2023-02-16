#include <ulib/containers/list.h>
#include <numeric>
#include <gtest/gtest.h>


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

    list.Resize(5);
    EXPECT_EQ(list.Size(), 5);  
}

TEST(ListTest, Clears)
{
    ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

    EXPECT_EQ(list.Size(), 10);

    ASSERT_NO_FATAL_FAILURE(list.Clear());
    EXPECT_EQ(list.Size(), 0);
}

TEST(ListTest, NullSizeInitialization)
{
	ulib::List<char> list;
	EXPECT_EQ(list.Size(), 0);
}

TEST(ListTest, FastEraseTest)
{
	ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

	list.FastErase(7);

	EXPECT_EQ(list.Size(), 9);
	EXPECT_EQ(list[7], 9);
}

TEST(ListTest, FastEraseFull)
{
	ulib::List<int> list(10);
	size_t prevSize = list.Size();

    while (list.Size())
	{
		list.FastErase(list.begin());
		EXPECT_NE(prevSize, list.Size());
		prevSize = list.Size();
	}
}
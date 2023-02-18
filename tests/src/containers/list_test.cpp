#include <gtest/gtest.h>
#include <numeric>
#include <ulib/containers/list.h>

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
    ASSERT_EQ(list.Capacity() >= list.Size(), true);
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

TEST(ListTest, FastErase)
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

TEST(ListTest, EraseFull)
{
    ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

    size_t prevSize = list.Size();
    while (list.Size())
    {
        list.Erase(0);
        EXPECT_NE(prevSize, list.Size());
        prevSize = list.Size();
    }
}

TEST(ListTest, SequenceAfterErase)
{
    ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

    ASSERT_EQ(list[4], 4);
    list.Erase(4);

    ASSERT_EQ(list[4], 5);
    ASSERT_EQ(list.Size(), 9);
    ASSERT_EQ(list.Capacity() >= list.Size(), true);
}

TEST(ListTest, Enumerating)
{
    ulib::List<int> list(10);
    std::iota(list.begin(), list.end(), 0);

    ASSERT_EQ(list.Size(), 10);
    ASSERT_EQ(list.Capacity() >= list.Size(), true);

    size_t counter = 0;
    for (auto obj : list)
    {
        ASSERT_EQ(counter, obj);
        counter++;
    }

    ASSERT_EQ(counter, 10);
}

TEST(ListTest, Pushing)
{
    ulib::List<int> list;
    ASSERT_EQ(list.Size(), 0);

    for (size_t i = 0; i != 600; i++)
    {
        int value = rand();
        list.PushBack(value);

        ASSERT_EQ(list.Size() <= list.Capacity(), true);
        ASSERT_EQ(list.Size(), i + 1);
        ASSERT_EQ(list.Back(), value);
    }

    ASSERT_EQ(list.Size(), 600);

    for (size_t i = 600; i != 1200; i++)
    {
        int value = rand();
        list.push_back(value);

        ASSERT_EQ(list.size() <= list.capacity(), true);
        ASSERT_EQ(list.size(), i + 1);
        ASSERT_EQ(list.back(), value);
    }

    ASSERT_EQ(list.size(), 1200);
}

TEST(ListTest, Pop)
{
    ulib::List<int> list;
    ASSERT_EQ(list.Size(), 0);

    for (size_t i = 0; i != 600; i++)
    {
        int value = rand();
        list.PushBack(value);

        ASSERT_EQ(list.Size() <= list.Capacity(), true);
        ASSERT_EQ(list.Size(), i + 1);
        ASSERT_EQ(list.Back(), value);
    }

    ASSERT_EQ(list.Size(), 600);

    size_t prevSize = list.Size();
    while (list.Size())
    {
        list.Pop();
        ASSERT_EQ(list.Size(), prevSize - 1);
        prevSize = list.Size();
    }

    ASSERT_EQ(list.Size(), 0);
    ASSERT_EQ(list.Begin(), list.End());
}

TEST(ListTest, Indexing)
{
    ulib::List<int> list(600);
    std::iota(list.begin(), list.end(), 0);

    ASSERT_EQ(list.At(8), 8);
    ASSERT_EQ(list.At(17), 17);
    ASSERT_EQ(list.At(200), 200);

    ASSERT_EQ(list.at(8), 8);
    ASSERT_EQ(list.at(17), 17);
    ASSERT_EQ(list.at(200), 200);

    ASSERT_EQ(list[8], 8);
    ASSERT_EQ(list[17], 17);
    ASSERT_EQ(list[200], 200);

    for (size_t i = 0; i != 600; i++)
    {
        ASSERT_EQ(list.At(i), i);
        ASSERT_EQ(list.at(i), i);
        ASSERT_EQ(list[i], i);
    }

    for (size_t i = 0; i != 600; i++)
    {
        size_t rr = rand() % 600;

        ASSERT_EQ(list.At(rr), rr);
        ASSERT_EQ(list.At(rr), rr);
        ASSERT_EQ(list.At(rr), rr);
    }
}

TEST(ListTest, Moving)
{
    ulib::List<int> list(600);
    std::iota(list.begin(), list.end(), 0);

    auto list2 = std::move(list);
    ASSERT_EQ(list2.Size(), 600);
    ASSERT_EQ(list.Data(), nullptr);

    for (size_t i = 0; i != 600; i++)
    {
        ASSERT_EQ(list2.At(i), i);
        ASSERT_EQ(list2.at(i), i);
        ASSERT_EQ(list2[i], i);
    }
}

TEST(ListTest, SelfContains)
{
    using InList = ulib::List<int>;

    ulib::List<InList> list;
    for (size_t i = 0; i != 600; i++)
    {
        list.Add(InList{0, 1, 2, 3, 4, 5});
    }

    ASSERT_EQ(list.Size(), 600);

    for (size_t i = 0; i != 600; i++)
    {
        ASSERT_EQ(list[i].Size(), 6);
        ASSERT_EQ(list[i].Capacity() >= 6, true);

        size_t index = 0;
        for(auto it = list[i].begin(); it != list[i].end(); it++)
        {
            ASSERT_EQ(*it, index);
            index++;
        }
    }

    ASSERT_EQ(list.Size(), 600);
}
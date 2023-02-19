#include <stdexcept>
#include <ulib/string.h>
#include <ulib/u16.h>
#include <ulib/u32.h>
#include <ulib/u8.h>
#include <ulib/wchar.h>
#include <ulib/split.h>
#include <ulib/list.h>

#include <gtest/gtest.h>

TEST(FeaturesTest, Split)
{
    ulib::u8string str(u8"full plak");

    ulib::List<ulib::u8string> words;
    for (auto word : ulib::split(str, u8" "))
        words.Add(word);
    
    ASSERT_EQ(words.Size(), 2);
    ASSERT_EQ(words[0], u8"full");
    ASSERT_EQ(words[1], u8"plak");
}

TEST(FeaturesTest, SplitWithFirstSeparator)
{
    ulib::u8string str(u8" full plak");

    ulib::List<ulib::u8string> words;
    for (auto word : ulib::split(str, u8" "))
        words.Add(word);
    
    ASSERT_EQ(words.Size(), 2);
    ASSERT_EQ(words[0], u8"full");
    ASSERT_EQ(words[1], u8"plak");
}

TEST(FeaturesTest, SplitWithEndSeparator)
{
    ulib::u8string str(u8"full plak ");

    ulib::List<ulib::u8string> words;
    for (auto word : ulib::split(str, u8" "))
        words.Add(word);
    
    ASSERT_EQ(words.Size(), 2);
    ASSERT_EQ(words[0], u8"full");
    ASSERT_EQ(words[1], u8"plak");
}

TEST(FeaturesTest, SplitWithBeginEndSeparators)
{
    ulib::u8string str(u8" full plak ");

    ulib::List<ulib::u8string> words;
    for (auto word : ulib::split(str, u8" "))
        words.Add(word);
    
    ASSERT_EQ(words.Size(), 2);
    ASSERT_EQ(words[0], u8"full");
    ASSERT_EQ(words[1], u8"plak");
}

TEST(FeaturesTest, SplitWithMultipleSeparators)
{
    ulib::u8string str(u8"      full        plak      ");

    ulib::List<ulib::u8string> words;
    for (auto word : ulib::split(str, u8" "))
        words.Add(word);
    
    ASSERT_EQ(words.Size(), 2);
    ASSERT_EQ(words[0], u8"full");
    ASSERT_EQ(words[1], u8"plak");
}

TEST(FeaturesTest, SplitWithLongSeparator)
{
    ulib::u8string str(u8"123full123plak123");

    ulib::List<ulib::u8string> words;
    for (auto word : ulib::split(str, u8"123"))
        words.Add(word);
    
    ASSERT_EQ(words.Size(), 2);
    ASSERT_EQ(words[0], u8"full");
    ASSERT_EQ(words[1], u8"plak");
}

TEST(FeaturesTest, SplitByIndex)
{
    ulib::u8string str(u8"123full123plak123");

    auto split = ulib::split(str, u8"123");
    ASSERT_EQ(split[0], u8"full");
    ASSERT_EQ(split[1], u8"plak");

    auto split1 = ulib::split(str, u8"full");
    ASSERT_EQ(split1[0], u8"123");
    ASSERT_EQ(split1[1], u8"123plak123");

    auto split2 = ulib::split(str, u8"plak");
    ASSERT_EQ(split2[0], u8"123full123");
    ASSERT_EQ(split2[1], u8"123");

    ASSERT_THROW(split[2], std::out_of_range);
    ASSERT_THROW(split1[2], std::out_of_range);
    ASSERT_THROW(split2[2], std::out_of_range);
}
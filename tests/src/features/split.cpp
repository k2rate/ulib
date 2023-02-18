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
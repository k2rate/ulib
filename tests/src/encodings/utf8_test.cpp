#include <ulib/encodings/utf8/encoding.h>
#include <ulib/encodings/utf16/encoding.h>
#include <ulib/encodings/utf32/encoding.h>

#include <ulib/encodings/converter.h>

#include <gtest/gtest.h>

TEST(Utf8Test, ToUtf16)
{
    constexpr ulib::u8char kUtf8Str[] = u8"Hello world! 😎";
    constexpr ulib::u16char kUtf16Str[] = u"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf8, ulib::Utf16>(kUtf8Str);    
    ASSERT_EQ(converted, kUtf16Str);
}

TEST(Utf8Test, ToUtf32)
{
    constexpr ulib::u8char kUtf8Str[] = u8"Hello world! 😎";
    constexpr ulib::u32char kUtf32Str[] = U"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf8, ulib::Utf32>(kUtf8Str);    
    ASSERT_EQ(converted, kUtf32Str);
}

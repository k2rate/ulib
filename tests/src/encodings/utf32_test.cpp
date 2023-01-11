#include <ulib/encodings/utf8/encoding.h>
#include <ulib/encodings/utf16/encoding.h>
#include <ulib/encodings/utf32/encoding.h>

#include <ulib/encodings/converter.h>

#include <gtest/gtest.h>

TEST(Utf32Test, ToUtf8)
{
    constexpr ulib::u32char kUtf32Str[] = U"Hello world! 😎";
    constexpr ulib::u8char kUtf8Str[] = u8"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf32, ulib::Utf8>(kUtf32Str);    
    ASSERT_EQ(converted, kUtf8Str);
}

TEST(Utf32Test, ToUtf16)
{
    constexpr ulib::u32char kUtf32Str[] = U"Hello world! 😎";
    constexpr ulib::u16char kUtf16Str[] = u"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf32, ulib::Utf16>(kUtf32Str);    
    ASSERT_EQ(converted, kUtf16Str);
}

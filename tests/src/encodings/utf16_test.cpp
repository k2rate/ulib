#include <ulib/encodings/utf8/encoding.h>
#include <ulib/encodings/utf16/encoding.h>
#include <ulib/encodings/utf32/encoding.h>

#include <ulib/encodings/converter.h>

#include <ulib/containers/string_ostream.h>

#include <gtest/gtest.h>

TEST(Utf16Test, ToUtf8)
{
    constexpr ulib::u16char kUtf16Str[] = u"Hello world! 😎";
    constexpr ulib::u8char kUtf8Str[] = u8"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf16, ulib::Utf8>(kUtf16Str);    
    ASSERT_EQ(converted, kUtf8Str);
}

TEST(Utf16Test, ToUtf32)
{
    constexpr ulib::u16char kUtf16Str[] = u"Hello world! 😎";
    constexpr ulib::u32char kUtf32Str[] = U"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf16, ulib::Utf32>(kUtf16Str);    
    ASSERT_EQ(converted, kUtf32Str);
}

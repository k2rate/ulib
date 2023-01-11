#include <ulib/encodings/utf8/encoding.h>
#include <ulib/encodings/utf16/encoding.h>

#include <ulib/encodings/converter.h>

#include <gtest/gtest.h>

TEST(Utf8Test, ConvertToUtf16)
{
    constexpr ulib::u8char kUtf8Str[] = u8"Hello world! 😎";
    constexpr ulib::u16char kUtf16Str[] = u"Hello world! 😎";

    const auto converted = ulib::Convert<ulib::Utf8, ulib::Utf16>(kUtf8Str);    
    FAIL() << "TODO: Equality comparisons are not available in strings yet!";
    // ASSERT_EQ(converted, kUtf16Str);
}

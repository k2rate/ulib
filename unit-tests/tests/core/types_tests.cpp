#include <ulib/types.h>
#include <gtest/gtest.h>

TEST(UlibCoreTypesTest, IntegerSizesAreCorrect)
{
    ASSERT_GE(sizeof(ushort), sizeof(uchar));
    ASSERT_GE(sizeof(uint), sizeof(ushort));
    ASSERT_GE(sizeof(ulong), sizeof(uint));
    ASSERT_GE(sizeof(ullong), sizeof(ulong));
}

TEST(UlibCoreTypesTest, NativeIntegerIsEqualToIntPtr)
{
    ASSERT_GE(sizeof(nuint), sizeof(std::intptr_t));
}

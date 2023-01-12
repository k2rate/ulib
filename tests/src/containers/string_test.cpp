#include <ulib/containers/string.h>
#include <gtest/gtest.h>

TEST(StringTest, Constructs)
{
    ASSERT_NO_THROW(ulib::String{"hi"});
}

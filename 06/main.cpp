#include "format.hpp"
#include "gtest/gtest.h"

TEST(Lab6, correct)
{
	ASSERT_EQ( format("{1}+{1} = {0}", 2, "one"), "one+one = 2" );
}

TEST(Lab6, nested_brace)
{
	ASSERT_THROW(format("{{1}+{1} = {0}", 2, "one"), std::invalid_argument );
}

TEST(Lab6, limit_break)
{
	ASSERT_THROW(format("{1}+{1} = {0}", 2), std::invalid_argument );
}

TEST(Lab6, empty_braces)
{
	ASSERT_THROW(format("{1}+{1} = {}", 2, "one"), std::invalid_argument );
}

TEST(Lab6, missed_opening_brace)
{
	ASSERT_THROW(format("{1}+{1} = 0}", 2, "one"), std::invalid_argument );
}

TEST(Lab6, missed_closing_brace)
{
	ASSERT_THROW(format("{1}+{1} = {0", 2, "one"), std::invalid_argument );
}

TEST(Lab6, incorrect_symbol)
{
	ASSERT_THROW(format("{1}+{1} = {o}", 2, "one"), std::invalid_argument );
}

TEST(Lab6, arg_not_used)
{
	ASSERT_THROW(format("{0}*{0} = {0}", 2, "one"), std::invalid_argument );
}

TEST(Lab6, empty_func)
{
	ASSERT_THROW(format(), std::invalid_argument );
}

TEST(Lab6, hello_world)
{
	ASSERT_EQ( format("Hello world"), "Hello world" );
}

TEST(Lab6, empty_str)
{
	ASSERT_EQ( format(""), "" );
}

int main(int argc, char **argv)
{
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}


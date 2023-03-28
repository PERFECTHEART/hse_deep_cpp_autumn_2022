#include "format.hpp"
#include "gtest/gtest.h"

TEST(Lab6, correct) {
	ASSERT_EQ(format("{1}+{1} = {0}", 2, "one"), "one+one = 2");
}

TEST(Lab6, nested_brace) {
	EXPECT_THROW({
		try {
			format("{{1}+{1} = {0}");
		} catch (const MyException & e) {
			EXPECT_STREQ("Недопустимый вложенный символ: {.", e.what());
			throw;
		}
	}, MyException);
}

TEST(Lab6, limit_break) {
	EXPECT_THROW({
		try {
			format("{1}+{1} = {0}", 2);
		} catch (const MyException & e) {
			EXPECT_STREQ("Номер запрошенного аргумента превышает число аргументов в вызове.", e.what());
			throw;
		}
	}, MyException);
}

#ifdef NO_EMPTY_BRACES
TEST(Lab6, empty_braces) {
	EXPECT_THROW({
		try {
			format("{1}+{1} = {}", 2, "one");
		} catch (const MyException & e) {
			EXPECT_STREQ("Нет значения внутри {}.", e.what());
			throw;
		}
	}, MyException);

}
#endif

TEST(Lab6, missed_opening_brace) {
	EXPECT_THROW({
		try {
			format("{1}+{1} = 0}", 2, "one");
		} catch (const MyException & e) {
			EXPECT_STREQ("Символу } не сопутствует символ {.", e.what());
			throw;
		}
	}, MyException);
}

TEST(Lab6, missed_closing_brace) {
	EXPECT_THROW({
		try {
			format("{1}+{1} = {0", 2, "one");
		} catch (const MyException & e) {
			EXPECT_STREQ("Нет } после {.", e.what());
			throw;
		}
	}, MyException);
}

TEST(Lab6, incorrect_symbol) {
	EXPECT_THROW({
		try {
			format("{1}+{1} = {o}", 2, "one");
		} catch (const MyException & e) {
			EXPECT_STREQ("Некорректный символ o внутри {}.", e.what());
			throw;
		}
	}, MyException);
}

TEST(Lab6, arg_not_used) {
	EXPECT_THROW({
		try {
			format("{0}*{0} = {0}", 2, "one");
		} catch (const MyException & e) {
			EXPECT_STREQ("Аргумент 1 не использован.", e.what());
			throw;
		}
	}, MyException);
}

TEST(Lab6, empty_func) {
	EXPECT_THROW({
		try {
			format();
		} catch (const MyException & e) {
			EXPECT_STREQ("Вызвана функция без аргументов.", e.what());
			throw;
		}
	}, MyException);
}

TEST(Lab6, hello_world) {
	ASSERT_EQ(format("Hello world"), "Hello world");
}

TEST(Lab6, empty_str) {
	ASSERT_EQ(format(""), "");
}

TEST(Lab6, blanc_str) {
	ASSERT_EQ(format(" "), " ");
}

int main(int argc, char ** argv) {
	::testing::InitGoogleTest( & argc, argv);
	return RUN_ALL_TESTS();
}
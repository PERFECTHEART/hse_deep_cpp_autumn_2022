#include <cassert>
#include <iostream>
#include "serializer.hpp"
#include <string>
#include <gtest/gtest.h>

TEST(Lab5, ok1)
{
	Data y { 0, false, 0 };
	Data u { 111, true, 222 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
#ifdef DEBUG0
	std::cout << u.a << " " << (u.b ? "true" : "false") << " " << u.c << std::endl;
#endif
	err = deserializer.load(u);
#ifdef DEBUG0
	std::cout << "y.a EQ u.a" << std::endl;
	std::cout << "y.b EQ u.b" << std::endl;
	std::cout << "y.c EQ u.c" << std::endl;
	std::cout << u.a << " " << (u.b ? "true" : "false") << " " << u.c << std::endl << std::endl;
#endif
	ASSERT_EQ(y.a, u.a);
	ASSERT_EQ(y.b, u.b);
	ASSERT_EQ(y.c, u.c);
	ASSERT_EQ(err, Error::NoError);
}

TEST(Lab5, ok2)
{
	Data x { 10, true, 20 };
	Data y { 0, false, 0 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(x);
	ASSERT_EQ(err, Error::NoError);
#ifdef DEBUG0
	std::cout << "x      " << x.a << " " << (x.b ? "true" : "false") << " " << x.c << std::endl;
	std::cout << "before " << y.a << " " << (y.b ? "true" : "false") << " " << y.c << std::endl;
	std::cout << "after  " << y.a << " " << (y.b ? "true" : "false") << " " << y.c << std::endl << std::endl;
#endif
	err = deserializer.load(y);
	ASSERT_EQ(x.a, y.a);
	ASSERT_EQ(x.b, y.b);
	ASSERT_EQ(x.c, y.c);
	ASSERT_EQ(err, Error::NoError);
}

TEST(Lab5, not_ok)
{
	Data x { 10, true, 20 };
	Data3i z { 100, 200, 300 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(x);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(z);
#ifdef DEBUG0
	std::cout << "Corrupted archive" << std::endl;
#endif
	ASSERT_EQ(x.a, z.a);
	ASSERT_NE(x.b, z.b);
	ASSERT_NE(x.c, z.c);
	ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(Lab5, corrupt_test)
{
	Data1b x { true };
	Data1i z { 2 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(x);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(z);
#ifdef DEBUG0
	std::cout << "Corrupted archive" << std::endl;
#endif
	ASSERT_NE(x.a, z.a);
	ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(Lab5, corrupt_test_string)
{	
	Data1str x { "teststring" };
	Data3i z { 0, 1, 3 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);	
	
	err = serializer.save(x);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(z);
#ifdef DEBUG0
	std::cout << "Corrupted archive" << std::endl;
#endif
	ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(Lab5, single_int)
{
	Data1i y { 0 };
	Data1i u { 5 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);
	
	ASSERT_EQ(y.a, u.a);
	ASSERT_EQ(err, Error::NoError);
}

TEST(Lab5, single_int1)
{
	Data1i y { 1 };
	Data1i u { 5 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);
	
	ASSERT_EQ(y.a, u.a);
	ASSERT_EQ(err, Error::NoError);
}

TEST(Lab5, single_int_largest)
{
	Data1i y { (uint64_t) -1 };
	Data1i u { 5 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);
	
	ASSERT_EQ(y.a, u.a);
	ASSERT_EQ(err, Error::NoError);
#ifdef DEBUG0
	std::cout << y.a << std::endl;
#endif
}

TEST(Lab5, single_bool)
{
	Data1b y { false };
	Data1b u { true };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);

	ASSERT_EQ(y.a, u.a);
	ASSERT_EQ(err, Error::NoError);
}

TEST(Lab5, 1_to_true)
{
	Data1b y { 1 };
	Data1b u { false };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);
	ASSERT_EQ(y.a, u.a);
	ASSERT_NE(err, Error::CorruptedArchive);
}

TEST(Lab5, 0_to_false)
{
	Data1b y { 0 };
	Data1b u { true };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);
	ASSERT_EQ(y.a, u.a);
	ASSERT_NE(err, Error::CorruptedArchive);
}

// 18446744073709551615 max uint64_t
TEST(Lab5, str_to_int)
{
	Data1str y { "18446744073709551616" };
	Data1i u { 5 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(err, Error::NoError);
	err = deserializer.load(u);
	
	ASSERT_EQ(18446744073709551615u, u.a);
	ASSERT_EQ(err, Error::InvalidNumericValue);
#ifdef DEBUG0
	std::cout << y.a << std::endl;
#endif
}

TEST(Lab5, stream_3_data)
{
	Data y { 123, false, 456 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(stream.str(), "123 false 456");
	ASSERT_EQ(err, Error::NoError);
}

TEST(Lab5, stream_str_data)
{
	Data1str y { " " };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);

	err = serializer.save(y);
	ASSERT_EQ(stream.str(), " ");
	ASSERT_EQ(err, Error::NoError);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

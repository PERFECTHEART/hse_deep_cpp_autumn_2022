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

	serializer.save(y);
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
	ASSERT_EQ( y.a, u.a );
	ASSERT_EQ( y.b, u.b );
	ASSERT_EQ( y.c, u.c );
	ASSERT_EQ( err, Error::NoError );
}

TEST(Lab5, ok2)
{
	Data x { 10, true, 20 };
	Data y { 0, false, 0 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	serializer.save(x);
#ifdef DEBUG0
	std::cout << "x      " << x.a << " " << (x.b ? "true" : "false") << " " << x.c << std::endl;
	std::cout << "before " << y.a << " " << (y.b ? "true" : "false") << " " << y.c << std::endl;
	std::cout << "after  " << y.a << " " << (y.b ? "true" : "false") << " " << y.c << std::endl << std::endl;
#endif
	err = deserializer.load(y);
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

	serializer.save(z);
	err = deserializer.load(x);
#ifdef DEBUG0
	std::cout << "Corrupted archive" << std::endl;
#endif
	ASSERT_EQ(err, Error::CorruptedArchive);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#include "biginteger.hpp"
#include <stdlib.h>
#include <gtest/gtest.h>

TEST(Lab4, lte)
{
	BigInt x("-500000000000000005");
	BigInt y("20000000000000002");
	std::cout << "x LT y" << std::endl;
	ASSERT_LT( x, y );
	std::cout << "x LE y" << std::endl;
	ASSERT_LE( x, y );
}

TEST(Lab4, eq)
{
	BigInt x("-500000000000000005");
	BigInt y("-500000000000000005");
	std::cout << "x EQ y" << std::endl;
	ASSERT_EQ( x, y );
}

TEST(Lab4, ne)
{
	BigInt x("-500000000000000005");
	BigInt y("11111111");
	std::cout << "x NE y" << std::endl;
	ASSERT_NE( x, y );
}

TEST(Lab4, selfle)
{
	BigInt x("-500000000000000005");
	std::cout << "x EQ x" << std::endl;
	ASSERT_LE( x, x );
}

TEST(Lab4, gte)
{
	BigInt x("-500000000000000005");
	BigInt y("20000000000000002");
	std::cout << "y GT x" << std::endl;
	ASSERT_GT( y, x );
	std::cout << "y GE x" << std::endl;
	ASSERT_GE( y, x );
}

TEST(Lab4, sub)
{
	BigInt x("-500000000000000005");
	std::cout << "-" << std::endl;
	ASSERT_EQ( x - x, 0 );
}

TEST(Lab4, sum)
{
	std::cout << "+" << std::endl;
	ASSERT_EQ( BigInt("100") + BigInt(200), BigInt("300") );
}

TEST(Lab4, mult)
{
	std::cout << "*" << std::endl;
	ASSERT_EQ( BigInt("5000000000") * BigInt(3), BigInt("15000000000") );
}

TEST(Lab4, uno)
{
	std::cout << "-(x) EQ -x" << std::endl;
	ASSERT_EQ( -BigInt(100), BigInt(-100) );
}

TEST(Lab4, all)
{
	BigInt x("-500000000000000005");
	BigInt y("20000000000000002");

	BigInt tmp;
	std::cout << "x: " << x << std::endl;

	std::cout << "y: " << y << std::endl;
	std::cout << "x + y: " <<  x + y << std::endl;
	std::cout << "x - y: " << x - y << std::endl;
	std::cout << "x * y: " <<  x * y << std::endl;

	std::cout << x << " + -100: " << x + (-100) << std::endl;
	std::cout << x << " - -100: " << x - (-100) << std::endl;
	std::cout << x << " * -100: " << x * (-100) << std::endl;

	BigInt z("-000");
	std::cout << "z = -000: " << z << std::endl;

	BigInt x1 = x + y;
	std::cout << "x1 = x + y: " << x1 << std::endl;

	y = -y;
	std::cout << "y = -y: " << y << std::endl;

	x1 = -x1;
	std::cout << "x1 = -x1: " << x1 << std::endl;
	BigInt x2 = std::move(x1);
	std::cout << "x2: " << x2 << std::endl;
	x2 = std::move(x2);
	
	z = -z;
	std::cout << "z = -z: " << z << std::endl;

}
int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

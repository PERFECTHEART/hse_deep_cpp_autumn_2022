#include "biginteger.hpp"
#include <stdlib.h>

int main()
{
	BigInt x("-500000000000000005");
	std::cout << "x: " << x << std::endl;

	BigInt y("20000000000000002");
	std::cout << "y: " << y << std::endl;
	std::cout << "x + y: " << x + y << std::endl;
	std::cout << "x - y: " << x - y << std::endl;
	std::cout << "x * y: " << x * y << std::endl;

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
	
	z = -z;
	std::cout << "z = -z: "  << z << std::endl;

	assert( x < y );
	assert( x <= y );
	assert( !(x == y) );
	assert( !(x > y) );
	assert( !(x >= y) );
	assert( x == x );
	assert( x != y );

	assert( !(y < x) );
	assert( !(y <= x) );
	assert( !(y == x) );
	assert( y > x );
	assert( y >= x );
	assert( !(y == x) );
	assert( y != x);

	exit(0);
}
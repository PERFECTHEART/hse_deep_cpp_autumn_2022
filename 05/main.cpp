#include <gtest/gtest.h>
#include <iostream>
#include "serializer.cpp"
#include <string>

int main(int argc, char** argv)
{
	Data x { 10, true, 20 };
	Data y { 0, false, 0 };
	Data u { 111, true, 222 };
	Data3i z { 100, 200, 300 };
	Error err;
	std::stringstream stream;
	Serializer serializer(stream);
	Deserializer deserializer(stream);

	serializer.save(y);
std::cout << u.a << " " << (u.b ? "true" : "false") << " " << u.c << std::endl;
	err = deserializer.load(u);
std::cout << u.a << " " << (u.b ? "true" : "false") << " " << u.c << std::endl;
std::cout << std::endl;
	assert( err == Error::NoError);
	assert( y.a == u.a && y.b == u.b && y.c == u.c );

	serializer.save(x);
std::cout << y.a << " " << (y.b ? "true" : "false") << " " << y.c << std::endl;
	err = deserializer.load(y);
std::cout << y.a << " " << (y.b ? "true" : "false") << " " << y.c << std::endl;
std::cout << std::endl;
	assert(err == Error::CorruptedArchive);

	serializer.save(z);
	err = deserializer.load(x);
	assert(err == Error::CorruptedArchive);

	exit(0);
}
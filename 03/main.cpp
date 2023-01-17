#include "matrix.hpp"
#include <random>
#include <ctime>
#include "gtest/gtest.h"

TEST(Lab3, getparams)
{
	Matrix  m(20, 10);

	std::cout << "Get matrix dimensions." << std::endl;
	ASSERT_EQ(m.getRows(), (size_t) 20);
	ASSERT_EQ(m.getColumns(), (size_t) 10);
}

TEST(Lab3, getvalue)
{
	Matrix  m(20, 10);

	std::cout << "Check assigned value." << std::endl;
	ASSERT_NE(m[1][2],50);
	m[1][2] = 50;
	ASSERT_EQ(m[1][2],50);
}

TEST(Lab3, multiply)
{
	Matrix  m(20, 10);
	
	std::cout << "Check multiplied value." << std::endl;
	m[1][2] = 50;
	ASSERT_NE(m[1][2], 50*3);
	m *= 3;
	ASSERT_EQ(m[1][2], 50*3);
}

TEST(Lab3, compare)
{
	Matrix  m(5, 3);
	Matrix m2(5, 3);
	size_t r, c;

	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = rand() % 100;
	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m2[i][j] = rand() % 100;

	std::cout << "m + m2:" << std::endl << m + m2 << std::endl;

	srand(time(NULL));
	r = 2 + rand() % ( 6 - 2 + 1 );
	c = 2 + rand() % ( 10 - 2 + 1 );
	Matrix m1( r, c );
	std::cout << "Check (m != m1)" << std::endl;
	ASSERT_NE(m, m1);
	std::cout << "Check (m == m)" << std::endl;
	ASSERT_EQ(m, m);
}

TEST(Lab3, bound)
{
	Matrix  m(5, 3);

	std::cout << "m[6][4] = 5;" << std::endl;
	ASSERT_THROW(m[6][4] = 5, std::out_of_range);
	std::cout << "m[0][3] = 33;" << std::endl;
	ASSERT_THROW(m[0][3] = 33, std::out_of_range);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

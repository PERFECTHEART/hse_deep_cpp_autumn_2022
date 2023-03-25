#include "matrix.hpp"
#include <random>
#include "gtest/gtest.h"

TEST(Lab3, getparams)
{
	Matrix  m(20, 10);

	ASSERT_EQ(m.getRows(), (size_t) 20);
	ASSERT_EQ(m.getColumns(), (size_t) 10);
}

TEST(Lab3, getvalue)
{
	Matrix  m(20, 10);
	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = i*m.getColumns() + j;
	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			ASSERT_EQ(m[i][j],i*m.getColumns() + j);
}

TEST(Lab3, multiply)
{
	Matrix  m(20, 10);

	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = i*m.getColumns() + j;
	m *= 3;
	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			ASSERT_EQ(m[i][j],(i*m.getColumns() + j)*3);
}

TEST(Lab3, compare)
{
	Matrix  m(5, 3);
	Matrix m2(5, 3);

	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = i*m.getColumns() + j;
	for( size_t i=0; i<m2.getRows(); i++)
		for(size_t j=0; j<m2.getColumns(); j++)
			m2[i][j] = i*m2.getColumns() + j;
	ASSERT_EQ(m, m2);
	m[1][1] = 100;
	ASSERT_NE(m, m2);
	m[1][1] = 4;
	ASSERT_EQ(m, m2);
	Matrix m1(5,4);
	for( size_t i=0; i<m1.getRows(); i++)
		for(size_t j=0; j<m1.getColumns(); j++)
			m1[i][j] = i*m1.getColumns() + j;
	ASSERT_NE(m,m1);
}

TEST(Lab3, sum)
{
	Matrix  m(5, 3);
	Matrix m2(5, 3);

	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = rand() % 100;
	for( size_t i=0; i<m2.getRows(); i++)
		for(size_t j=0; j<m2.getColumns(); j++)
			m2[i][j] = rand() % 100;
	Matrix m3 = m + m2;
	for( size_t i=0; i<m3.getRows(); i++)
		for(size_t j=0; j<m3.getColumns(); j++)
			ASSERT_EQ(m3[i][j],m[i][j]+m2[i][j]);

	Matrix m1(5,4);
	for( size_t i=0; i<m1.getRows(); i++)
		for(size_t j=0; j<m1.getColumns(); j++)
			m1[i][j] = i*m1.getColumns() + j;
	ASSERT_THROW(Matrix tmp = m + m1, std::out_of_range);
}


TEST(Lab3, bound)
{
	Matrix  m(5, 3);

	ASSERT_THROW(m[6][4] = 5, std::out_of_range);
	ASSERT_THROW(m[0][3] = 33, std::out_of_range);
	ASSERT_THROW(m[5][0] = 13, std::out_of_range);
}

TEST(Lab3, output)
{
	Matrix  m(5, 3);

	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = i*m.getColumns() + j;
	std::stringstream ss;
	ss << m;
	ASSERT_EQ(ss.str(), "0\t1\t2\t\n3\t4\t5\t\n6\t7\t8\t\n9\t10\t11\t\n12\t13\t14\t\n");
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include "allocator.hpp"
#include "gtest/gtest.h"

TEST(TestA, CheckZeroMemory)
{
	Allocator *m1;

	m1 = new Allocator();
	m1->makeAllocator(0);
	ASSERT_EQ( m1->getSize(), (size_t) 0 );
	delete m1;
}

TEST(TestA, CheckOffset)
{
	Allocator *m1;
	
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(10);
	ASSERT_EQ( m1->getOffset(), (size_t) 0 );
	s1 = m1->alloc(5);
	s1[0] = ' ';
	ASSERT_NE( m1->getOffset(), (size_t) 0 );
	ASSERT_EQ( m1->getOffset(), (size_t) 5 );
	delete m1;
}

TEST(TestA, CheckOutOfSize)
{
	Allocator *m1;
	
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(10);
	ASSERT_EQ( m1->getOffset(), (size_t) 0 );
	s1 = m1->alloc(20);
	ASSERT_EQ( s1, nullptr );
	delete m1;
}

TEST(TestA, CheckReset)
{
	Allocator *m1;
	
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(100);
	ASSERT_EQ( m1->getOffset(), (size_t) 0 );
	s1 = m1->alloc(20);
	ASSERT_EQ( m1->getSize(), (size_t) 80 );
	s1[0] = ' ';
	m1->reset();
	ASSERT_EQ( m1->getSize(), (size_t) 100 );
	delete m1;
}

TEST(TestA, CheckBorder)
{
	Allocator *m1;
	
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(20);
	s1 = m1->alloc(0);
	ASSERT_EQ( m1->getOffset(), (size_t) 0 );
	s1 = m1->alloc(20);
	s1[19]='$';
	ASSERT_EQ( m1->getOffset(), (size_t) 20 );
	delete m1;
}

TEST(TestA, CheckReallocate)
{
	Allocator *m1;

	m1 = new Allocator();
	char *s1;
	char *s2;
	m1->makeAllocator(30);
	m1->makeAllocator(40);
	s1 = m1->alloc(20);
	ASSERT_TRUE( s1 != nullptr );
	s2 = m1->alloc(20);
	ASSERT_TRUE( s2 != nullptr );
	m1->reset();
	s2 = m1->alloc(20);
	ASSERT_TRUE( s2 != nullptr );
	s1 = m1->alloc(50);
	ASSERT_FALSE( s1 != nullptr );
	delete m1;
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

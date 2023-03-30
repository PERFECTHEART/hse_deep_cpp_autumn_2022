#include "allocator.hpp"
#include "gtest/gtest.h"

TEST(Lab1, CheckZeroMemory)
{
	Allocator *m1;
	m1 = new Allocator();
	m1->makeAllocator(0);
	ASSERT_EQ(m1->getSize(), (size_t) 0);
	delete m1;
}

TEST(Lab1, CheckOffset)
{
	Allocator *m1;
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(10);
	ASSERT_EQ(m1->getOffset(), (size_t) 0);
	s1 = m1->alloc(5);
	s1[0] = ' ';
	ASSERT_NE(m1->getOffset(), (size_t) 0);
	ASSERT_EQ(m1->getOffset(), (size_t) 5);
	delete m1;
}

TEST(Lab1, CheckOutOfSize)
{
	Allocator *m1;
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(10);
	ASSERT_EQ(m1->getOffset(), (size_t) 0);
	s1 = m1->alloc(20);
	ASSERT_EQ(s1, nullptr);
	delete m1;
}

TEST(Lab1, CheckReset)
{
	Allocator *m1;
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(100);
	ASSERT_EQ(m1->getOffset(), (size_t) 0);
	s1 = m1->alloc(20);
	ASSERT_EQ(m1->getSize(), (size_t) 80);
	s1[0] = ' ';
	m1->reset();
	ASSERT_EQ(m1->getSize(), (size_t) 100);
	delete m1;
}

TEST(Lab1, CheckBorder)
{
	Allocator *m1;
	char *s1;
	m1 = new Allocator();
	m1->makeAllocator(20);
	s1 = m1->alloc(0);
	ASSERT_EQ(m1->getOffset(), (size_t) 0);
	s1 = m1->alloc(20);
	s1[19] = '$';
	ASSERT_EQ(m1->getOffset(), (size_t) 20);
	delete m1;
}

TEST(Lab1, CheckReallocate)
{
	Allocator *m1;
	m1 = new Allocator();
	char *s1;
	char *s2;
	m1->makeAllocator(30);
	m1->makeAllocator(40);
	s1 = m1->alloc(20);
	ASSERT_TRUE(s1 != nullptr);
	s2 = m1->alloc(20);
	ASSERT_TRUE(s2 != nullptr);
	m1->reset();
	s2 = m1->alloc(20);
	ASSERT_TRUE(s2 != nullptr);
	s1 = m1->alloc(50);
	ASSERT_FALSE(s1 != nullptr);
	delete m1;
}

TEST(Lab1, Chain1)
{
	Allocator *m1;
	char *s1;
	char *s2;
	m1 = new Allocator();
	m1->makeAllocator(1);
	s1 = m1->alloc(1);
	s2 = m1->alloc(1);
	ASSERT_TRUE(s1 != nullptr);
	ASSERT_TRUE(s2 == nullptr);
	delete m1;
}

TEST(Lab1, Chain50)
{
	Allocator *m1;
	char *s1;
	char *s2;
	char *s3;
	m1 = new Allocator();
	m1->makeAllocator(50);
	s1 = m1->alloc(30);
	s2 = m1->alloc(20);
	s3 = m1->alloc(1);
	ASSERT_TRUE(s1 != nullptr);
	ASSERT_TRUE(s2 != nullptr);
	ASSERT_TRUE(s3 == nullptr);
	m1->reset();
	s3 = m1->alloc(50);
	ASSERT_FALSE(s3 == nullptr);
	delete m1;
}

TEST(Lab1, Pointer)
{
	Allocator *m1;
	char *s1;
	char *s2;
	m1 = new Allocator();
	m1->makeAllocator(25);
	s1 = m1->alloc(12);
	s2 = m1->alloc(13);
	ASSERT_TRUE(s1 != nullptr);
	ASSERT_TRUE(s2 != nullptr);
	ASSERT_EQ(s1-s2, -12);
	delete m1;
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

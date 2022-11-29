#include "allocator.hpp"

#define ASSERT_TRUE( a )	cout <<  __FILE__ ":" << __LINE__ << (a ? " Passed\n" : " Failed\n")
#define ASSERT_FALSE( a )	cout <<  __FILE__ ":" << __LINE__ << (a ? " Failed\n" : " Passed\n")

main()
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
	exit(0);
}

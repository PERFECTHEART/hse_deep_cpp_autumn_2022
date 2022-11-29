#pragma once

#include <stdlib.h>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

class Allocator
{
	char* m_memstart;
	size_t m_offset;
	size_t m_total;
public:
	Allocator();
	char *alloc(size_t size);
	void reset();
	void makeAllocator(size_t maxSize);
	// Это деструктор!
	~Allocator();
};
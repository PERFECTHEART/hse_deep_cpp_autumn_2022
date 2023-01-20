#pragma once

#include <iostream>

class Allocator
{
	char* m_memstart = nullptr;
	size_t m_offset = (size_t) 0;
	size_t m_total = (size_t) 0;
public:
	Allocator();
	char *alloc(size_t size);
	void reset();
	void makeAllocator(size_t maxSize);
	size_t getSize();
	size_t getOffset();
	// Это деструктор!
	~Allocator();
};

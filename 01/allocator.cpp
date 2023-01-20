#include "allocator.hpp"

void Allocator::makeAllocator(size_t maxSize) 
{
	if( m_memstart != nullptr ) {
		delete m_memstart;
		m_memstart = nullptr;
		#ifdef DEBUG
			std::cout << "Memory reallocated" << std::endl;
		#endif
	}
	if ( maxSize > 0 )
		m_memstart = new char[maxSize];
	m_offset = (size_t) 0;
	if( m_memstart == nullptr ) {
		#ifdef DEBUG
			std::cout << "Cannot allocate " << maxSize << " bytes of memory\n";
		#endif
		m_total = (size_t) 0;
	}
	else {
		m_total = maxSize;
	}
	#ifdef DEBUG
		std::cout << "Free memory: " << m_total << std::endl;
	#endif
}

size_t Allocator::getSize()
{
	return m_total - m_offset;
}

size_t Allocator::getOffset()
{
	return m_offset;
}

char *Allocator::alloc(size_t size)
{
	char *ptr = nullptr;

	if( size <= m_total - m_offset ) {
		ptr = m_memstart + m_offset;
		m_offset = m_offset + size;
		#ifdef DEBUG
			std::cout << size << " bytes taken" << std::endl;
		#endif
	}
	else {
		#ifdef DEBUG
			std::cout << size << " bytes allocating... Not enough memory in allocator" << std::endl;
		#endif
	}
	#ifdef DEBUG
		std::cout << "Free memory: " << m_total - m_offset << std::endl;
	#endif
	return ptr;
}

void Allocator::reset() 
{
	m_offset = 0;
	#ifdef DEBUG
		std::cout << "All memory released" << std::endl;
	#endif
}

Allocator::Allocator() {
	#ifdef DEBUG
		std::cout << "Constructor executed" << std::endl;
	#endif
	m_memstart = nullptr;
}

Allocator::~Allocator() {
	if( m_memstart != nullptr ) {
		delete m_memstart;
	}
	#ifdef DEBUG
		std::cout << "Destructor executed" << std::endl << std::endl;
	#endif
}

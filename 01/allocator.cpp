#include "allocator.hpp"

void Allocator::makeAllocator(size_t maxSize)
{
	if( m_memstart != nullptr ) {
		free( m_memstart );
		m_memstart = nullptr;
		#ifdef DEBUG
			cout << "Memory reallocated\n";
		#endif
	}
	m_memstart = (char *) malloc( maxSize );
	m_offset = 0;
	if( m_memstart == nullptr ) {
		#ifdef DEBUG
			cout << "Cannot allocate memory\n";
		#endif
	}
	else {
		m_total = maxSize;
	}
	#ifdef DEBUG
		cout << "Free memory: " << m_total << "\n";
	#endif
}

char *Allocator::alloc(size_t size)
{
	char *ptr = nullptr;
	if( size <= m_total - m_offset ) {
		ptr = m_memstart + m_offset;
		m_offset = m_offset + size;
		#ifdef DEBUG
			cout << size << " bytes taken\n";
		#endif
	}
	else {
		#ifdef DEBUG
			cout << "Not enough memory\n";
		#endif
	}
	#ifdef DEBUG
		cout << "Free memory: " << m_total - m_offset << "\n";
	#endif
	return ptr;
}

void Allocator::reset() 
{
	m_offset = 0;
	#ifdef DEBUG
		cout << "All memory released\n";
	#endif
}

Allocator::Allocator() {
	#ifdef DEBUG
		cout << "Constructor executed\n";
	#endif
	m_memstart = nullptr;
}

Allocator::~Allocator() {
	if( m_memstart != nullptr ) {
		free( m_memstart );
	}
	#ifdef DEBUG
		cout << "Destructor executed\n";
	#endif
}

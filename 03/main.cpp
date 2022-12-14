#include "matrix.hpp"
#include <random>

int main()
{
	Matrix m(5, 3);
	Matrix m2(5,3);

	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m[i][j] = rand() % 100;
	for( size_t i=0; i<m.getRows(); i++)
		for(size_t j=0; j<m.getColumns(); j++)
			m2[i][j] = rand() % 100;

	assert(m.getRows() == 5);
	assert(m.getColumns() == 3);
	m[1][2] = 50;
	double x = m[4][1];
	cout << "double x: " << x << endl;
	cout << "m:" << endl << m << endl;
	cout << "m2:" << endl << m2 << endl;
	cout << "m + m2:" << endl << m + m2 << endl;
	m *= 3;
	cout << "m *= 3:" << endl << m << endl;
	size_t r, c;
	r = 2 + rand() % ( 6 - 2 + 1 );
	c = 2 + rand() % ( 10 - 2 + 1 );
	cout << "r: " << r << " c: " << c << endl;
	Matrix m1( r, c );
	cout << "m1:" << endl << m1 << endl;
	cout << "m == m1: " << boolalpha << (m == m1) << endl;
	cout << "m == m: " << boolalpha << (m == m) << endl;
	cout << "m[6][4] = 5;\n";
	m[6][4] = 5;
	cout << "m[0][3] = 33;\n";
	m[0][3] = 33;
	return 0 ; 
}

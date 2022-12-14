#include "matrix.hpp"

static size_t sd2;

Matrix::Matrix (const size_t &n_rows, const size_t &n_cols)	//constructor of class Matrix
{
	d1 = n_rows;
	d2 = n_cols;
	matrix = new BASE_TYPE *[d1];	
	for (size_t i = 0; i < d1; i++)
		matrix[i] = new BASE_TYPE[d2] ();
}

Matrix::Matrix (const Matrix & rhs) //copy Matrix constructor
{
	d1 = rhs.d1;
	d2 = rhs.d2;
	matrix = new BASE_TYPE *[rhs.d1];
	for (size_t i = 0; i < rhs.d1; i++)
		matrix[i] = new BASE_TYPE[rhs.d2];
	for (size_t i = 0; i < rhs.d1; i++)
		for (size_t j = 0; j < rhs.d2; j++)
			matrix[i][j] = rhs.matrix[i][j];
}

size_t Matrix::getRows () const { return d1; }
	
size_t Matrix::getColumns () const { return d2; }

Matrix Matrix::operator + (const Matrix & rhs) const
{
	Matrix temp (d1, d2);
	if (d1 != rhs.d1 || d2 != rhs.d2)
		throw std::out_of_range("Different matrix sizes.");
	else
		for (size_t i = 0; i < d1; i++)
			for (size_t j = 0; j < d2; j++)
				temp.matrix[i][j] = rhs.matrix[i][j] + matrix[i][j];
	return temp;
}

bool Matrix::operator == (const Matrix & rhs) const
{
	bool res (true);

	if (d1 != rhs.d1 || d2 != rhs.d2)
		res = false;
	for (size_t i = 0; i < d1 && res; i++)
		for (size_t j = 0; j < d2 && res; j++)
			if (matrix[i][j] != rhs.matrix[i][j])
				res = false;
	return res;
}

Matrix Matrix::operator *= (const size_t &num) const
{
	for (size_t i = 0; i < d1; i++)
		for (size_t j = 0; j < d2; j++)
			matrix[i][j] *= num;
	return *this;
}

Matrix::~Matrix ()
{
	for (size_t i = 0; i < d1; i++)
		delete[]matrix[i];
	delete[]matrix;
}

Matrix::Proxy Matrix::operator[](const size_t &index ) const
{
	if( d1 != 0 && index >= d1 ) 
		throw std::out_of_range("Row index " + to_string(index) + " out of range [0.." + to_string(d1-1) + "]");
	else {
		sd2 = d2;
		return Proxy(matrix[index]);
	}
}
BASE_TYPE & Matrix::Proxy::operator[] (const size_t &index) const
{
	if( sd2 != 0 && index >= sd2 ) 
		throw std::out_of_range("Column index " + to_string(index) + " out of range [0.." + to_string(sd2-1) + "]");
	return row[index];
}

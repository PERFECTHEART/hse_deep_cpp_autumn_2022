#pragma once

#include <iostream>
#include <cassert>
#include <stdexcept>

using namespace std;

#define BASE_TYPE	int

class Matrix
{
public:
	class Proxy {
	public:
		Proxy(BASE_TYPE* pRow) : row(pRow) {}
		~Proxy() {}
		BASE_TYPE & operator[] (const size_t & index)const;
	private:
		BASE_TYPE* row;
	};

private:
  	BASE_TYPE **matrix;
  	size_t	d1;
  	size_t	d2;
public:
	Matrix (const size_t &d1,const size_t &d2);
	Matrix(const Matrix &rhs);
	~Matrix();
	Matrix	operator =(const Matrix &rhs)const;
	Matrix  operator +(const Matrix &rhs)const ;
	Matrix  operator *=(const size_t &num)const;
	bool	operator ==(const Matrix &rhs)const;
	size_t 	getRows()const;
	size_t 	getColumns()const;
	Proxy operator[](const size_t &index )const;

	friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
		for (size_t i=0; i < m.d1; ++i) {
			for (size_t j=0; j < m.d2; ++j) {
				os << m.matrix[i][j] << "\t" ;
			}
			os << '\n';
		}
		return os;
	}
};

#pragma once

#include <iostream>
#include <cassert>
#include <stdexcept>
#define	PLUS	'+'
#define ZERO	'0'
#define	MINUS	'-'

#define	BASE		10	// Основание системы счисления
#define	BASE_TYPE	int

class BigInt
{
public:
	BigInt();
	BigInt(std::string);
	BigInt(int32_t);
	BigInt(const BigInt &rhs);

	void zero_data();
	void no_leading_zeros();
	~BigInt();
	void Debug();
//	friend std::ostream& operator <<(std::ostream&, const BigInt&);
	// Арифметические операторы 
	BigInt operator +(const BigInt&) const;
	BigInt operator +(int32_t) const;
	BigInt operator -(const BigInt&) const;
	BigInt operator -(int32_t) const;
	BigInt operator *(const BigInt&) const;
	BigInt operator *(int32_t) const;
	// Унарный минус
	BigInt operator -();
	// Логические операторы
	bool operator ==(const BigInt&) const;
	bool operator !=(const BigInt&) const;
	bool operator <(const BigInt&) const;        
	bool operator >=(const BigInt&) const;
	bool operator <=(const BigInt&) const;
	bool operator >(const BigInt&) const;
	BigInt operator =(const BigInt &rhs);

	friend std::ostream &operator<<(std::ostream &os, const BigInt &m) {
		if ( m.sign == MINUS )
			os << '-';
		for (size_t i=0; i < m.len; ++i) {
			os << m.data[i];
		}
//		os << '\n';
		return os;
	}
 
private:
	BASE_TYPE	*data;	// данные
	size_t		len;	// размер данных
	char		sign;	// знак
};

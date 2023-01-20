#include "biginteger.hpp"
#include <string>
#include <string.h>

BigInt::BigInt(): data(nullptr), len(0), sign(ZERO)
{
#ifdef DEBUG0
	std::cout << "Empty constructor called" << std::endl;
#endif
	this->zero_data();
}

BigInt::~BigInt()
{

#ifdef DEBUG0
	std::cout << "Destructor called sign=[" << sign << "] len=" << len << std::endl;
	if (data == nullptr)
		std::cout << "Data already deleted" << std::endl;
#endif
	if (data != nullptr) {
		delete [] data;
		data = nullptr;
	}
}

void BigInt::zero_data()
{
	for( size_t i = 0; i < len; i++ )
		data[i] = 0;
}

void BigInt::no_leading_zeros()
{
	size_t skip;
	BASE_TYPE *tmp;

	for( skip=0; skip < len-1 && data[skip] == 0; skip++ )
		;
	if( skip > 0 ) 
	{
		tmp = new BASE_TYPE[len-skip];
		for( size_t i = skip; i < len; ++i )
			tmp[i-skip] = data[i];
		len -= skip;
		delete [] data;
		data = tmp;
	}
	// Ноль - всегда положительный и имеет особый знак 0
	if( len == 1 && data[0] == 0) {
		sign = ZERO;
	}
}

BigInt::BigInt(int32_t x) {
#ifdef DEBUG0
	std::cout << "Constructor from int32_t called" << std::endl;
#endif
	std::string str = std::to_string(x);
	switch( str[0] )
	{
		case '-':
			str = str.substr(1);
			sign = MINUS;
			break;
		case '0':
			sign = ZERO;
			break;
		default:
			sign = PLUS;
	}
	len = str.length();
#ifdef DEBUG0
	if ( data != nullptr ) {
		std::cout << "*** Non empty data in constructor." << std::endl;
	}
#endif
	data = new BASE_TYPE[len];
	for( int i = str.length()-1; i >= 0; i-- ) {
		data[i] = str[i] - '0';
	}
}

BigInt::BigInt(std::string str) {
#ifdef DEBUG0
	std::cout << "Constructor from string [" << str << "] called" << std::endl;
#endif
	// Пустую строчку интерпретируем как "0"
	if (str.length() == 0) {
        	sign = ZERO;
		len = 1;
		data = new BASE_TYPE[len];
		this->zero_data();
	}
	else {
		if (str[0] == '-') {
			str = str.substr(1);
			sign = MINUS;
		}
		else {
			sign = PLUS;
		}
		len = str.length();
		if( data != nullptr )
			delete [] data;
		data = new BASE_TYPE[len];
		this->zero_data();
 
		for( int i = str.length()-1; i >= 0; i-- ) {
			data[i] = str[i] - '0';
		}
		this->no_leading_zeros();
	}
}
                                
BigInt::BigInt (const BigInt & rhs):data(new BASE_TYPE[rhs.len]), len(rhs.len), sign(rhs.sign)	// копирующий конструктор
{
#ifdef DEBUG0
	std::cout << "Copying constructor called" << std::endl;
#endif
	memcpy(data, rhs.data, len * sizeof(BASE_TYPE));
}

BigInt::BigInt (BigInt&& rhs)
{
#ifdef DEBUG0
	std::cout << "Moving constructor called" << std::endl;
#endif
	if (*this != rhs)
	{
		data = rhs.data;
		rhs.data = nullptr;
		len = rhs.len;
		sign = rhs.sign;	
		rhs.len = 0;
		rhs.sign = '0';
	}
	#ifdef DEBUG0
	else
		std::cout << "Attempt to self-move" << std::endl;
	#endif
}

BigInt BigInt::operator -()
{
	BigInt copy(*this);
	if (sign != ZERO) {
		copy.sign = (sign == MINUS) ? PLUS : MINUS;
	}
	return copy;
}

BigInt BigInt::operator +(const BigInt& num2)
{
	size_t newlen = len + num2.len;
	BigInt res(std::string(newlen,'1'));
	BigInt lnum1(*this);
	res.zero_data();

	if( sign == num2.sign ) { // +1 + +2 | -2 + -4 | 0 + 0
		for( size_t i = 0; i < len; i++ ) { 
			res.data[newlen-i-1] = data[len-i-1];
		}
		for( size_t i = 0; i < num2.len; i++ ) {
			res.data[newlen-i-1] += num2.data[num2.len-i-1];
			res.data[newlen-i-2] += res.data[newlen-i-1] / BASE;
			res.data[newlen-i-1] = res.data[newlen-i-1] % BASE;		
		}
		res.no_leading_zeros();
		res.sign = sign;
	}
	else if( num2.sign == ZERO ) { // x + 0 = x
		res = lnum1;
	}
	else if ( sign == ZERO ) { // 0 + y = y
		res = num2;
	}
	else if( sign == MINUS ) { // -1 + +2
		res = num2 - -lnum1;
	}
	else {	// +1 + -2
		BigInt lnum2(num2);
		res = lnum1 - -lnum2;
	}
	return res;
}

BigInt BigInt::operator +(int32_t x)
{
	BigInt num2(x);
	return *this + num2;
}

BigInt BigInt::operator -(int32_t x)
{
	BigInt num2(x);
	return *this - num2;
}

BigInt BigInt::operator *(int32_t x)
{
	BigInt num2(x);
	return *this * num2;
}

BigInt BigInt::operator -(const BigInt& num2) const
{
	size_t newlen = len > num2.len ? len : num2.len;
	BigInt res(std::string(newlen,'1'));
	res.zero_data();
	BigInt lnum1(*this);
	BigInt lnum2 = num2;

	if( lnum2.sign == ZERO ) {	// x - 0 = x
		res = lnum1;
	}
	else if( sign == ZERO ) {	// 0 - x = -x
		res = -lnum2;
	} 
	else if( sign != lnum2.sign ) { // +1 - (-2) ; -2 - +1 = -2 + -1
		res = lnum1 + -lnum2;
	} 
	else if (sign == MINUS && num2.sign == MINUS) {	// -3 - -9 = -3 + 9 = 9 - 3
		res = -lnum2 - -lnum1;
	}
	else if ( lnum1 != lnum2 ) { // Знаки разные, оба числа ненулевые
		if( lnum1 < num2 ) { // второе число больше по модулю, чем первое. Знак минус в итоге
			res.sign = MINUS;
			for( size_t i = 0; i < num2.len; i++ ) { // Копируем 2 число в результат и вычитаем в столбик
				res.data[newlen-i-1] = num2.data[num2.len-i-1];
			}
			for( size_t i = 0; i < len; i++ ) {
				res.data[newlen-i-1] -= data[len-i-1];
				if( res.data[newlen-i-1] < 0 ) {
					res.data[newlen-i-1] += BASE;
					res.data[newlen-i-2]--;
				}
			}
		}
		else {
			res.sign = PLUS;
			for( size_t i = 0; i < len; i++ ) { // Копируем 1 число в результат
				res.data[newlen-i-1] = data[len-i-1];
			}
			for( size_t i = 0; i < num2.len; i++ ) {
				res.data[newlen-i-1] -= num2.data[num2.len-i-1];
				if( res.data[newlen-i-1] < 0 ) {
					res.data[newlen-i-1] += BASE;
					res.data[newlen-i-2]--;
				}
			}
		}
		for( size_t i = 0; i < newlen; ++i ) {	// занимаем соседние разряды, если второе число короче первого.
			if( res.data[newlen-i-1] < 0 ) {
				res.data[newlen-i-1] += BASE;
				res.data[newlen-i-2]--;
			}
		}
	}
	res.no_leading_zeros();
	return res;
}

BigInt BigInt::operator *(const BigInt& num2)
{
	size_t newlen = len + num2.len;
	BigInt res(std::string(newlen,'1'));
	res.zero_data();

	if( sign == ZERO || num2.sign == ZERO ) {
		res.sign = ZERO;
	}
	else if( sign == num2.sign ) {
		res.sign = PLUS;
	}
	else {
		res.sign = MINUS;
	}
	if( res.sign != ZERO ) {
		for( size_t i = 0; i < num2.len; i++ ) { 
			for( size_t j = 0; j < len; j++ ) {
				res.data[newlen-i-j-1] += num2.data[num2.len-i-1] * data[len-j-1];
				res.data[newlen-i-j-2] += res.data[newlen-i-j-1] / BASE;
				res.data[newlen-i-j-1] = res.data[newlen-i-j-1] % BASE;		
			}
		}
		res.no_leading_zeros();
	}
	return res;
}

void BigInt::Debug()
{
#ifdef DEBUG0
	std::cout << "len: " << len << "  sign: [" << sign << "] data: ";

	for( size_t i = 0; i < len; ++i )
		std::cout << data[i] << ".";    
	std::cout << std::endl;
#endif
}

bool BigInt::operator ==(const BigInt& num2) const
{
	bool res = true;

	if( sign != num2.sign || len != num2.len )
		res = false;
	else {
		for( size_t i=0; i<len; ++i ) 
			if( data[i] != num2.data[i] ) {
				res = false;
				break;
			}
	}
	return res;
}

bool BigInt::operator !=(const BigInt& num2) const
{
	return ! (*this == num2);
}

bool BigInt::operator <(const BigInt& num2) const
{
	bool res = false;
	
	if( sign == ZERO ) { // 0 < +1
		if( num2.sign == PLUS ) {
			res = true;
		}
	}
	else if( sign == MINUS ) { // -1 < 0 | -1 < +1
		if( num2.sign != MINUS ) {
		       	res = true;
		}
	}
	if( sign == num2.sign && sign != 0 ) {
		if( len < num2.len ) {
			res = (sign == PLUS) ? true : false;
		}
		else if( len > num2.len ) {
			res = (sign == MINUS);
		}
		else { // Оба числа одной длины
			for( size_t i=0; i<len; ++i ) {
				if (data[i] < num2.data[i] && sign == PLUS && num2.sign == PLUS) {
					res = true;
					break;	
				}
				if (data[i] > num2.data[i] && sign == MINUS && num2.sign == MINUS) {
					res = true;
					break;	
				}

			}
		}
	}
	
	return res;
}
bool BigInt::operator >=(const BigInt& num2) const
{
	return ! (*this < num2);
}

bool BigInt::operator <=(const BigInt& num2) const
{
	return (*this < num2) || (*this == num2);
}

bool BigInt::operator >(const BigInt& num2) const
{
	return !(*this <= num2);
}

BigInt BigInt::operator =(const BigInt & rhs)	// перегружаем оператор =
{
	len = rhs.len;
	sign = rhs.sign;
	if( data != nullptr )
	{
		delete [] data;
	}
	data = new BASE_TYPE(len);
	memcpy( data, rhs.data, len * sizeof(BASE_TYPE));
	return *this;
}

BigInt& BigInt::operator =(BigInt&& num2) // присваивание перемещением
{
#ifdef DEBUG0
	std::cout << "Assignment by movement" << std::endl;
#endif
	if( *this != num2 ) {
		if( data != nullptr )
			delete [] data;
		data = num2.data;
		len = num2.len;
		sign = num2.sign;
		num2.data = nullptr;
	}
	#ifdef DEBUG0
	else
		std::cout << "Attempt to self-move assign" << std::endl;
	#endif
	return *this;
}

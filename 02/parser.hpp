#pragma once

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

//using namespace std;

#define	MAX_DATA	1000

class TokenParser
{
public:
//	TokenParser() = default;

    	// Устанавливаем callback-функцию перед стартом парсинга.
	int SetStartCallback( void (*f)() );

    	// Устанавливаем callback-функцию после окончания парсинга.
	int SetEndCallback( void (*f)() );

    	// Устанавливаем callback-функцию для обработки чисел.
	int SetDigitTokenCallback( int (*f)(uint64_t) );

    	// Тут другие методы для установки callback-функций.
    	//...
	void Parse(const std::string &);
};

class Data
{
public:
	uint64_t	num;
	std::string	txt;
	int		len; // if 0, then numeric

	void ShowValue(char *s1, char *s2);
	unsigned int Store( std::string str, unsigned int pos);
};

void ShowCounters();
void ShowValues();
int ShowOdd(uint64_t);
int ShowEven(uint64_t);


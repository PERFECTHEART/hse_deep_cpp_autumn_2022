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

using namespace std;

#define	MAX_DATA	1000

class TokenParser
{
public:
//	TokenParser() = default;

    	// ������������� callback-������� ����� ������� ��������.
	SetStartCallback( void (*f)() );

    	// ������������� callback-������� ����� ��������� ��������.
	SetEndCallback( void (*f)() );

    	// ������������� callback-������� ��� ��������� �����.
	SetDigitTokenCallback( int (*f)(uint64_t) );

    	// ��� ������ ������ ��� ��������� callback-�������.
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

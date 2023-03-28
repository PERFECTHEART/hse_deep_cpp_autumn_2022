#pragma once

#include <sstream> 

#ifndef __linux__ 
#include <locale>
#include <windows.h>
#endif
using namespace std;

#define MAXN 200 // максимальное количество аргументов

int sarg_cnt = 0;
std::string *sarg = nullptr;

class MyException : public runtime_error
{
public:
	MyException(const std::string &msg):runtime_error(msg)
	{
//		this->dataState_ = dataState_;
	}
//	int GetDataState() { return dataState_; }
private:
//	int dataState_; // Reserved for future use
};

size_t check_template(const std::string &, const size_t);
std::string build_string(const std::string *);

void inline convArgs()
{
	throw MyException("Вызвана функция без аргументов.");
}

template <typename T>
void inline convArgs(T &&x)
{
	std::stringstream buffer;
	buffer << std::forward<T>(x);
	sarg[sarg_cnt++] = buffer.str();
}

template <typename FirstT, typename ...Args>
void inline convArgs(FirstT &&first, Args&&...remaining)
{
	convArgs(std::forward<FirstT>(first));
	convArgs(std::forward<Args>(remaining)...);
}

template <typename ...Params>
std::string format(Params&&... params)
{
	size_t code;
	sarg_cnt = 0;
	std::string res;

	if( sarg == nullptr )
	{
		sarg = new std::string[MAXN];
	}
	
	convArgs(std::forward<Params>(params)...);
	if( sarg_cnt > 0 ) {
		// sarg_cnt - 1, потому что собственно строка с форматом не является аргументом, который надо выводить
		if ((code = check_template( sarg[0], sarg_cnt - 1 )) == 0) {
			res = build_string( sarg );
		}
	}
	delete [] sarg;
	sarg = nullptr;
	return res;
}


// Функция получает на вход строку форматирования и число аргументов при вызове format()
// Выявляются несоответствия
//   вложенный символ {
//   нецифровой символ после {
//   отсутствие } после {
//   наличие } до {
// При отсутсвии ошибок код возврата - 0
// Иначе - номер строки кода
//#define NO_EMPTY_BRACES

size_t check_template(const std::string &str, const size_t cnt)
{
	size_t ret = 0;
	int open_brace = 0; // открывающая фигурная скобка
	size_t val = 0;
	size_t *arg_use_counter;
#ifdef NO_EMPTY_BRACES
	size_t digits_after_brace;	// Счётчик цифр после {
#endif

	arg_use_counter = new size_t[cnt];
#ifndef __linux__ 
	SetConsoleOutputCP(CP_UTF8);
#endif

	for( size_t i=0; i < cnt; i++ )
		arg_use_counter[i] = 0;	// При обнаружении номера аргумента проставится счётчик.

	for(size_t i = 0; i < str.size() && ret == 0; i++) {
		switch( str[i] )
		{
		case '{':
			if( open_brace == 1 ) {
				delete [] arg_use_counter;
				throw MyException("Недопустимый вложенный символ: {.");
				ret = __LINE__;
			}
			else {
				open_brace = 1;
				val = 0;
#ifdef NO_EMPTY_BRACES
				digits_after_brace = 0;
#endif
			}
			break;
		case '}':
			if ( open_brace == 1 ) {
				open_brace = 0;
				if( val >= cnt ) {
					delete [] arg_use_counter;
					throw MyException("Номер запрошенного аргумента превышает число аргументов в вызове.");
					ret = __LINE__;
				}
				else {
#ifdef NO_EMPTY_BRACES
					if(digits_after_brace == 0) {
						delete [] arg_use_counter;
						throw MyException("Нет значения внутри {}.");
						ret = __LINE__;
					}
#endif
					arg_use_counter[val]++;
				}
			}
			else {
				delete [] arg_use_counter;
				throw MyException("Символу } не сопутствует символ {.");
				ret = __LINE__;
			}
			break;
		default:
			if( open_brace == 1 ) {
				if( !isdigit(str[i]) ) {
					delete [] arg_use_counter;
					throw MyException("Некорректный символ " + str.substr(i,1) + " внутри {}.");
					ret = __LINE__;
				}
				else {
#ifdef NO_EMPTY_BRACES
					digits_after_brace++;
#endif
					val = val * 10 + str[i] - '0';
				}
			}
			break;
		}
	}
	if( open_brace == 1 ) {
		delete [] arg_use_counter;
		throw MyException("Нет } после {." );
		ret = __LINE__;
	}
	for( size_t i = 0; i < cnt; i++ ) {
		if( arg_use_counter[i] == 0 ) {
			delete [] arg_use_counter;
			throw MyException("Аргумент " + std::to_string(i) + " не использован.");
			ret = __LINE__;
		}
	}
	delete [] arg_use_counter;
	return ret;
}

std::string build_string( const std::string *arg )
{
	std::string res = "";
	size_t brace = 0;
	size_t val;

	for( size_t i = 0; arg[0][i] != '\0'; i++ ) {
		switch( arg[0][i] ) {
			case '{':
				brace = 1;
				// Получаем число после {
				val = std::stoi(arg[0].substr(i+1)) + 1; // arg[0] не является аргументом для строки arg[0]
				res += arg[val];
				break;
			case '}':
				brace = 0;
				break;
			default:
				if( brace == 0 )
					res += arg[0][i];
				break;
		}
	}
	return res;
}

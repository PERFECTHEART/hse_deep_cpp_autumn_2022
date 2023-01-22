#include <string>
#include <iostream>
#include <cstdlib>

#ifndef __linux__ 
#include <locale>
#include <windows.h>
#endif

// Функция получает на вход строку форматирования и число аргументов при вызове format()
// Выявляются несоответствия
//   вложенный символ {
//   нецифровой символ после {
//   отсутствие } после {
//   наличие } до {
// При отсутсвии ошибок код возврата - 0
// Иначе - номер строки кода
#define NO_EMPTY_BRACES

size_t check_template(const std::string str, const size_t cnt)
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
				throw std::invalid_argument("Недопустимый вложенный символ: {.");
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
					throw std::invalid_argument("Номер запрошенного аргумента превышает число аргументов в вызове.");
					ret = __LINE__;
				}
				else {
#ifdef NO_EMPTY_BRACES
					if(digits_after_brace == 0) {
						delete [] arg_use_counter;
						throw std::invalid_argument("Нет значения внутри {}.");
						ret = __LINE__;
					}
#endif
					arg_use_counter[val]++;
				}
			}
			else {
				delete [] arg_use_counter;
				throw std::invalid_argument(" Символу } не сопутствует символ {.");
				ret = __LINE__;
			}
			break;
		default:
			if( open_brace == 1 ) {
				if( !isdigit(str[i]) ) {
					delete [] arg_use_counter;
					throw std::invalid_argument("Некорректный символ " + str.substr(i,1) + " внутри {}.");
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
		throw std::invalid_argument("Нет } после {." );
		ret = __LINE__;
	}
	for( size_t i = 0; i < cnt; i++ ) {
		if( arg_use_counter[i] == 0 ) {
			delete [] arg_use_counter;
			throw std::invalid_argument("Аргумент " + std::to_string(i) + " не использован.");
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

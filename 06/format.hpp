#pragma once

#include <iostream>
#include <cstring>
#include <sstream> 

#define MAXN 200 // максимальное количество аргументов

#ifndef SARG_DEF
#define SARG_DEF
int sarg_cnt;
std::string *sarg = nullptr;
#endif

size_t check_template(const std::string, const size_t);
std::string build_string(const std::string *);

void convArgs()
{
	throw std::invalid_argument("Вызвана функция без аргументов.");
}

template <typename T>
void convArgs(T &&x)
{
	std::stringstream buffer;
	buffer << std::forward<T>(x);
	sarg[sarg_cnt++] = buffer.str();
}

template <typename FirstT, typename ...Args>
void convArgs(FirstT &&first, Args&&...remaining)
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
// Ошибки формата ловятся в check_template и срабатывает соответствующий throw
//		else {
//			std::cout << "#" << code << " Неверный формат строки" << std::endl;
//		}
	}
	delete [] sarg;
	sarg = nullptr;
	return res;
}

CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0
TEST_LIB=-lgtest -lgtest_main -lpthread

all: matrix.o tests.exe

matrix.o: matrix.cpp matrix.hpp tests.cpp Makefile
	$(CC) $(FLAGS) -c matrix.cpp

tests.exe: matrix.o tests.cpp
	$(CC) $(FLAGS) matrix.o tests.cpp -o tests.exe $(TEST_LIB)

test: tests.exe
	tests.exe

clean:
	del *.o tests.exe

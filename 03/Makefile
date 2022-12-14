CC=g++
FLAGS=-std=c++03 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0
TEST_LIB=

all: matrix.o main.exe

matrix.o: matrix.cpp matrix.hpp
	$(CC) $(FLAGS) -c matrix.cpp

main.exe: matrix.o main.cpp
	$(CC) $(FLAGS) matrix.o main.cpp -o main.exe $(TEST_LIB)

test: main.exe
	main.exe

clean:
	rm -f *.o main.exe

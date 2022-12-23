CC=g++
FLAGS=-std=c++11 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0
TEST_LIB=


biginteger.o: biginteger.cpp biginteger.hpp Makefile
	$(CC) $(FLAGS) -c biginteger.cpp


main: biginteger.hpp biginteger.o main.cpp Makefile
	$(CC) $(FLAGS) biginteger.o main.cpp -o main $(TEST_LIB)

test: main
	main.exe

clean:
	rm -f *.o main.exe

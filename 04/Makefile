CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG
TEST_LIB=-lgtest_main -lgtest -lpthread

all: biginteger.o main

biginteger.o: biginteger.cpp biginteger.hpp Makefile
	$(CC) $(FLAGS) -c biginteger.cpp


main: biginteger.hpp biginteger.o main.cpp Makefile
	$(CC) $(FLAGS) biginteger.o main.cpp -o main.exe $(TEST_LIB)

test: main.exe
	./main

valtest: main
	valgrind ./main

clean:
	rm -f *.o main.exe

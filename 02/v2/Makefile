CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0 -fpermissive
TEST_LIB=-lgtest_main -lgtest -lpthread

all: test_bin test

test_bin: tests.cpp parser.hpp Makefile
	$(CC) $(FLAGS) tests.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin
clean:
	del *.o test_bin*

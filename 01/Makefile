CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0 -O3
TEST_LIB=-lgtest -lgtest_main -lpthread

all: allocator test_bin

allocator: allocator.cpp allocator.hpp
	$(CC) $(FLAGS) -c allocator.cpp

test_bin: allocator.o tests.cpp Makefile
	$(CC) $(FLAGS) allocator.o tests.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

mem: test_bin
	valgrind ./test_bin

mem-s: test_bin
	valgrind -s ./test_bin

clean:
	rm *.o* test_bin*

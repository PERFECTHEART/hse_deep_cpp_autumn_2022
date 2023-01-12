CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG -O3
TEST_LIB=-lgtest -lgtest_main -lpthread

all: allocator test_bin

allocator: allocator.cpp allocator.hpp
	$(CC) $(FLAGS) -c allocator.cpp

test_bin: allocator.o tests.cpp
	$(CC) $(FLAGS) allocator.o tests.cpp -o test_bin $(TEST_LIB)

test: allocator.o test_bin.exe
	test_bin.exe

clean:
	del *.o test_bin.*

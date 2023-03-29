CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG
TEST_LIB=-lgtest_main -lgtest -lpthread

all: test_bin.exe test

test_bin.exe: serializer.hpp tests.cpp Makefile
	$(CC) $(FLAGS) $(INC) tests.cpp -o test_bin.exe $(TEST_LIB)

test: test_bin.exe
	./test_bin.exe

clean:
	del *.o test_bin.exe

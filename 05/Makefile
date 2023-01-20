CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0
TEST_LIB=-lgtest_main -lgtest -lpthread

test_bin.exe: serializer.hpp main.cpp Makefile
	$(CC) $(FLAGS) $(INC) main.cpp -o test_bin $(TEST_LIB)

test: test_bin.exe
	./test_bin

clean:
	rm -f *.o test_bin

CC=g++
FLAGS=-std=c++03 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG0
TEST_LIB=

all: parser test_bin

parser: parser.cpp parser.hpp
	$(CC) $(FLAGS) -c parser.cpp

test_bin: parser.o tests.cpp
	$(CC) $(FLAGS) parser.o tests.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin < test.txt

e1:
	./test_bin < e1.txt

clean:
	rm -f *.o test_bin.*

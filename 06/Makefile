CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DNO_EMPTY_BRACES
TEST_LIB=-lgtest_main -lgtest -lpthread
EXE=test_bin.exe

all: ${EXE} test

$(EXE): format.hpp tests.cpp Makefile
	$(CC) $(FLAGS) tests.cpp -o $(EXE) $(TEST_LIB)

test: $(EXE)
	./$(EXE)

clean:
	rm -f *.o $(EXE)

CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG
TEST_LIB=-lgtest_main -lgtest -lpthread
EXE=test_bin.exe

all: test_bin.exe

$(EXE): avl_array.hpp main.cpp Makefile
	$(CC) $(FLAGS) main.cpp -o $(EXE) $(TEST_LIB)

test: $(EXE)
	./$(EXE)

clean:
	rm -f *.o $(EXE)

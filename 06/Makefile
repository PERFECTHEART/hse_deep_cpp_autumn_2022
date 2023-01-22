CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG
TEST_LIB=-lgtest_main -lgtest -lpthread
EXE=test_bin.exe

all: format.o test_bin.exe

format.o: format.hpp format.cpp Makefile
	$(CC) $(FLAGS) -c format.cpp

$(EXE): format.o format.hpp main.cpp Makefile
	$(CC) $(FLAGS) format.o main.cpp -o $(EXE) $(TEST_LIB)

test: $(EXE)
	./$(EXE)

clean:
	rm -f *.o $(EXE)

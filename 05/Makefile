CC=g++
FLAGS=-std=c++20 -LD:\msys64\mingw64\lib 
INC=-ID:\msys64\mingw64\include 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: test_bin

test_bin: serializer.hpp main.cpp
	$(CC) $(FLAGS) $(INC) main.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	del *.o test_bin.exe
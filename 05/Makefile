CC=g++
FLAGS=-std=c++20 -LD:\msys64\mingw64\lib 
INC=-ID:\msys64\mingw64\include 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: serializer.o test_bin

serializer.o: serializer.cpp serializer.hpp
	$(CC) $(FLAGS) $(INC) -c serializer.cpp

test_bin: serializer.o main.cpp
	$(CC) $(FLAGS)  $(INC) serializer.o main.cpp -o test_bin $(TEST_LIB)

test: test_bin
	./test_bin

clean:
	del *.o test_bin.exe
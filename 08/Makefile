# https://github.com/mailcourses/hse_deep_cpp_autumn_2022/blob/main/lesson-09/homework.md
.DEFAULT_GOAL := install

all: install test

install: utils.c
	python setup.py install

test:
	./main.py

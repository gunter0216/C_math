.PHONY: all clean test s21_math.a check gcov_report
CC=gcc -std=c11
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	CC += -D OS_MAC
else
	CC += -D OS_LINUX
endif

default: all

all: clean s21_math.a test

test: s21_math.a
ifeq ($(OS), Darwin)
	$(CC) -o test test.c s21_math.a -lcheck
else
	$(CC) -o test test.c s21_math.a -lcheck -lm -lsubunit -lrt -lpthread
endif
	./test

s21_math.a: s21_math.o
	ar rcs s21_math.a s21_math.o
	ranlib $@
	cp $@ lib$@

s21_math.o: s21_math.c
	$(CC) -c -o s21_math.o s21_math.c

gcov_report: s21_math.a
ifeq ($(OS), Darwin)
	$(CC) --coverage -o testresult test.c s21_math.c -lcheck
else
	$(CC) --coverage -o testresult test.c s21_math.c -lcheck -lm -lsubunit -lrt -lpthread
endif
	./testresult
	lcov -t "testresult" -o testresult.info -c -d .
	genhtml -o report testresult.info

check:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extension=c s21_math.c s21_math.h test.c
ifeq ($(OS), Darwin)
	CK_FORK=no leaks -atExit -- ./test
else
	valgrind -s --track-origins=yes --leak-check=full --log-file="leak.log" ./test
endif

clean:
	rm -rf test *.a *.o *.out *.gc* *.info *.log report testresult CPPLINT.cfg

rebuild: clean all
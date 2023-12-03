CC = clang++
CFLAGS = -std=c++14 -Wall -Wextra -pedantic -g -I../include -DUNIT_TEST

all: main

interval.o: ../src/Interval.cpp
	$(CC) $(CFLAGS) $^ -c -o $@
domain.o: ../src/Domain.cpp
	$(CC) $(CFLAGS) $^ -c -o $@
main.o: main.cpp
	$(CC) $(CFLAGS) $^ -c -o $@
main: main.o domain.o interval.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o main
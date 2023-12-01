CC = clang++
CFLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic -g -I../include -DUNIT_TEST

all: main

domain.o: ../src/domain.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

main: main.o domain.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o main
MAKEFLAGS += --no-builtin-rules

SRC:=$(wildcard *.c)
TARGETS:=$(patsubst %.c, %, $(SRC))

all: ${TARGETS}

%: %.c
	clang -emit-llvm -S -fno-discard-value-names -Xclang -disable-O0-optnone -c -o $@.ll $<
	opt -load ../build/OOBChecker.so -OOBChecker $@.ll -disable-output 2>&1 > $@.out | tee $@.err
	@echo "\n"


clean:
	rm -f *.ll *.out *.err
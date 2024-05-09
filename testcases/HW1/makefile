CC=../../AFLplusplus/afl-clang-fast++
FZ=../../AFLplusplus/afl-fuzz
CFLAGS=-O0 -g --coverage
.PHONY : fuzz clean

#run
run: main.out

# build btree
main.out: main.cpp
	AFL_USE_ASAN=1 $(CC) -o $@ $^ #$(CFLAGS)

#with trace-pc-guard
crash_main.out: main.cpp
	clang -g -fsanitize=address -fsanitize-coverage=trace-pc-guard $^ -o $@ trace-pc-fo.o

fuzz: main.out
	$(FZ) -i inputs -o outputs ./$^


# fault localization and find 10 buggy line
sbfl: 
	gcc sbfl.c -o sbfl.out -DD=0

clean:
	rm -rf *.out 
	rm -rf *.gcda *.gcov *.gcno



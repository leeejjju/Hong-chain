CC=../AFLplusplus/afl-clang-fast
# CFLAGS= -g -fsanitize=address -fsanitize-coverage=trace-pc-guard
# CFLAGS=-O0 -g --coverage 
CFLAGS=-O0 -g --coverage 
.PHONY : fuzz clean


# build btree
btree.out: btree.c
	AFL_USE_ASAN=1 $(CC) $(CFLAGS) $^ -o $@


# run afl-fuzz
fuzz:
	../AFLplusplus/afl-fuzz -i inputs -o outputs ./btree

clean:
	rm -rf *.out 
	rm -rf *.gcda *.gcov *.gcno


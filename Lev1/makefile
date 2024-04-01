CC=../AFLplusplus/afl-clang-fast
# CFLAGS= -g -fsanitize=address -fsanitize-coverage=trace-pc-guard
# CFLAGS=-O0 -g --coverage 
CFLAGS=-O0 -g --coverage 

# build btree
btree: btree.c
	AFL_USE_ASAN=1 $(CC) $(CFLAGS) btree.c -o btree


# run afl-fuzz
bt:
	../AFLplusplus/afl-fuzz -i inputs -o outputs ./btree

clean:
	rm -rf btree 

gclean:
	rm -rf *.gcda *.gcov 


CC=../AFLplusplus/afl-clang-fast
FZ=../AFLplusplus/afl-fuzz
# CFLAGS= -g -fsanitize=address -fsanitize-coverage=trace-pc-guard
# CFLAGS=-O0 -g --coverage 
CFLAGS=-O0 -g --coverage 
.PHONY : fuzz clean

#run
run: btree.out crash_btree.out queue_coverage.out crash_coverage.out

# build btree
btree.out: btree.c
	AFL_USE_ASAN=1 $(CC) $(CFLAGS) $^ -o $@

#with trace-pc-guard
crash_btree.out: btree.c
	clang -g -fsanitize=address -fsanitize-coverage=trace-pc-guard $^ -o $@ trace-pc-fo.o

# run afl-fuzz
fuzz: btree.out
	 $(FZ) -i inputs -o outputs ./$^

#get the queue_coverage
FL.out: fault_localization.c
	gcc $^ -o $@ -DD=0
	./$@

clean:
	rm -rf *.out 
	rm -rf *.gcda *.gcov *.gcno



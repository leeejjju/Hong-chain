CC=../../AFLplusplus/afl-clang-fast++

go: test compare

test:
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 testcopy.cpp -o testcopy.out
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 test.cpp -o test.out

compare: compare.cpp
	$(CC) compare.cpp -o compare.out

clean: 
	rm -f *.out

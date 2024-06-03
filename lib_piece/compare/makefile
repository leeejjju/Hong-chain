CC=../../AFLplusplus/afl-clang-fast++

go: test compare

test:
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 testcopy.cpp -o testcopy.out
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 test.cpp -o test.out
    
bst:
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 solution_bst.cpp -o solution_bst.out
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 submission_bst3.cpp -o submission_bst3.out
	AFL_USE_ASAN=1 $(CC) -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 submission_bst1.cpp -o submission_bst1.out



compare: compare.cpp
	$(CC) compare.cpp -o compare.out

clean: 
	rm -f *.out
test:
	AFL_USE_ASAN=1 ../AFLplusplus/afl-clang++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 testcopy.cpp -o testcopy
	AFL_USE_ASAN=1 ../AFLplusplus/afl-clang++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -g -O0 test.cpp -o test

compare: compare.cpp
	clang++ compare.cpp -o compare
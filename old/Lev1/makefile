CC=../AFLplusplus/afl-clang-fast
FZ=../AFLplusplus/afl-fuzz
# CFLAGS= -g -fsanitize=address -fsanitize-coverage=trace-pc-guard
# CFLAGS=-O0 -g --coverage 
CFLAGS=-O0 -g --coverage 
.PHONY : fuzz clean

TARGET = ""

# build TARGET
$(TARGET).out: $(TARGET).c
	AFL_USE_ASAN=1 $(CC) $(CFLAGS) $^ -o $@

#with trace-pc-guard
crash_$(TARGET).out: $(TARGET).c
	clang -g -fsanitize=address -fsanitize-coverage=trace-pc-guard $^ -o $@ trace-pc-fo.o

# run afl-fuzz
fuzz:
	../AFLplusplus/afl-fuzz -i inputs -o outputs ./$(TARGET).out

# fault localization and find 10 buggy line
sbfl: 
	gcc sbfl.c -o sbfl.out -DD=0

clean:
	rm -rf *.out 
	rm -rf *.gcda *.gcov *.gcno



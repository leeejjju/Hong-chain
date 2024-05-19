CC=AFLplusplus/afl-clang++
FZ=AFLplusplus/afl-fuzz


# variables
INCLUDE_PATH ?= include
LIB_PATH ?= 
LIB_NAME ?= 
SID ?= 


CRASH_DIR = outputs/default/crashes/

check_for_crashes:
	@if ls $(CRASH_DIR)id* 1> /dev/null 2>&1; then \
	    echo "Crash Occurred....\nPlease fix the code." > outputs/report; \
	else \
	    echo "No crashes detected\nYOU CAN USE DAEMON!!" > outputs/report; \
	fi
	@echo "\nexecs_done        : `grep -Po 'execs_done\s*:\s*\K\d+' outputs/default/fuzzer_stats`" >> outputs/report; \
	echo "saved_crashes     : `grep -Po 'saved_crashes\s*:\s*\K\d+' outputs/default/fuzzer_stats`" >> outputs/report; \
	echo "saved_hangs       : `grep -Po 'saved_hangs\s*:\s*\K\d+' outputs/default/fuzzer_stats`" >> outputs/report

	


# create subdirectory of students
# make create_subdir SID=22000711
create_subdir:
	mkdir -p submissions/$(SID)


# make solution execution file
solution:
	mkdir -p .log
ifeq ($(INCLUDE_PATH),)	# INCLUDE_PATH is empty
ifeq ($(LIB_NAME),)	# LIB_NAME is also empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp solution.cpp -o solution.out
else	# INCLUDE_PATH is empty, but LIB_NAME is defiend
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp solution.cpp -L$(LIB_PATH) -l$(patsubst lib%.a,%,$(LIB_NAME)) -o solution.out
endif
else ifeq ($(LIB_NAME),)	# INCLUDE_PATH is defiend, but LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp solution.cpp -I$(INCLUDE_PATH) -o solution.out
else	# INCLUDE_PATH and LIB_NAME are defiend
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp solution.cpp -I$(INCLUDE_PATH) -L$(LIB_PATH) -l$(patsubst lib%.a,%,$(LIB_NAME)) -o solution.out
endif

#	make solution INCLUDE_PATH=include
#	$(CC) -std=c++11 test_driver.cpp solution.cpp -I../include -L../lib -lnowic_linux -o solution



# make submission execution file
submission:
	mkdir -p .log
ifeq ($(INCLUDE_PATH),)	# INCLUDE_PATH is empty
ifeq ($(LIB_NAME),)	# LIB_NAME is also empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp submissions/$(SID)/submission.cpp -o submissions/$(SID)/submission.out
else	# INCLUDE_PATH is empty, but LIB_NAME is defiend
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp submissions/$(SID)/submission.cpp -L$(LIB_PATH) -l$(patsubst lib%.a,%,$(LIB_NAME)) -o submissions/$(SID)/submission.out
endif
else ifeq ($(LIB_NAME),)	# INCLUDE_PATH is defiend, but LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp  submissions/$(SID)/submission.cpp -I$(INCLUDE_PATH) -o submissions/$(SID)/submission.out
else	# INCLUDE_PATH and LIB_NAME are defiend
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp submissions/$(SID)/submission.cpp -I$(INCLUDE_PATH) -L$(LIB_PATH) -l$(patsubst lib%.a,%,$(LIB_NAME)) -o submissions/$(SID)/submission.out
endif

# $(CC) -std=c++11 test_driver.cpp submission.cpp -Iinclude -Llib -libnowic_db -o submission
# how to use :s
# make submission SID=22000711 INCLUDE_PATH=include LIB_PATH=lib LIB_NAME=libnowic_db.a 


# solution file fuzzing
fz_solution: 
	mkdir -p outputs
	timeout 10s env AFL_NO_AFFINITY=1 $(FZ) -i inputs -o outputs ./solution.out 
# make fz_solution 


# submission file fuzzing
fz_submission: 
# make report folder to save raw report
	mkdir -p submissions/$(SID)/report/crash	
	mkdir -p submissions/$(SID)/report/incorrect
	mkdir -p submissions/$(SID)/report/log/crash	
	mkdir -p submissions/$(SID)/report/log/incorrect
# make outputs folder to save fuzz results
	mkdir -p submissions/$(SID)/outputs	
	timeout 10s env AFL_NO_AFFINITY=1 $(FZ) -i inputs -o submissions/$(SID)/outputs submissions/$(SID)/submission.out 
# how to use :
# make fz_submission SID=22000711



clean:
	rm -rf outputs
	rm *.out





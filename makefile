CC=AFLplusplus/afl-clang++
FZ=AFLplusplus/afl-fuzz


# variables
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
# how to use: make create_subdir SID=22000711
create_subdir:
	mkdir -p submissions/$(SID)


# make solution execution file, LIB_NAME is optional
# how to use: make solution LIB_NAME=libnowic_db.a 
solution:
	mkdir -p .log
ifeq ($(LIB_NAME),) # LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp solution.cpp -I./include -o solution.out
else    
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp solution.cpp -I./include -L./lib -l$(patsubst lib%.a,%,$(LIB_NAME)) -o solution.out
endif


# make submission execution file, LIB_NAME is optional but SID is essential
# how to use : make submission SID=22000711 LIB_NAME=libnowic_db.a 
submission:
	mkdir -p .log
ifeq ($(LIB_NAME),) # LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp  submissions/$(SID)/submission.cpp -I./include -o submissions/$(SID)/submission.out
else    
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp submissions/$(SID)/submission.cpp -I./include -L./lib -l$(patsubst lib%.a,%,$(LIB_NAME)) -o submissions/$(SID)/submission.out
endif


# solution file fuzzing
# how to use: make fz_solution 
fz_solution: 
	mkdir -p outputs
	timeout 10s env AFL_NO_AFFINITY=1 $(FZ) -i inputs -o outputs ./solution.out 



# submission file fuzzing
# how to use : make fz_submission SID=22000711
fz_submission: 
# make report folder to save raw report
	mkdir -p submissions/$(SID)/report/crash    
	mkdir -p submissions/$(SID)/report/incorrect
	mkdir -p submissions/$(SID)/report/log/crash    
	mkdir -p submissions/$(SID)/report/log/incorrect
# If files exist in the above directories, delete them
	rm -rf submissions/$(SID)/report/crash/*
	rm -rf submissions/$(SID)/report/incorrect/*
	rm -rf submissions/$(SID)/report/log/crash/*
	rm -rf submissions/$(SID)/report/log/incorrect/*
# make outputs folder to save fuzz results
# exit status code 124 from the timeout command indicates that the command terminated due to a timeout
	mkdir -p submissions/$(SID)/outputs 
	timeout 10s env AFL_NO_AFFINITY=1 $(FZ) -i inputs -o submissions/$(SID)/outputs ./submissions/$(SID)/submission.out
	rm -rf .log/*


clean:
	rm -rf outputs
	rm -f *.out







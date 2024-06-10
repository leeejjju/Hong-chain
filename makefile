CC1 = AFLplusplus/afl-clang++
CC2 = g++
FZ = AFLplusplus/afl-fuzz

IP = 127.0.0.1
PORT = 9090
CFLAGS = -pthread -I/usr/include/jsoncpp -lcurl -ljsoncpp



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
	echo "saved_hangs       : `grep -Po 'saved_hangs\s*:\s*\K\d+' outputs/default/fuzzer_stats`" >> outputs/report \	
	avg_time=$$(grep -Po 'execs_per_sec\s*:\s*\K[\d.]+' outputs/default/fuzzer_stats); \
	avg_time=`echo "scale=2; 1000/$$avg_time" | bc`; \
	echo "average_time      : $$avg_time milliseconds per execution" >> outputs/report
	


# make solution execution file, LIB_NAME is optional
# how to use: make solution LIB_NAME=libnowic_db.a 
solution:
	mkdir -p .log
ifeq ($(LIB_NAME),) # LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC1) -std=c++11 test_driver.cpp solution.cpp -I./include -o solution.out
else    
	AFL_USE_ASAN=1 $(CC1) -std=c++11 test_driver.cpp solution.cpp -I./include -L./lib -l$(patsubst lib%.a,%,$(LIB_NAME)) -o solution.out
endif


# make submission execution file, LIB_NAME is optional but SID is essential
# how to use : make submission SID=22000711 LIB_NAME=libnowic_db.a 
submission:
	mkdir -p .log
ifeq ($(LIB_NAME),) # LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC1) -std=c++11 test_driver.cpp  submissions/$(SID)/submission.cpp -I./include -o submissions/$(SID)/submission.out
else    
	AFL_USE_ASAN=1 $(CC1) -std=c++11 test_driver.cpp submissions/$(SID)/submission.cpp -I./include -L./lib -l$(patsubst lib%.a,%,$(LIB_NAME)) -o submissions/$(SID)/submission.out
endif


# solution file fuzzing
# how to use: make fz_solution 
fz_solution: 
	mkdir -p outputs/ok/temp
# If files exist in the outputs/ok/temp, delete them
	rm -rf outputs/ok/temp/*
# Create acc_result file and remove its content if it exists
	echo "" > outputs/ok/acc_result
	timeout 10s env AFL_NO_AFFINITY=1 $(FZ) -i inputs -o outputs ./solution.out 1 0 || true
	./outputs/ok/cal.sh



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
	timeout 10s env AFL_NO_AFFINITY=1 $(FZ) -i inputs -o submissions/$(SID)/outputs ./submissions/$(SID)/submission.out 0 0 || true



daemon.out: daemon.cpp
	$(CC2) $^ -o $@ $(CFLAGS) 

rs: daemon.out
	./$^ $(PORT)


clean:
	rm -rf outputs/default
	rm -rf outputs/ok/temp
	rm -rf outputs/ok/acc_result
	rm -f *.out







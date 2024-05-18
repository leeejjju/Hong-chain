CC=AFLplusplus/afl-clang++
FZ=AFLplusplus/afl-fuzz


# variables
INCLUDE_PATH ?= 
LIB_PATH ?= 
LIB_NAME ?= 
SID ?= 


CRASH_DIR = outputs/default/crashes/

check_for_crashes:
	@if ls $(CRASH_DIR)id* 1> /dev/null 2>&1; then \
	    echo "\nCrash Occurred....\nPlease fix the code."; \
	else \
	    echo "\nNo crashes detected\nYOU CAN USE DAEMON!!"; \
	fi
	


# create subdirectory of students
# make create_subdir SID=22000711
create_subdir:
	mkdir -p submissions/$(SID)


# make solution execution file
solution:
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

#	$(CC) -std=c++11 test_driver.cpp solution.cpp -I../include -L../lib -lnowic_linux -o solution



# make submission execution file
submission:
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
	AFL_NO_AFFINITY=1 $(FZ) -i inputs -o outputs ./solution.out
# AFL_NO_AFFINITY=1 make fz_solution 


# submission file fuzzing
fz_submission: 
# make report folder to save raw report
	mkdir -p submissions/$(SID)/report/crash	
	mkdir -p submissions/$(SID)/report/incorrect
# make outputs folder to save fuzz results
	mkdir -p submissions/$(SID)/outputs	
	cd submissions/$(SID) && AFL_NO_AFFINITY=1 ../../$(FZ) -i ../../inputs -o outputs ./submission.out
#	AFL_NO_AFFINITY=1 $(FZ) -i inputs -o submissions/$(SID)/outputs submissions/$(SID)/submission.out
# how to use :
# timeout 10s env make fz_submission SID=22000711



clean:
	rm -rf outputs
	rm *.out





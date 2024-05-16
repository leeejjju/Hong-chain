CC=AFLplusplus/afl-clang++
FZ=AFLplusplus/afl-fuzz


# variables
INCLUDE_PATH ?= 
LIB_PATH ?= 
LIB_NAME ?= 
STUDENT_PATH ?= 


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
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp $(STUDENT_PATH)/submission.cpp -o $(STUDENT_PATH)/submission.out
else	# INCLUDE_PATH is empty, but LIB_NAME is defiend
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp $(STUDENT_PATH)/submission.cpp -L$(LIB_PATH) -l$(patsubst lib%.a,%,$(LIB_NAME)) -o $(STUDENT_PATH)/submission.out
endif
else ifeq ($(LIB_NAME),)	# INCLUDE_PATH is defiend, but LIB_NAME is empty
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp  $(STUDENT_PATH)/submission.cpp -I$(INCLUDE_PATH) -o $(STUDENT_PATH)/submission.out
else	# INCLUDE_PATH and LIB_NAME are defiend
	AFL_USE_ASAN=1 $(CC) -std=c++11 test_driver.cpp $(STUDENT_PATH)/submission.cpp -I$(INCLUDE_PATH) -L$(LIB_PATH) -l$(patsubst lib%.a,%,$(LIB_NAME)) -o $(STUDENT_PATH)/submission.out
endif

# $(CC) -std=c++11 test_driver.cpp submission.cpp -Iinclude -Llib -libnowic_db -o submission
# how to use :
# make submission STUDENT_PATH=submissions/22000711 INCLUDE_PATH=include LIB_PATH=lib LIB_NAME=libnowic_db.a 


# solution file fuzzing
fz_solution: 
	mkdir -p outputs
	AFL_NO_AFFINITY=1 $(FZ) -i inputs -o outputs ./solution.out

# AFL_NO_AFFINITY=1 make fz_solution 


# submission file fuzzing
fz_submission: 
# make report folder to save raw report
	mkdir -p $(STUDENT_PATH)/report/crash	
	mkdir -p $(STUDENT_PATH)/report/incorrect
# make outputs folder to save fuzz results
	mkdir -p $(STUDENT_PATH)/outputs		
	AFL_NO_AFFINITY=1 $(FZ) -i inputs -o $(STUDENT_PATH)/outputs $(STUDENT_PATH)/submission.out
# how to use :
# timeout 10s env make fz_submission STUDENT_PATH=submissions/22000711



clean:
	rm -rf outputs
	rm *.out




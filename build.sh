#!/bin/bash


# read variables value (INCLUDE_PATH, LIB_PATH, LIB_NAME)
LIB_PATH=$1
LIB_NAME=$2

# make solution execution file
if [[-n "$LIB_PATH" && -n "$LIB_NAME" ]]; then   # INCLUDE_PATH, LIB_NAME, LIB_PATH are defined
    make solution INCLUDE_PATH="include" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
else
    make solution INCLUDE_PATH="include"   
fi 
# elif [[ -n "$INCLUDE_PATH" && ! -n "$LIB_PATH" && ! -n "$LIB_NAME" ]]; then     # only INCLUDE_PATH is defined
#     make solution INCLUDE_PATH="include"
# elif [[ -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then    # only LIB_NAME, LIB_PATH are defined"
#     make solution LIB_PATH="include" LIB_NAME="$LIB_NAME"
# else      
#     if [[ -n "$LIB_PATH" || -n "$LIB_NAME" ]]; then   # if either LIB_PATH or LIB_NAME is defined
#         echo "WARNING: Only one of LIB_PATH or LIB_NAME is defined. Please define both." >&2
#         exit 1
#     fi
#         make solution   # nothing defined
# fi



# solution fuzzing
make fz_solution || true

sleep 1s

rm -rf .log/*

# provide the verification report of the solution to the instructor
make check_for_crashes 





#!/bin/bash

# read variables value (INCLUDE_PATH, LIB_PATH, LIB_NAME)
LIB_NAME=$1

# make solution execution file
if [ -n "$LIB_NAME" ]; then   # LIB_NAME is defined
    make solution LIB_NAME="$LIB_NAME"
else
    make solution 
fi 

# solution fuzzing
make fz_solution 

# provide the verification reporst of the solution to the instructor
make check_for_crashes

#!/bin/bash

# read variables value
read -p "Enter INCLUDE_PATH: " INCLUDE_PATH
read -p "Enter LIB_PATH: " LIB_PATH
read -p "Enter LIB_NAME: " LIB_NAME
read -p "Enter SEED_DIR: " SEED_DIR


# Check if SEED_DIR is defined
if [[ -z "$SEED_DIR" ]]; then
    echo "SEED_DIR is not defined. Please enter a value for SEED_DIR."
    exit 1
fi

# execute make file
if [[ -n "$INCLUDE_PATH" && -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then   # INCLUDE_PATH, LIB_NAME, LIB_PATH are defiend
    make solution INCLUDE_PATH="$INCLUDE_PATH" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
    make submission INCLUDE_PATH="$INCLUDE_PATH" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
elif [[ -n "$INCLUDE_PATH" ]]; then     # only INCLUDE_PATH is defiend
    make solution INCLUDE_PATH="$INCLUDE_PATH"
    make submission INCLUDE_PATH="$INCLUDE_PATH"
elif [[ -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then    # only LIB_NAME, LIB_PAHT are defiend
    make solution LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
    make submission LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
else        # nothing defiend
    make submission
fi


# fuzzing
timeout 10s env AFL_NO_AFFINITY=1 make f_solution SEED_DIR="$SEED_DIR" 
timeout 10s env AFL_NO_AFFINITY=1 make f_submission SEED_DIR="$SEED_DIR"


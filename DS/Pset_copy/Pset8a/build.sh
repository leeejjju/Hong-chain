#!/bin/bash

# read variables value
read -p "Enter INCLUDE_PATH: " INCLUDE_PATH
read -p "Enter LIB_PATH: " LIB_PATH
read -p "Enter LIB_NAME: " LIB_NAME
read -p "Enter SEED_DIR: " SEED_DIR

# execute make file
make solution INCLUDE_PATH="$INCLUDE_PATH" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
make submission INCLUDE_PATH="$INCLUDE_PATH" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
timeout 10s env AFL_NO_AFFINITY=1 make f_solution SEED_DIR="$SEED_DIR" 
timeout 10s env AFL_NO_AFFINITY=1 make f_submission SEED_DIR="$SEED_DIR"


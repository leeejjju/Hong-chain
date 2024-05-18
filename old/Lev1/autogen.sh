#!/bin/bash

# Check if the number of arguments is correct
if [ $# -ne 1 ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

# Assign the argument to a variable
target=$1

# change makefile to target
sed -i "s/TARGET =.*/TARGET = $target/g" makefile
# TODO: change queue_coverage.sh to target
# TODO: change crash_coverage.sh to target

# Make btree.out
make clean
rm -rf outputs/*

echo "make $target.out"

# Make fuzz and run for 30 seconds
export TARGET=$target
make fuzz &
sleep 30
kill %1

# Run queue_coverage.sh
./queue_coverage.sh

# Run crash_coverage.sh
./crash_coverage.sh

# Make sbfl
make sbfl

# Run sbfl.out
./sbfl.out

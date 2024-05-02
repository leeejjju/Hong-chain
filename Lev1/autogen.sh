#!/bin/bash

# Make btree.out
make clean
rm -rf outputs/*
make btree.out

# Make fuzz and run for 30 seconds
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

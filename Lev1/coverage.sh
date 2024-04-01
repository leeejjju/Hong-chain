#!/bin/bash

# Create directories
mkdir -p cov/crashes
mkdir -p cov/queue

# Process crashes
# for file in outputs/default/crashes/*; do
#     if [ -f "$file" ]; then
#         ./btree < "$file"
#         gcov -bcif "$file"
#         mv *.gcov "cov/crashes/$(basename "$file").gcov"
#         rm -f *.gcda
#     fi
# done

# Process queue
for file in outputs/default/queue/*; do
    if [ -f "$file" ]; then
        ./btree < "$file"
        gcov -bcif *.gcda
        mv *.gcov "cov/queue/$(basename "$file").gcov"
        rm -f *.gcda
    fi
done

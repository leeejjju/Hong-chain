#!/bin/bash

# Create directories
mkdir -p cov/crashes
mkdir -p cov/queue

# Process crashes
# need human's touch...

# Process queue
rm *.gcda -f
for file in outputs/default/queue/*; do
    if [ -f "$file" ]; then
        ./btree.out < "$file"
    fi
done

gcov btree.gcda
mv *.gcda cov/queue
mv *.gcov cov/queue

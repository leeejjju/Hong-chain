#!/bin/bash

# Create directories
mkdir -p cov/crashes
mkdir -p cov/queue

# Process crashes
# need human's touch...

# Process queue
for file in outputs/default/queue/*; do
    if [ -f "$file" ]; then
        ./btree < "$file"
    fi
done

gcov btree.gcda

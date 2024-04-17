#!/bin/bash

# Create directories
mkdir -p gcov/queue

# Process queue
for file in outputs/default/queue/*; do
    if [ -f "$file" ]; then
		filename=$(basename "$file")
        ./btree.out < "$file"
		gcov btree.gcda
		mv *.gcov gcov/queue/$filename.gcov
		echo "saved $filename.gcov"
		rm *.gcda -f
		rm *.gcov -f
    fi
done


echo "\n- - - - - - - - - - all the gcov files are saved - - - - - - - - - -\n"
ls gcov/queue
echo "\n- - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - -\n"


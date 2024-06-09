#!/bin/bash

# File paths
temp_dir="outputs/ok/temp"
acc_result="outputs/ok/acc_result"

# Initialize the accumulated values if acc_result does not exist
if [ ! -f "$acc_result" ]; then
    echo -e "total_input : 0\ntotal_rep : 0\ninput_pass : 0\ninput_fail : 0\nrep_pass : 0\nrep_fail : 0" > "$acc_result"
fi

# Read the values from temp files and accumulate
total_input=0
total_rep=0
input_pass=0
input_fail=0
rep_pass=0
rep_fail=0

# Loop through temp directory and accumulate values from files
for file in "$temp_dir"/ok_*; do
    if [ -f "$file" ]; then
        total_input=$((total_input + $(awk '/total_input/ {print $3}' "$file")))
        total_rep=$((total_rep + $(awk '/total_rep/ {print $3}' "$file")))
        input_pass=$((input_pass + $(awk '/input_pass/ {print $3}' "$file")))
        input_fail=$((input_fail + $(awk '/input_fail/ {print $3}' "$file")))
        rep_pass=$((rep_pass + $(awk '/rep_pass/ {print $3}' "$file")))
        rep_fail=$((rep_fail + $(awk '/rep_fail/ {print $3}' "$file")))
    fi
done

# Write the accumulated values to acc_result
echo -e "total_input : $total_input\ninput_pass : $input_pass\ninput_fail : $input_fail\ninput_pass_percentage : $(awk "BEGIN {print $input_pass/$total_input*100}")%\n\ntotal_rep : $total_rep\nrep_pass : $rep_pass\nrep_fail : $rep_fail\nrep_pass_percentage : $(awk "BEGIN {print $rep_pass/$total_rep*100}")%" > "$acc_result"

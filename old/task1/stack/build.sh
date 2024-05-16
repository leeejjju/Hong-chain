# for instructor
# 1. afl build (solution.cpp) 2. fuzz solution 3. 교수한테 report


# read variables value (INCLUDE_PATH, LIB_PATH, LIB_NAME)
read -p "Enter INCLUDE_PATH: " INCLUDE_PATH
read -p "Enter LIB_PATH: " LIB_PATH
read -p "Enter LIB_NAME: " LIB_NAME


# make solution execution file
if [[ -n "$INCLUDE_PATH" && -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then   # INCLUDE_PATH, LIB_NAME, LIB_PATH are defiend
    make solution INCLUDE_PATH="$INCLUDE_PATH" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
elif [[ -n "$INCLUDE_PATH" ]]; then     # only INCLUDE_PATH is defiend
    make solution INCLUDE_PATH="$INCLUDE_PATH"
elif [[ -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then    # only LIB_NAME, LIB_PAHT are defiend
    make solution LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
else        # nothing defiend
    make solution
fi






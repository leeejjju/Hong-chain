# for instructor
# 1. afl build (solution.cpp) 2. fuzz solution 3. 교수한테 report


# read variables value (INCLUDE_PATH, LIB_PATH, LIB_NAME)
read -p "Enter INCLUDE_PATH: " INCLUDE_PATH
read -p "Enter LIB_PATH: " LIB_PATH
read -p "Enter LIB_NAME: " LIB_NAME


# make solution execution file
if [[ -n "$INCLUDE_PATH" && -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then   # INCLUDE_PATH, LIB_NAME, LIB_PATH are defined
    make solution INCLUDE_PATH="$INCLUDE_PATH" LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
elif [[ -n "$INCLUDE_PATH" && ! -n "$LIB_PATH" && ! -n "$LIB_NAME" ]]; then     # only INCLUDE_PATH is defined
    make solution INCLUDE_PATH="$INCLUDE_PATH"
elif [[ -n "$LIB_PATH" && -n "$LIB_NAME" ]]; then    # only LIB_NAME, LIB_PATH are defined"
    make solution LIB_PATH="$LIB_PATH" LIB_NAME="$LIB_NAME"
else      
    if [[ -n "$LIB_PATH" || -n "$LIB_NAME" ]]; then   # if either LIB_PATH or LIB_NAME is defined
        echo "WARNING: Only one of LIB_PATH or LIB_NAME is defined. Please define both." >&2
        exit 1
    fi
        make solution   # nothing defined
fi


echo "from1"


# solution fuzzing
timeout 10s env make fz_solution || true


echo "from2" 

# provide the verification report of the solution to the instructor
make check_for_crashes || true











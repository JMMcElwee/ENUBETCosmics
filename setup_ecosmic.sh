#!/bin/bash

# Default path
ECOSMIC_SRC_DEFAULT=${PWD}

# Detect OS and use different library paths
case "$(uname)" in
    Darwin)  # macOS
        LIB_PATH_VAR="DYLD_LIBRARY_PATH"
        ;;
    Linux)
        LIB_PATH_VAR="LD_LIBRARY_PATH"
        ;;
    *)
        echo "Unsupported operating system: $(uname)" >&2
        exit 1
        ;;
esac

# Set ECosmic paths
export ECOSMIC_SRC="${ECOSMIC_SRC:-$ECOSMIC_SRC_DEFAULT}"  # Use existing or default
export ECOSMIC_LIB="${ECOSMIC_SRC}/lib"
export ECOSMIC_INC="${ECOSMIC_SRC}/include"

# Update library path
export ${LIB_PATH_VAR}="${ECOSMIC_LIB}:${!LIB_PATH_VAR}"

